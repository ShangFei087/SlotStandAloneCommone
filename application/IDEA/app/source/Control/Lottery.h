#ifndef _Lottery_H_
#define _Lottery_H_

#include <stdint.h> // 替代byte，用uint8_t

// Lottery
typedef struct Lottery {
	int32_t mBaseLottery;//基础彩票值
	int32_t mShowLottery;//显示用彩票值
	int32_t mLotteryPool;//彩票奖池当前值
	int32_t mNextGiveLotteryThresh;//下次开奖阈值

	int32_t mTotalAccumPool;//累计总奖池值
	int32_t mTotalGivePool;//累计发放奖励值
	uint16_t mTotalGiveTime;//累计开奖次数
	uint16_t mMaxLottery;//彩票最大值 / 上限

	int32_t mThreshMax;//开奖阈值最大值
	int32_t mThreshMin;//开奖阈值最小值
	int32_t mDrawRate;//抽取率

	int64_t mDbShowLottery;//高精度显示用值
	int64_t mDbLotteryPool;//高精度奖池值
	int64_t mDbTotalAccumPool;//高精度累计总奖池

	int32_t scale; //放大奖池的倍数
	int32_t mAccumDrawVal;    // 累积的抽取值
	int32_t mAccumScore;      // 累积的score
} Lottery;

// 函数声明（替代类成员函数）
void Lottery_Init(Lottery* self, int32_t threshMax, int32_t threshMin, int32_t drawRate, int32_t scale);

void Lottery_OnPlay(Lottery* self, int32_t score);
int32_t Lottery_TryGet(Lottery* self, int32_t playScore, int32_t* val, int32_t* rate); // 返回0/1替代bool
int32_t Lottery_CheckGet(Lottery* self, int32_t playScore); // 返回0/1替代bool

int32_t Lottery_GetShowValue(Lottery* self);
int32_t Lottery_GetLotteryPool(Lottery* self);
int32_t Lottery_GetThresh(Lottery* self);
int32_t Lottery_GetTotalGiveTime(Lottery* self);
int32_t Lottery_GetTotalGiveVal(Lottery* self);

int32_t Lottery_GetBaseValue(Lottery* self);
void Lottery_SetBaseValue(Lottery* self, int32_t value, int32_t maxValue);
void Lottery_SetShowValue(Lottery* self, int32_t showValue);
int32_t Lottery_GetMaxValue(Lottery* self);


#endif // _Lottery_H_
