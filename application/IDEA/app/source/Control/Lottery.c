#include "Lottery.h"
#include "../GameAlgo/common/JRand.h"

// mDrawRate 使用万分比配置（1/10000）
#define LOTTERY_DRAW_RATE_DENOM 10000


void Lottery_Init(Lottery* self,int32_t threshMax, int32_t threshMin, int32_t drawRate, int32_t scale)
{
    self->mBaseLottery = 0;
    self->mThreshMax = threshMax;   // 开奖阈值上限
    self->mThreshMin = threshMin;   // 开奖阈值下限
    self->mDrawRate = drawRate;     // 抽取率（万分比）
    self->mMaxLottery = 0;          // 彩金上限，后续由 SetBaseValue 初始化
    self->scale = scale;
    self->mAccumScore = 0;
    self->mAccumDrawVal = 0;
    self->mShowSpeedPermil = 300;
}

void Lottery_SetBaseValue(Lottery* self, int32_t baseValue, int32_t maxValue)
{
    self->mBaseLottery = baseValue;                 // 基础值：彩金初始值
    self->mShowLottery = self->mBaseLottery;        // 显示值：初始化为基础值
    self->mLotteryPool = self->mBaseLottery * 0.6f;// 实际奖池金额（初始为基础值的60%）
    self->mTotalAccumPool = 0;
    self->mAccumScore = 0;
    self->mAccumDrawVal = 0;
    self->mShowSpeedPermil = 300;

    // 上限边界校验：限制在基础值的 1.5~10 倍范围
    if (maxValue < self->mBaseLottery * 1.5)
    {
        maxValue = self->mBaseLottery * 1.5;
    }
    if (maxValue > self->mBaseLottery * 10)
    {
        maxValue = self->mBaseLottery * 10;
    }
    self->mMaxLottery = maxValue;

    // 计算下一次开奖阈值：基础值 + (上限-基础) * 随机系数
    self->mNextGiveLotteryThresh = self->mBaseLottery * 1 +(self->mMaxLottery - self->mBaseLottery) * JRandFrom(45, 95) /100;
}

int32_t Lottery_TryGet(Lottery* self, int32_t playScore, int32_t* val, int32_t* rate)
{
    // 先检查是否满足开奖条件
    if (!Lottery_CheckGet(self, playScore))
    {
        return 0;
    }

    //最小下注
    int32_t scoreInCoin = playScore;
    if (scoreInCoin < self->mThreshMin)
    {
        return 0;
    }

    // 本次派奖值取当前显示值（整数）
    int32_t nIntVal = (int32_t)self->mShowLottery;
    *val = nIntVal * 1;
    if (rate != NULL)
    {
        *rate = 0;
    }

    return 1;
}

void Lottery_CommitGet(Lottery* self, int32_t val)
{
    if (self == NULL || val <= 0)
    {
        return;
    }

    // 放行后再落账：扣池、重置显示值、重置阈值与统计。
    self->mLotteryPool -= val;
    self->mShowLottery = self->mBaseLottery;
    if (self->mLotteryPool < 0)
    {
        self->mLotteryPool = 0;
    }
    if (self->mLotteryPool > self->mBaseLottery * 1)
    {
        self->mLotteryPool = self->mBaseLottery;
    }
    self->mNextGiveLotteryThresh = self->mBaseLottery * 1 + (self->mMaxLottery - self->mBaseLottery) * JRandFrom(40, 95) / 100;
    self->mShowSpeedPermil = 300;
    self->mTotalGivePool += val;
    self->mTotalGiveTime++;
}

int32_t Lottery_CheckGet(Lottery* self, int32_t playScore)
{
    // 条件1：池值需覆盖显示值，且达到开奖阈值
    if (self->mLotteryPool < self->mShowLottery || self->mLotteryPool < self->mNextGiveLotteryThresh)
    {
        return 0;
    }

    // 条件2：本局分值需达到最小阈值
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
    // 显示值超过上限时直接返回
    if (self->mShowLottery > self->mMaxLottery)
    {
        return;
    }

    // 累计分值
    self->mAccumScore += score;

    // 计算当前可抽取值
    self->mAccumDrawVal = (int32_t)(((int64_t)self->mAccumScore * self->mDrawRate) / LOTTERY_DRAW_RATE_DENOM);

    if (self->mAccumDrawVal > 5)
    {
        int32_t poolDiffVal = 0;
        int32_t showDiffVal = 0;
        int32_t deltaPermil = 0;
        int32_t showDrawVal = 0;

        self->mTotalAccumPool += self->mAccumDrawVal;

        // 动态计算显示值增量：根据 show 与 pool 到阈值的距离调整增长速度。
        poolDiffVal = self->mNextGiveLotteryThresh - self->mLotteryPool;
        if (poolDiffVal < 1)
        {
            poolDiffVal = 1;
        }

        showDiffVal = self->mNextGiveLotteryThresh - self->mShowLottery;
        if (showDiffVal < 1)
        {
            showDiffVal = 1;
        }

        // 等价于 fDelta = showDiff/poolDiff，使用千分比避免浮点。
        deltaPermil = (int32_t)(((int64_t)showDiffVal * 1000) / poolDiffVal);
        if (deltaPermil > 950) deltaPermil = 950;
        if (deltaPermil < 50) deltaPermil = 50;

        showDrawVal = (int32_t)(((int64_t)self->mAccumDrawVal * deltaPermil) / 1000);
        if (showDrawVal < 1)
        {
            showDrawVal = 1;
        }

        self->mShowLottery += showDrawVal;
        self->mLotteryPool += self->mAccumDrawVal;

        if (self->mShowLottery > self->mMaxLottery)
        {
            self->mShowLottery = self->mMaxLottery;
        }

        // 回扣已消费分值
        self->mAccumScore -= (int32_t)(((int64_t)self->mAccumDrawVal * LOTTERY_DRAW_RATE_DENOM) / self->mDrawRate);
        if (self->mAccumScore < 0)
        {
            self->mAccumScore = 0;
        }
    }
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

