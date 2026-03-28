#include "LotteryManager.h"
#include "ComputerData.h"
#include "Test.h"


static int32_t LotteryManager_GetTierPermil(const LotteryManager* manager, int32_t score);

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
    gLotteryManager.mInjectRatePermil = 1000;     // 基础注入倍率：1.0x
    gLotteryManager.mTierMidBet = 1000;           // 中档下注阈值
    gLotteryManager.mTierHighBet = 5000;          // 高档下注阈值
    gLotteryManager.mTierMidInjectPermil = 1200;  // 中档注入：1.2x
    gLotteryManager.mTierHighInjectPermil = 1500; // 高档注入：1.5x
    gLotteryManager.mWinFreezeTime = 0;         // 命中彩金后的冷却
    gLotteryManager.mMinPlayScoreToTrigger = 20;  // 触发开奖的最小下注
    gLotteryManager.mMinPlayGapAfterWin = 10;     // 两次开奖最小间隔局数
    gLotteryManager.mPlaySinceLastWin = gLotteryManager.mMinPlayGapAfterWin;

    int32_t nLocalJpUnit = JPDrawRateUnit[4];
    int32_t minThresh =0;
    int32_t threshRange[] = { 20, 50, 75, 100 };

    minThresh = threshRange[0];
    for (int8_t i = 0; i < GAME_Local_JP_MAX; i++)
    {
        Lottery_Init
        (
            &gLotteryManager.mLotterys[i],
            2 * gLotteryManager.mScale, // 开奖阈值上限
            minThresh, // 开奖阈值下限
            JPWeight[i+1] * nLocalJpUnit * gLotteryManager.mScale,  // 抽取率
            gLotteryManager.mScale // 彩金上限
        );
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
                break;
            }
        }
    }

    return res;
}

// 候选彩金放行后，提交到本地奖池并更新冷却/间隔。
void LotteryManager_CommitLottery(LotteryManager* manager, int32_t id, int32_t val)
{
    int32_t idx = id;
    if (manager == NULL || val <= 0)
    {
        return;
    }

    if (idx < 0 || idx >= GAME_Local_JP_MAX)
    {
        return;
    }

    Lottery_CommitGet(&manager->mLotterys[idx], val);
    manager->mTotalDraw += val;
    manager->mFrozenTime += manager->mWinFreezeTime;
    manager->mPlaySinceLastWin = 0;
}

// 回补已扣出的本地彩金到指定奖池。
void LotteryManager_RefundLottery(LotteryManager* manager, int32_t id, int32_t val)
{
    int32_t idx = id;
    if (manager == NULL || val <= 0)
    {
        return;
    }

    // 兼容两种入参：0~2（数组下标）或 1~3（业务类型值）。
    if (idx >= 1 && idx <= GAME_Local_JP_MAX)
    {
        idx -= 1;
    }
    if (idx < 0 || idx >= GAME_Local_JP_MAX)
    {
        return;
    }

    manager->mLotterys[idx].mLotteryPool += val;
    if (manager->mTotalDraw >= val)
    {
        manager->mTotalDraw -= val;
    }
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

