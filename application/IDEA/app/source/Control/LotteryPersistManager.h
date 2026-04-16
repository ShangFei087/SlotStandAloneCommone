#ifndef _LOTTERY_PERSIST_MANAGER_H_
#define _LOTTERY_PERSIST_MANAGER_H_

#include "LotteryManager.h"

/* Jackpot 持久化镜像
 * 通过 magic/version 校验镜像兼容性，确保恢复数据可用。 */
#define LOTTERY_PERSIST_BLOB_MAGIC        ((uint32_t)0x4A505042u) /* 'JPPB' */
#define LOTTERY_PERSIST_VERSION           ((uint32_t)1u)

typedef struct
{
    int32_t mShowLottery;//当前展示奖池值
    int32_t mLotteryPool;//当前累计奖池
    int32_t mNextGiveLotteryThresh;//下一次派奖阈值
    int32_t mTotalAccumPool;//历史累计入池值
    int32_t mTotalGivePool;//历史累计派奖值
    uint16_t mTotalGiveTime;//历史累计派奖次数
    int32_t mAccumDrawVal;//累计中奖值
    int32_t mAccumScore;//累计下注值
} LotteryPersistPool_t;

typedef struct
{
    uint32_t blobMagic;//数据魔数
    uint32_t version;//数据版本号
    int32_t mTotalPlayTime;//累计游玩次数
    int32_t mTotalPlay;//累计下注值
    int32_t mTotalDraw;//累计中奖值
    int32_t mFrozenTime;//冻结轮数（>0 时不可派奖）
    int32_t mPlaySinceLastWin;//距离上次中奖的局数
    LotteryPersistPool_t pools[GAME_Local_JP_MAX];//各奖位奖池持久化数据
} LotteryPersistManager;

extern LotteryPersistManager g_LotteryPersistBlob;

void lottery_persist_reset_blob(void);
int8_t lottery_persist_is_blob_valid(void);
int8_t lottery_persist_is_active(const LotteryManager* manager);
void lottery_persist_pack(LotteryPersistManager* blob, const LotteryManager* manager);
void lottery_persist_unpack(LotteryManager* manager, const LotteryPersistManager* blob);

void LotteryPersist_Init(void);
void LotteryPersist_Save(LotteryManager* manager);
void LotteryPersist_Clear(void);
void LotteryPersist_TryRestore(LotteryManager* manager);

int32_t LotteryPersist_ClampInt32(int32_t v, int32_t lo, int32_t hi);

#endif /* _LOTTERY_PERSIST_MANAGER_H_ */
