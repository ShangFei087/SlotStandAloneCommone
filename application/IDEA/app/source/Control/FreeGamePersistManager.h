#ifndef _FREE_GAME_PERSIST_MANAGER_H_
#define _FREE_GAME_PERSIST_MANAGER_H_

#include "ComputerData.h"
#include "CommonStruct.h"
#include "GameManager.h"

/* 多实例免费局持久化：每槽绑定一个 gameId，最多 GAME_INSTANCE_ID_MAX 路同时进行中的免费 */
#define FREE_GAME_PERSIST_BLOB_MAGIC   ((uint32_t)0x46525042u) /* 'FRPB' */
#define FREE_GAME_PERSIST_SLOT_MAGIC   ((uint32_t)0x4652534Cu) /* 'FRSL' */
#define FREE_GAME_PERSIST_VERSION      ((uint32_t)4u) /* 新增 FreeGameInfo_t.nFreeTotalWin 持久化字段 */

/* 单路游戏的免费快照：与 RAM 中 GameInstance_t.freeGameInfo 对应，按 gameId 查找 */
typedef struct {
    int8_t nCurFreeIdx;
    int8_t nTotalFreeTime;
    int8_t WildPosArray[GE_MaxFreeNum][GE_WheelChessMaxNum];
    Matrix_u pFreeMxu[GE_MaxFreeNum];
    uint16_t FreeIDVec[GE_MaxFreeNum][GE_MaxIDNum];
    GameInstanceId_t gameId; /* 所属游戏，多实例时各 gameId 各占一槽 */
    uint32_t slotMagic;       /* FREE_GAME_PERSIST_SLOT_MAGIC 表示本槽有效 */
    int32_t nRemainFreeBet;
    int32_t nBetVal;
    uint16_t nFreeTotalWin;
    uint32_t FreeBetArray[GE_MaxFreeNum];
    uint32_t nFreeBet;
} FreeGamePersistSlot_t;

/* 整块绑定 idea_database_connect，idea_database_exec 周期刷写到 Flash */
typedef struct {
    uint32_t blobMagic; /* 整包有效标记 */
    uint32_t version;   /* 结构变更时递增，不匹配则丢弃历史快照 */
    FreeGamePersistSlot_t slots[1];
} FreeGamePersistManager;

extern FreeGamePersistManager g_FreeGamePersistManager;

void freegame_persist_reset_blob(void);
int8_t freegame_persist_is_active(const FreeGameInfo_t* f);
int8_t freegame_persist_slot_is_active(const FreeGamePersistSlot_t* s);
void freegame_persist_slot_pack(FreeGamePersistSlot_t* s, const GameInstance_t* inst);
void freegame_persist_slot_unpack(GameInstance_t* inst, const FreeGamePersistSlot_t* s);
int32_t freegame_persist_find_slot_for_game(GameInstanceId_t gameId);
int32_t freegame_persist_find_empty_slot(void);

void FreeGamePersist_Init(void);
void FreeGamePersist_Save( GameInstance_t* inst);
void FreeGamePersist_ClearByGameId(GameInstanceId_t gameId);
void FreeGamePersist_TryRestore(GameInstance_t* inst);

#endif /* _FREE_GAME_PERSIST_MANAGER_H_ */
