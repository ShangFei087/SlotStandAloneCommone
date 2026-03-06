#include "LotteryManager.h"
#include "ComputerData.h"
#include "Test.h"

// 静态函数声明（对应原私有方法）
//static void LotteryManager_SaveData(LotteryManager* manager);

/**
 * @brief 初始化 LotteryManager 实例
 */
LotteryManager gLotteryManager;  // 定义并初始化
void LotteryManager_Init(void) 
{
    // 初始化成员变量
    gLotteryManager.mTotalPlayTime = 0;
    gLotteryManager.mTotalPlay = 0;
    gLotteryManager.mTotalDraw =0;
    gLotteryManager.mScale = 1;  //假设放大100倍
    gLotteryManager.mFrozenTime = 0;
    gLotteryManager.mFrozenTime = -1.0f;

    int32_t nLocalJpUnit = JPDrawRateUnit[3];
    int32_t minThresh =0;
    int32_t threshRange[] = { 25, 50, 75, 100 };
	if (0 > 10) {
		minThresh = 25;
	}
	else {
		minThresh = threshRange[0];
	}
       

    for (int8_t i = 0; i < GAME_Local_JP_MAX; i++)
    {
        Lottery_Init(&gLotteryManager.mLotterys[i], 2 * gLotteryManager.mScale, minThresh, JPWeight[i+1] * nLocalJpUnit * gLotteryManager.mScale, gLotteryManager.mScale);
    }

}

/**
 * @brief 销毁 LotteryManager 实例
 */
void LotteryManager_Destroy(LotteryManager* manager) 
{
    if (manager == NULL) return;

    // 重置成员变量
    manager->mTotalPlayTime =0;
    manager->mTotalPlay = 0;
    manager->mTotalDraw = 0;
    manager->mFrozenTime = 0;
}

/**
 * @brief 设置彩票基础值和最大值
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
 * @brief 获取彩票基础值和最大值
 */
void LotteryManager_GetBaseValue(LotteryManager* manager, int32_t baseValue[3], int32_t maxValue[3])
{
    if (manager == NULL) return;

    for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
    {
         baseValue[i] = Lottery_GetBaseValue(&manager->mLotterys[i]);
         maxValue[i] = Lottery_GetMaxValue(&manager->mLotterys[i]);
    }
}

/**
 * @brief 设置彩票显示值
 */
void LotteryManager_SetShowValue(LotteryManager* manager, int32_t id, int32_t showValue) 
{
      Lottery_SetShowValue(&manager->mLotterys[id], showValue);
}


/**
 * @brief 处理游玩逻辑
 */
void LotteryManager_OnPlay(LotteryManager* manager, int32_t value)
{
#ifndef _LocalLottery
    return;
#endif
    manager->mTotalPlayTime++;

#ifdef _DebugControlMode
    //value *= _gc.debugParam.nLotterAccumScale;
#endif

    for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
    {
        Lottery_OnPlay(&manager->mLotterys[i], value);
    }

    manager->mTotalPlay += value / 100;
}

/**
 * @brief 获取彩票显示值
 */
int32_t LotteryManager_GetLotteryShowValue(LotteryManager* manager, int32_t id)
{
    return Lottery_GetShowValue(&manager->mLotterys[id]);
}

/**
 * @brief 尝试获取彩票奖励
 */
int32_t LotteryManager_TryGetLottery(LotteryManager* manager, int32_t playScore, int32_t* id, int32_t* val) 
{
    if (manager == NULL || id == NULL || val == NULL) return 0;
    if (manager->mFrozenTime > 0) return 0;

    int32_t res = 0;
    int32_t fRate = 0.0f;
    int32_t ShowLottery[3] = { 0 };
    int32_t LotteryPool[3] = { 0 };
    int32_t LotteryThresh[3] = { 0 };

	// 读取所有彩票的当前状态
	for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
	{
		ShowLottery[i] = Lottery_GetShowValue(&manager->mLotterys[i]);
		LotteryPool[i] = Lottery_GetLotteryPool(&manager->mLotterys[i]);
		LotteryThresh[i] = Lottery_GetThresh(&manager->mLotterys[i]);
	}

    // 尝试获取指定ID的彩票奖励
    if (*id > 0) 
    {
        (*id)--; // 转换为数组索引
        if (*id >= 0 && *id < GAME_Local_JP_MAX)
        {
            res = Lottery_TryGet(&manager->mLotterys[*id], playScore, val, &fRate);
            if (res) {
                manager->mFrozenTime += 60;
            }
        }
    }
    else 
    {
        // 遍历所有彩票尝试获取
        for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i) 
        {

			res = Lottery_TryGet(&manager->mLotterys[i], playScore, val, &fRate);
			if (res)
			{
				*id = i;
				manager->mFrozenTime += 60;
				break;
			}
        }
    }

    return res;
}

/**
 * @brief 检查是否可获取彩票奖励
 */
int32_t LotteryManager_CheckGetLottery(LotteryManager* manager, int32_t playScore) 
{
    if (manager == NULL) return 0;
    if (manager->mFrozenTime > 0) return 0;

    for (int32_t i = 0; i < 3; ++i) 
    {
		if (Lottery_CheckGet(&manager->mLotterys[i], playScore)) 
        {
			return i + 1; // 返回1-based ID
		}
    }
    return 0;
}

/**
 * @brief 帧更新逻辑
 */
void LotteryManager_Update(LotteryManager* manager, int32_t deltaTime) 
{
    if (manager == NULL) return;

#ifdef _DebugControlMode
    //deltaTime *= _gc.debugParam.nLotterAccumScale * 100;
#endif

    if (manager->mFrozenTime > 0) 
    {
        manager->mFrozenTime -= deltaTime;
    }
}