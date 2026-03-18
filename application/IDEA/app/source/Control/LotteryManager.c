#include "LotteryManager.h"
#include "ComputerData.h"
#include "Test.h"

// 静态函数声明
//static void LotteryManager_SaveData(LotteryManager* manager);
static int32_t LotteryManager_GetTierPermil(const LotteryManager* manager, int32_t score);
static void LotteryManager_ApplyGuaranteedGrowth(LotteryManager* manager);

/**
 * @brief 初始化 LotteryManager 实例。
 */
LotteryManager gLotteryManager;  // 全局单例
void LotteryManager_Init(void)
{
    // 初始化运行时字段
    gLotteryManager.mTotalPlayTime = 0;
    gLotteryManager.mTotalPlay = 0;
    gLotteryManager.mTotalDraw =0;
    gLotteryManager.mScale = 1;  // 彩金缩放系数
    gLotteryManager.mFrozenTime = 0;
    gLotteryManager.mFrozenTime = 0;
    gLotteryManager.mInjectRatePermil = 1200;     // 基础注入倍率：1.2x
    gLotteryManager.mTierMidBet = 1000;           // 中档下注阈值
    gLotteryManager.mTierHighBet = 5000;          // 高档下注阈值
    gLotteryManager.mTierMidInjectPermil = 1200;  // 中档注入：1.2x
    gLotteryManager.mTierHighInjectPermil = 1500; // 高档注入：1.5x
    gLotteryManager.mGuaranteeEveryNPlay = 20;    // 每 20 局触发一次保底增长
    gLotteryManager.mGuaranteeMinGrowthPerLottery = 1; // 每池最小保底增长
    gLotteryManager.mGuaranteePlayCounter = 0;
    gLotteryManager.mWinFreezeTime = 120;         // 命中彩金后的冷却
    gLotteryManager.mMinPlayScoreToTrigger = 50;  // 触发开奖的最小下注
    gLotteryManager.mMinPlayGapAfterWin = 10;     // 两次开奖最小间隔局数
    gLotteryManager.mPlaySinceLastWin = gLotteryManager.mMinPlayGapAfterWin;

    int32_t nLocalJpUnit = JPDrawRateUnit[3];
    int32_t minThresh =0;
    int32_t threshRange[] = { 25, 50, 75, 100 };

    minThresh = threshRange[0];
    for (int8_t i = 0; i < GAME_Local_JP_MAX; i++)
    {
        Lottery_Init
        (
            &gLotteryManager.mLotterys[i],
            2 * gLotteryManager.mScale,
            minThresh,
            JPWeight[i+1] * nLocalJpUnit * gLotteryManager.mScale,  // 抽取率
            gLotteryManager.mScale);
    }

}

/**
 * @brief 重置 LotteryManager 实例。
 */
void LotteryManager_Destroy(LotteryManager* manager)
{
    if (manager == NULL) return;

    // 重置运行时字段
    manager->mTotalPlayTime =0;
    manager->mTotalPlay = 0;
    manager->mTotalDraw = 0;
    manager->mFrozenTime = 0;
    manager->mGuaranteePlayCounter = 0;
    manager->mPlaySinceLastWin = 0;
}

/**
 * @brief 设置基础值和上限值。
 */
void LotteryManager_SetBaseValue(LotteryManager* manager, int32_t baseValue[3], int32_t maxValue[3])
{
    if (manager == NULL) return;

    for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
    {
       Lottery_SetBaseValue(&manager->mLotterys[i], baseValue[i]* manager->mScale, maxValue[i] * manager->mScale);
    }
}

/**
 * @brief 处理一次游玩事件。
 */
