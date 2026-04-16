#include "LotteryPersistManager.h"
#include "Test.h"

#include "idea_qs.h"
#include <string.h>

LotteryPersistManager g_LotteryPersistBlob = { 0 };

/**
 * @brief 重置持久化镜像。
 * @details 清空全部字段，并写入固定 magic/version 头用于后续校验。
 */
void lottery_persist_reset_blob(void)
{
    memset(&g_LotteryPersistBlob, 0, sizeof(g_LotteryPersistBlob));
    g_LotteryPersistBlob.blobMagic = LOTTERY_PERSIST_BLOB_MAGIC;
    g_LotteryPersistBlob.version = LOTTERY_PERSIST_VERSION;
}

/**
 * @brief 校验当前持久化镜像是否有效。
 * @return 1 表示有效，0 表示无效。
 * @details 仅当 magic 和 version 同时匹配时，才允许执行恢复。
 */
int8_t lottery_persist_is_blob_valid(void)
{
    return (g_LotteryPersistBlob.blobMagic == LOTTERY_PERSIST_BLOB_MAGIC &&
        g_LotteryPersistBlob.version == LOTTERY_PERSIST_VERSION) ? 1 : 0;
}

/**
 * @brief 判断当前奖池管理器是否处于“有效运行”状态。
 * @param manager 奖池管理器实例。
 * @return 1 表示存在可持久化的运行数据，0 表示无有效数据。
 * @details 只要任一累计字段大于 0 即视为 active。
 */
int8_t lottery_persist_is_active(const LotteryManager* manager)
{
    if (manager == NULL) return 0;
    return (manager->mTotalPlayTime > 0 || manager->mTotalPlay > 0 || manager->mTotalDraw > 0) ? 1 : 0;
}

/**
 * @brief 将运行时 LotteryManager 数据打包到持久化镜像。
 * @param blob 目标持久化镜像。
 * @param manager 源运行时管理器。
 * @details 同步全局统计字段与各奖位字段，并刷新 magic/version。
 */
void lottery_persist_pack(LotteryPersistManager* blob, const LotteryManager* manager)
{
    int32_t i = 0;
    if (blob == NULL || manager == NULL) return;

    blob->blobMagic = LOTTERY_PERSIST_BLOB_MAGIC;
    blob->version = LOTTERY_PERSIST_VERSION;
    blob->mTotalPlayTime = manager->mTotalPlayTime;
    blob->mTotalPlay = manager->mTotalPlay;
    blob->mTotalDraw = manager->mTotalDraw;
    blob->mFrozenTime = manager->mFrozenTime;
    blob->mPlaySinceLastWin = manager->mPlaySinceLastWin;

    for (i = 0; i < GAME_Local_JP_MAX; ++i)
    {
        const Lottery* src = &manager->mLotterys[i];
        LotteryPersistPool_t* dst = &blob->pools[i];
        dst->mShowLottery = src->mShowLottery;
        dst->mLotteryPool = src->mLotteryPool;
        dst->mNextGiveLotteryThresh = src->mNextGiveLotteryThresh;
        dst->mTotalAccumPool = src->mTotalAccumPool;
        dst->mTotalGivePool = src->mTotalGivePool;
        dst->mTotalGiveTime = src->mTotalGiveTime;
        dst->mAccumDrawVal = src->mAccumDrawVal;
        dst->mAccumScore = src->mAccumScore;
    }
}

/**
 * @brief 将持久化镜像数据恢复到运行时 LotteryManager。
 * @param manager 目标运行时管理器。
 * @param blob 源持久化镜像。
 * @details 对可疑值做保护，包含展示值、阈值及累计/派奖范围校正。
 */
