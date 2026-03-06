#ifndef _LotteryManager_H_
#define _LotteryManager_H_

#include "Lottery.h"
#include "ComputerData.h"

// 重构为 C 风格结构体
typedef struct LotteryManager {
	int32_t mTotalPlayTime;   // 总游玩次数
	int32_t mTotalPlay;       // 总游玩数值
	int32_t mTotalDraw;         // 总抽取数值
	Lottery mLotterys[GAME_Local_JP_MAX];     // 彩票实例数组
	int32_t mFrozenTime;        // 冻结时间
	int32_t mScale;				//放大奖池倍数
} LotteryManager;


/**
 * @brief 初始化 LotteryManager 实例
 * @param manager 指向 LotteryManager 实例的指针
 */
void LotteryManager_Init(void);

/**
 * @brief 销毁 LotteryManager 实例（释放资源）
 * @param manager 指向 LotteryManager 实例的指针
 */
void LotteryManager_Destroy(LotteryManager* manager);

/**
 * @brief 处理游玩逻辑
 * @param manager 指向 LotteryManager 实例的指针
 * @param value 游玩分数
 */
void LotteryManager_OnPlay(LotteryManager* manager, int32_t value);

/**
 * @brief 尝试获取彩票奖励
 * @param manager 指向 LotteryManager 实例的指针
 * @param playScore 游玩分数
 * @param id 输出参数，彩票ID
 * @param val 输出参数，奖励数值
 * @return 成功返回1，失败返回0
 */
int32_t LotteryManager_TryGetLottery(LotteryManager* manager, int32_t playScore, int32_t* id, int32_t* val);

/**
 * @brief 检查是否可获取彩票奖励
 * @param manager 指向 LotteryManager 实例的指针
 * @param playScore 游玩分数
 * @return 可获取返回彩票ID（1-3），不可获取返回0
 */
int32_t LotteryManager_CheckGetLottery(LotteryManager* manager, int32_t playScore);

/**
 * @brief 获取彩票显示值
 * @param manager 指向 LotteryManager 实例的指针
 * @param id 彩票ID
 * @return 彩票显示值
 */
int32_t LotteryManager_GetLotteryShowValue(LotteryManager* manager, int32_t id);

/**
 * @brief 更新逻辑（帧更新）
 * @param manager 指向 LotteryManager 实例的指针
 * @param deltaTime 帧间隔时间
 */
void LotteryManager_Update(LotteryManager* manager, int32_t deltaTime);

/**
 * @brief 设置彩票基础值和最大值
 * @param manager 指向 LotteryManager 实例的指针
 * @param baseValue 3个彩票的基础值数组
 * @param maxValue 3个彩票的最大值数组
 */
void LotteryManager_SetBaseValue(LotteryManager* manager, int32_t baseValue[3], int32_t maxValue[3]);

/**
 * @brief 获取彩票基础值和最大值
 * @param manager 指向 LotteryManager 实例的指针
 * @param baseValue 输出参数，3个彩票的基础值数组
 * @param maxValue 输出参数，3个彩票的最大值数组
 */
void LotteryManager_GetBaseValue(LotteryManager* manager, int32_t baseValue[3], int32_t maxValue[3]);

/**
 * @brief 设置彩票显示值
 * @param manager 指向 LotteryManager 实例的指针
 * @param id 彩票ID
 * @param showValue 显示值
 */
void LotteryManager_SetShowValue(LotteryManager* manager, int32_t id, int32_t showValue);
extern  LotteryManager gLotteryManager;
#endif // _LotteryManager_H_