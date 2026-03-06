#include "Lottery.h"
#include "../GameAlgo/common/JRand.h"

void Lottery_Init(Lottery* self,int32_t threshMax, int32_t threshMin, int32_t drawRate, int32_t scale)
{
    self->mBaseLottery = 0;
    self->mThreshMax = threshMax;   // 抽奖阈值上限（触发抽奖的最大游戏得分）
    self->mThreshMin = threshMin;   // 抽奖阈值下限（触发抽奖的最小游戏得分）
    self->mDrawRate = drawRate;     // 抽取率（决定每局游戏贡献多少到奖池）
    self->mMaxLottery = 0;         // 彩票最大值（后续由setBaseValue初始化）
    self->scale = scale;
}

void Lottery_SetBaseValue(Lottery* self, int32_t baseValue, int32_t maxValue)
{
    self->mBaseLottery = baseValue;// 基础值（彩票的初始基数）
    self->mShowLottery = self->mBaseLottery;// 显示值（前端展示的奖池数值，初始等于基础值）
    self->mLotteryPool = self->mBaseLottery * 0.6;// 实际奖池金额（初始为基础值的60%）

    // 最大值边界校验：限制在基础值的1.5~10倍之间（超过10倍强制拉回1.5倍）
    if (maxValue < self->mBaseLottery * 1.5) 
    {
        maxValue = self->mBaseLottery * 1.5;
    }
    if (maxValue > self->mBaseLottery * 10) 
    {
        maxValue = self->mBaseLottery * 1.5;
    }
    self->mMaxLottery = maxValue;

    // 高精度变量同步（避免浮点数精度丢失）
    self->mDbShowLottery = self->mShowLottery;
    self->mDbLotteryPool = self->mLotteryPool;

    // 生成下一次抽奖阈值：基础值 + (最大值-基础值) * 45~95%的随机数
    self->mNextGiveLotteryThresh = self->mBaseLottery * 1 +(self->mMaxLottery - self->mBaseLottery) * JRandFrom(45, 95) /100;
}

void Lottery_SetShowValue(Lottery* self, int32_t showValue) 
{
    //显示值不能小于基础值，且不能等于/超过最大值
    if (showValue < self->mBaseLottery || self->mMaxLottery - showValue <= 0)
    {
        return;
    }

    // 重新生成下一次抽奖阈值：显示值 + (最大值-显示值) * 40~95%的随机数
    self->mNextGiveLotteryThresh = showValue * 1 +(self->mMaxLottery - showValue) * JRandFrom(40, 95) /100;

    // 动态计算奖池金额：基于新阈值和随机系数（45~95%）反向推导
    int32_t fGiveDiffValue = self->mNextGiveLotteryThresh - showValue;
    self->mLotteryPool = self->mNextGiveLotteryThresh -fGiveDiffValue / (JRandFrom(45, 95) / 100);
    self->mShowLottery = showValue;

    // 高精度变量同步
    self->mDbShowLottery = self->mShowLottery;
    self->mDbLotteryPool = self->mLotteryPool;
}

int32_t Lottery_TryGet(Lottery* self, int32_t playScore, int32_t* val, int32_t* rate)
{
    // 先校验条件，不满足直接返回
    if (!Lottery_CheckGet(self, playScore))
    {
        return 0;
    }
    

    int32_t scoreInCoin = playScore;
    if (scoreInCoin < self->mThreshMin)
    {
        return 0;
    }

    // 计算中奖金额：取显示值的整数部分（避免小数）
    int32_t nIntVal = (int32_t)self->mShowLottery;
    *val = nIntVal * 1;
    //扣除彩金
    self->mLotteryPool -= *val;
    // 重置显示值为基础值（抽奖后奖池回退到初始状态）
    self->mShowLottery = self->mBaseLottery;
    // 同步高精度变量
    self->mDbShowLottery = self->mShowLottery;
    self->mDbLotteryPool = self->mLotteryPool;
    // 修复奖池负数，并重设下一次抽奖阈值
    if (self->mLotteryPool < 0)
    {
        self->mLotteryPool = 0;
    }

    if (self->mLotteryPool > self->mBaseLottery * 1)
    {
        self->mLotteryPool = self->mBaseLottery * 0.6;
    }
    self->mNextGiveLotteryThresh = self->mBaseLottery * 1 +(self->mMaxLottery - self->mBaseLottery) * JRandFrom(40, 95)/100;

    self->mTotalGivePool += *val;
    self->mTotalGiveTime++;
    if (self->mTotalGiveTime > 50000)
    {
        self->mTotalGiveTime = 0;
    }

    return 1;
}