void lottery_persist_unpack(LotteryManager* manager, const LotteryPersistManager* blob)
{
    int32_t i = 0;
    if (manager == NULL || blob == NULL) return;

    manager->mTotalPlayTime = (blob->mTotalPlayTime < 0) ? 0 : blob->mTotalPlayTime;
    manager->mTotalPlay = (blob->mTotalPlay < 0) ? 0 : blob->mTotalPlay;
    manager->mTotalDraw = (blob->mTotalDraw < 0) ? 0 : blob->mTotalDraw;
    manager->mFrozenTime = (blob->mFrozenTime < 0) ? 0 : blob->mFrozenTime;
    manager->mPlaySinceLastWin = (blob->mPlaySinceLastWin < 0) ? 0 : blob->mPlaySinceLastWin;

    for (i = 0; i < GAME_Local_JP_MAX; ++i)
    {
        const LotteryPersistPool_t* src = &blob->pools[i];
        Lottery* dst = &manager->mLotterys[i];
        const int32_t baseVal = dst->mBaseLottery;
        const int32_t maxVal = (dst->mMaxLottery >= baseVal) ? dst->mMaxLottery : baseVal;

        dst->mShowLottery = LotteryPersist_ClampInt32(src->mShowLottery, baseVal, maxVal);
        dst->mLotteryPool = (src->mLotteryPool < 0) ? 0 : src->mLotteryPool;
        dst->mNextGiveLotteryThresh = LotteryPersist_ClampInt32(src->mNextGiveLotteryThresh, baseVal, maxVal);
        dst->mTotalAccumPool = (src->mTotalAccumPool < 0) ? 0 : src->mTotalAccumPool;
        dst->mTotalGivePool = (src->mTotalGivePool < 0) ? 0 : src->mTotalGivePool;
        dst->mTotalGiveTime = src->mTotalGiveTime;
        dst->mAccumDrawVal = (src->mAccumDrawVal < 0) ? 0 : src->mAccumDrawVal;
        dst->mAccumScore = (src->mAccumScore < 0) ? 0 : src->mAccumScore;
    }
}

/**
 * @brief 初始化奖池持久化模块。
 * @details 连接底层数据库镜像，校验失败时重置为默认有效头并记录日志。
 */
void LotteryPersist_Init(void)
{
    (void)idea_database_connect(&g_LotteryPersistBlob, sizeof(g_LotteryPersistBlob));
    if (!lottery_persist_is_blob_valid())
    {
        lottery_persist_reset_blob();
        QS_LOG("\r\n [JP] persist blob init/reinit version=%u", (unsigned)LOTTERY_PERSIST_VERSION);
    }
    else
    {
        QS_LOG("\r\n [GM] persist blob init: flag=%u (expect=%u), keep persisted debug data", g_LotteryPersistBlob.blobMagic, LOTTERY_PERSIST_VERSION);
        QS_LOG("\r\n TotalPlay : %d", g_LotteryPersistBlob.mTotalPlay);
        QS_LOG("\r\n TotalPlayTime : %d", g_LotteryPersistBlob.mTotalPlayTime);
        QS_LOG("\r\n JT_Major :ShowLottery = %d  LotteryPool = %d", g_LotteryPersistBlob.pools[0].mShowLottery, g_LotteryPersistBlob.pools[0].mLotteryPool);
        QS_LOG("\r\n JT_Minor :ShowLottery = %d  LotteryPool = %d", g_LotteryPersistBlob.pools[1].mShowLottery, g_LotteryPersistBlob.pools[1].mLotteryPool);
        QS_LOG("\r\n JT_Mini :ShowLottery = %d  LotteryPool = %d", g_LotteryPersistBlob.pools[2].mShowLottery, g_LotteryPersistBlob.pools[2].mLotteryPool);
    }
}

/**
 * @brief 保存当前奖池状态到持久化镜像。
 * @param manager 当前运行时奖池管理器。
 * @details
 * 1) 校验镜像头，非法时重置；
 * 2) 若当前无有效运行数据则清空持久化；
 * 3) 否则执行数据打包。
 */
void LotteryPersist_Save(LotteryManager* manager)
{
    if (manager == NULL) return;
    if (!lottery_persist_is_blob_valid())
    {
        lottery_persist_reset_blob();
    }

    if (!lottery_persist_is_active(manager))
    {
        LotteryPersist_Clear();
        return;
    }

    lottery_persist_pack(&g_LotteryPersistBlob, manager);
}

/**
 * @brief 清空奖池持久化数据。
 * @details 通过重置镜像恢复默认头与空数据。
 */
void LotteryPersist_Clear(void)
{
    lottery_persist_reset_blob();
}

/**
 * @brief 尝试从持久化镜像恢复运行时状态。
 * @param manager 目标运行时奖池管理器。
 * @details 仅当镜像有效且校验通过时，才执行恢复。
 */
void LotteryPersist_TryRestore(LotteryManager* manager)
{
#ifdef _LocalDebug
    return;
#else
    if (manager == NULL) return;
    if (!lottery_persist_is_blob_valid()) return;
    lottery_persist_unpack(manager, &g_LotteryPersistBlob);
#endif
}

/**
 * @brief 将 int32 值限制在指定区间内。
 * @param v 待处理值。
 * @param lo 下界（最小值）。
 * @param hi 上界（最大值）。
 * @return 限制后的结果值。
 */
int32_t LotteryPersist_ClampInt32(int32_t v, int32_t lo, int32_t hi)
{
    if (v < lo) return lo;
    if (v > hi) return hi;
    return v;
}