void LotteryManager_OnPlay(LotteryManager* manager, int32_t value)
{
    if (manager == NULL)
    {
        return;
    }

    manager->mTotalPlayTime++;
    manager->mPlaySinceLastWin++;
    manager->mGuaranteePlayCounter++;

#ifdef _DebugControlMode

#endif

    const int32_t tierPermil = LotteryManager_GetTierPermil(manager, value);
    int32_t injectValue = (int32_t)(((int64_t)value * manager->mInjectRatePermil * tierPermil) / 1000000);
    if (injectValue < 1)
    {
        injectValue = 1;
    }

    for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
    {
        Lottery_OnPlay(&manager->mLotterys[i], injectValue);
    }

    //触发保底增长
    if (manager->mGuaranteeEveryNPlay > 0 &&
        manager->mGuaranteePlayCounter >= manager->mGuaranteeEveryNPlay)
    {
        LotteryManager_ApplyGuaranteedGrowth(manager);
        manager->mGuaranteePlayCounter = 0;
    }

    manager->mTotalPlay += value / 100;
}

/**
 * @brief 尝试触发一次开奖。
 */
int32_t LotteryManager_TryGetLottery(LotteryManager* manager, int32_t playScore, int32_t* id, int32_t* val)
{
    if (manager == NULL || id == NULL || val == NULL) return 0;
    if (manager->mFrozenTime > 0) return 0;//处于开奖冻结期，不允许开奖。
    if (playScore < manager->mMinPlayScoreToTrigger) return 0;//下注太小，不触发开奖。
    if (manager->mPlaySinceLastWin < manager->mMinPlayGapAfterWin) return 0;//离上次中奖间隔局数不足，不开奖。

    int32_t res = 0;//本次是否开奖成功
    int32_t fRate = 0.0f;
    int32_t ShowLottery[3] = { 0 };
    int32_t LotteryPool[3] = { 0 };
    int32_t LotteryThresh[3] = { 0 };

    // 读取所有彩金池的当前状态
    for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
    {
        ShowLottery[i] = Lottery_GetShowValue(&manager->mLotterys[i]);
        LotteryPool[i] = Lottery_GetLotteryPool(&manager->mLotterys[i]);
        LotteryThresh[i] = Lottery_GetThresh(&manager->mLotterys[i]);
    }

    // 先尝试指定 ID 的彩金池
    if (*id > 0)
    {
        (*id)--; // 转为数组下标
        if (*id >= 0 && *id < GAME_Local_JP_MAX)
        {
            res = Lottery_TryGet(&manager->mLotterys[*id], playScore, val, &fRate);
            if (res)
            {
                manager->mFrozenTime += manager->mWinFreezeTime;
                manager->mPlaySinceLastWin = 0;
            }
        }
    }
    else
    {
        // 否则遍历所有彩金池
        for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
        {

            res = Lottery_TryGet(&manager->mLotterys[i], playScore, val, &fRate);
            if (res)
            {
                *id = i;
                manager->mFrozenTime += manager->mWinFreezeTime;
                manager->mPlaySinceLastWin = 0;
                break;
            }
        }
    }

    return res;
}



static int32_t LotteryManager_GetTierPermil(const LotteryManager* manager, int32_t score)
{
    if (score >= manager->mTierHighBet)
    {
        return manager->mTierHighInjectPermil;
    }

    if (score >= manager->mTierMidBet)
    {
        return manager->mTierMidInjectPermil;
    }

    return 1000;
}

/**
 * @brief 彩金保底增长
 */
static void LotteryManager_ApplyGuaranteedGrowth(LotteryManager* manager)
{
    const int32_t growth = manager->mGuaranteeMinGrowthPerLottery * manager->mScale;
    if (growth <= 0)
    {
        return;
    }

    for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
    {
        Lottery* lottery = &manager->mLotterys[i];
        // 保底同量增长，显示与实际池保持同步。
        lottery->mLotteryPool += growth;
        lottery->mShowLottery += growth;
        if (lottery->mShowLottery > lottery->mMaxLottery)
        {
            lottery->mShowLottery = lottery->mMaxLottery;
        }
    }
}