int32_t Lottery_CheckGet(Lottery* self, int32_t playScore)
{
    // 条件1：实际奖池 ≥ 显示值 且 实际奖池 ≥ 下一次抽奖阈值
    if (self->mLotteryPool < self->mShowLottery || self->mLotteryPool < self->mNextGiveLotteryThresh) 
    {
        return 0;
    }

    // 条件2：游戏得分转换为金币后 ≥ 阈值下限
    //int32_t scoreInCoin = playScore / 100.0f;
    int32_t scoreInCoin = playScore;
    if (scoreInCoin < self->mThreshMin)
    {
        return 0;
    }

    return 1;
}

void Lottery_OnPlay(Lottery* self, int32_t score)
{
    //不让彩金超过最大值
    if (self->mDbShowLottery > self->mMaxLottery) 
    {
        return;
    }
    // 计算本局贡献到奖池的金额
    int32_t drawVal = score * self->mDrawRate / 1000000 ;
    // 累计总贡献金额
    self->mDbTotalAccumPool += drawVal;

    // 动态计算显示值的增量（核心算法）
    int32_t fShowDiffVal = self->mNextGiveLotteryThresh - self->mShowLottery; // 显示值与阈值的差值（最小0.01）
    if (fShowDiffVal < 0)
    {
        fShowDiffVal = 1;
    }

	int32_t fPoolDiffVal = self->mNextGiveLotteryThresh - self->mLotteryPool; // 奖池与阈值的差值（最小0.01，避免除零）
	if (fPoolDiffVal <0) 
    {
		fPoolDiffVal = 1;
	}
   
    // 计算增量系数：0.05~0.95之间（平衡显示值和奖池的增长速度）
	//int32_t fDelta = fShowDiffVal / fPoolDiffVal; 
	//if (fShowDiffVal / fPoolDiffVal > 95)
    //   {
    //       // 显示值增量 = 本局贡献 * 系数
    //       self->mDbShowLottery += drawVal * 0.95;
	//}

    //   if (fShowDiffVal / fPoolDiffVal < 5)
    //   {
    //       self->mDbShowLottery += drawVal * 0.05;
    //   }
    //int32_t ShowdrawVal = drawVal * fShowDiffVal / fPoolDiffVal;
    int32_t ShowdrawVal = drawVal *JRandFrom(5,95)/100;
    self->mDbShowLottery += ShowdrawVal;
    // 奖池金额累加本局贡献
    self->mDbLotteryPool += drawVal;
    // 同步到int32_t变量（供外部调用）
    self->mShowLottery = self->mDbShowLottery;
    self->mLotteryPool = self->mDbLotteryPool;
    self->mTotalAccumPool = self->mDbTotalAccumPool;

    //_gc.realTimeParam.fLocalJpPool += drawVal;
}

int32_t Lottery_GetShowValue(Lottery* self)
{
    return self->mShowLottery;
}

int32_t Lottery_GetLotteryPool(Lottery* self) {
    return self->mLotteryPool;
}

int32_t Lottery_GetThresh(Lottery* self) {
    return self->mNextGiveLotteryThresh;
}

int32_t Lottery_GetTotalGiveTime(Lottery* self) {
    return self->mTotalGiveTime;
}

int32_t Lottery_GetTotalGiveVal(Lottery* self) {
    return self->mTotalGivePool;
}

int32_t Lottery_GetBaseValue(Lottery* self) {
    return self->mBaseLottery;
}

int32_t Lottery_GetMaxValue(Lottery* self) {
    return self->mMaxLottery;
}