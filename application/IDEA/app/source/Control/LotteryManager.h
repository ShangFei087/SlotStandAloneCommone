#ifndef _LotteryManager_H_
#define _LotteryManager_H_

#include "Lottery.h"
#include "ComputerData.h"

// 彩金管理器运行时数据
typedef struct LotteryManager {
	int32_t mTotalPlayTime;   // 总游玩局数
	int32_t mTotalPlay;       // 总游玩分值
	int32_t mTotalDraw;       // 总抽奖分值
	Lottery mLotterys[GAME_Local_JP_MAX];   // 彩金池实例数组
	int32_t mFrozenTime;      // 开奖冻结时间
	int32_t mScale;           // 彩金缩放系数
	int32_t mInjectRatePermil;              // 基础注入倍率（千分比）
	int32_t mTierMidBet;                    // 中档下注阈值
	int32_t mTierHighBet;                   // 高档下注阈值
	int32_t mTierMidInjectPermil;           // 中档注入倍率（千分比）
	int32_t mTierHighInjectPermil;          // 高档注入倍率（千分比）
	int32_t mWinFreezeTime;                 // 命中彩金后的冷却时间
	int32_t mMinPlayScoreToTrigger;         // 触发开奖检查的最小下注
	int32_t mMinPlayGapAfterWin;            // 两次开奖之间最小间隔局数
	int32_t mPlaySinceLastWin;              // 距离上次开奖的局数
} LotteryManager;

/**
 * @brief 初始化彩金管理器全局实例。
 */
void LotteryManager_Init(void);

/**
 * @brief 重置彩金管理器运行时数据。
 */
void LotteryManager_Destroy(LotteryManager* manager);

/**
 * @brief 处理一次下注并更新所有彩金池。
 */
void LotteryManager_OnPlay(LotteryManager* manager, int32_t value);

/**
 * @brief 尝试触发一次彩金开奖。
 * @return 成功返回 1，未触发返回 0。
 */
int32_t LotteryManager_TryGetLottery(LotteryManager* manager, int32_t playScore, int32_t* id, int32_t* val);

/**
 * @brief 尝试触发多个彩金开奖。
 * @param ids 命中的彩金下标数组（0~GAME_Local_JP_MAX-1）。
 * @param vals 命中的彩金金额数组。
 * @param maxCount 输出数组容量。
 * @return 命中的彩金个数。
 */
int32_t LotteryManager_TryGetLotterys(LotteryManager* manager, int32_t playScore, int32_t* ids, int32_t* vals, int32_t maxCount);

/**
 * @brief 候选彩金放行后提交到奖池账目（id 支持 0~2 下标或 1~3 类型值）。
 */
void LotteryManager_CommitLottery(LotteryManager* manager, int32_t id, int32_t val);

/**
 * @brief 将金额回补到指定彩金池（id 支持 0~2 下标或 1~3 类型值）。
 */
void LotteryManager_RefundLottery(LotteryManager* manager, int32_t id, int32_t val);

/**
 * @brief 设置所有彩金池的基础值和上限。
 */
void LotteryManager_SetBaseValue(LotteryManager* manager, int32_t baseValue[3], int32_t maxValue[3]);

static int32_t LotteryManager_GetTierPermil(const LotteryManager* manager, int32_t score);

extern  LotteryManager gLotteryManager;
#endif // _LotteryManager_H_