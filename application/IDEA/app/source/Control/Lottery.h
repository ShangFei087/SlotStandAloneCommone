#ifndef _Lottery_H_
#define _Lottery_H_

#include <stdint.h> 

// 彩金池数据结构
typedef struct Lottery {
	int32_t mBaseLottery;        // 基础彩金值
	int32_t mShowLottery;        // 当前显示彩金值
	int32_t mLotteryPool;        // 当前实际彩金池值
	int32_t mNextGiveLotteryThresh; // 下一次开奖阈值

	int32_t mTotalAccumPool;     // 累计总注入值
	int32_t mTotalGivePool;      // 累计已派发值
	uint16_t mTotalGiveTime;     // 累计开奖次数
	uint16_t mMaxLottery;        // 彩金上限值

	int32_t mThreshMax;          // 开奖阈值上限
	int32_t mThreshMin;          // 开奖阈值下限
	int32_t mDrawRate;           // 抽取率（万分比）

	int32_t scale;             // 彩金缩放比例
	int32_t mAccumDrawVal;     // 当前累计可抽取值
	int32_t mAccumScore;       // 累计分值
	int32_t mShowSpeedPermil;  // 显示增长速度（千分比）
} Lottery;

// 初始化彩金池实例
void Lottery_Init(Lottery* self, int32_t threshMax, int32_t threshMin, int32_t drawRate, int32_t scale);

void Lottery_OnPlay(Lottery* self, int32_t score);
int32_t Lottery_TryGet(Lottery* self, int32_t playScore, int32_t* val, int32_t* rate); // 返回 0/1
int32_t Lottery_CheckGet(Lottery* self, int32_t playScore); // 返回 0/1

int32_t Lottery_GetShowValue(Lottery* self);
int32_t Lottery_GetLotteryPool(Lottery* self);
int32_t Lottery_GetThresh(Lottery* self);
int32_t Lottery_GetTotalGiveTime(Lottery* self);
int32_t Lottery_GetTotalGiveVal(Lottery* self);

int32_t Lottery_GetBaseValue(Lottery* self);
void Lottery_SetBaseValue(Lottery* self, int32_t value, int32_t maxValue);
int32_t Lottery_GetMaxValue(Lottery* self);


#endif // _Lottery_H_
