#include "FreeGamePersistManager.h"
#include "Test.h"

#include "idea_qs.h"
#include <string.h>

FreeGamePersistManager g_FreeGamePersistManager = {0};

void freegame_persist_reset_blob(void)
{
    memset(&g_FreeGamePersistManager, 0, sizeof(g_FreeGamePersistManager));
    g_FreeGamePersistManager.blobMagic = FREE_GAME_PERSIST_BLOB_MAGIC;
    g_FreeGamePersistManager.version = FREE_GAME_PERSIST_VERSION;
}

int8_t freegame_persist_is_active(const FreeGameInfo_t* f)
{
    if (!f) return 0;
    return (f->nTotalFreeTime > 0 && f->nCurFreeIdx < f->nTotalFreeTime) ? 1 : 0;
}

int8_t freegame_persist_slot_is_active(const FreeGamePersistSlot_t* s)
{
    if (!s) return 0;
    return (s->nTotalFreeTime > 0 && s->nCurFreeIdx < s->nTotalFreeTime) ? 1 : 0;
}

void freegame_persist_slot_pack(FreeGamePersistSlot_t* s, const GameInstance_t* inst)
{
    const RoundInfo_t* ri = &inst->freeGameInfo.roundInfo;

    s->slotMagic = FREE_GAME_PERSIST_SLOT_MAGIC;
    s->gameId = inst->id;
    s->nBetVal = inst->freeGameInfo.nBetVal;
    s->nCurFreeIdx = inst->freeGameInfo.nCurFreeIdx;
    s->nTotalFreeTime = inst->freeGameInfo.nTotalFreeTime;
    s->nRemainFreeBet = inst->freeGameInfo.nRemainFreeBet;
    s->nFreeTotalWin = inst->freeGameInfo.nFreeTotalWin;
    memcpy(s->pFreeMxu, ri->pFreeMxu, sizeof(s->pFreeMxu));
    memcpy(s->FreeBetArray, ri->FreeBetArray, sizeof(s->FreeBetArray));
    memcpy(s->FreeIDVec, ri->FreeIDVec, sizeof(s->FreeIDVec));
    s->nFreeBet = ri->nFreeBet;
    memcpy(s->WildPosArray, ri->WildPosArray, sizeof(s->WildPosArray));
}

void freegame_persist_slot_unpack(GameInstance_t* inst, const FreeGamePersistSlot_t* s)
{
    RoundInfo_t_Init(&inst->freeGameInfo.roundInfo);
    RoundInfo_t* ri = &inst->freeGameInfo.roundInfo;

    inst->freeGameInfo.nBetVal = s->nBetVal;
    inst->freeGameInfo.nCurFreeIdx = s->nCurFreeIdx;
    inst->freeGameInfo.nTotalFreeTime = s->nTotalFreeTime;
    inst->freeGameInfo.nRemainFreeBet = s->nRemainFreeBet;
    inst->freeGameInfo.nFreeTotalWin = s->nFreeTotalWin;
    memcpy(ri->pFreeMxu, s->pFreeMxu, sizeof(s->pFreeMxu));
    memcpy(ri->FreeBetArray, s->FreeBetArray, sizeof(s->FreeBetArray));
    memcpy(ri->FreeIDVec, s->FreeIDVec, sizeof(s->FreeIDVec));
    ri->nFreeBet = s->nFreeBet;
    memcpy(ri->WildPosArray, s->WildPosArray, sizeof(s->WildPosArray));
    ri->pCurCri = NULL;
}

int32_t freegame_persist_find_slot_for_game(GameInstanceId_t gameId)
{
    for (int32_t i = 0; i < GAME_INSTANCE_ID_MAX; i++) {
        if (g_FreeGamePersistManager.slots[i].slotMagic == FREE_GAME_PERSIST_SLOT_MAGIC &&
            g_FreeGamePersistManager.slots[i].gameId == gameId) {
            return i;
        }
    }
    return -1;
}

int32_t freegame_persist_find_empty_slot(void)
{
    for (int32_t i = 0; i < GAME_INSTANCE_ID_MAX; i++) {
        if (g_FreeGamePersistManager.slots[i].slotMagic != FREE_GAME_PERSIST_SLOT_MAGIC) 
        {
            return i;
        }
    }
    return -1;
}

void FreeGamePersist_Init(void)
{
    (void)idea_database_connect(&g_FreeGamePersistManager, sizeof(g_FreeGamePersistManager));
    if (g_FreeGamePersistManager.blobMagic != FREE_GAME_PERSIST_BLOB_MAGIC ||
        g_FreeGamePersistManager.version != FREE_GAME_PERSIST_VERSION) {
        freegame_persist_reset_blob();
        QS_LOG("\r\n [GM] FreeGame persist blob init/reinit version=%u", (unsigned)FREE_GAME_PERSIST_VERSION);
    }
    else
    {
        QS_LOG("\r\n [GM]FreeGame persist blob init: flag=%u (expect=%u), keep persisted debug data", g_FreeGamePersistManager.blobMagic, FREE_GAME_PERSIST_VERSION);
        QS_LOG("\r\n gameId : %d", g_FreeGamePersistManager.slots[0].gameId);
        QS_LOG("\r\n nBetVal : %d", g_FreeGamePersistManager.slots[0].nBetVal);
        QS_LOG("\r\n nCurFreeIdx : %d", g_FreeGamePersistManager.slots[0].nCurFreeIdx);
        QS_LOG("\r\n nRemainFreeBet : %d", g_FreeGamePersistManager.slots[0].nTotalFreeTime);
        QS_LOG("\r\n nFreeBet : %d", g_FreeGamePersistManager.slots[0].nFreeBet);
    }
}

void FreeGamePersist_ClearByGameId(GameInstanceId_t gameId)
{
    if (gameId == GAME_ID_INVALID) return;
    int32_t idx = freegame_persist_find_slot_for_game(gameId);
    if (idx >= 0) {
        memset(&g_FreeGamePersistManager.slots[idx], 0, sizeof(FreeGamePersistSlot_t));
    }
}

void FreeGamePersist_Save(GameInstance_t* inst)
{
    if (!inst || inst->id == GAME_ID_INVALID) return;

    if (!freegame_persist_is_active(&inst->freeGameInfo)) {
        FreeGamePersist_ClearByGameId(inst->id);
        return;
    }

    int32_t idx = freegame_persist_find_slot_for_game(inst->id);
    if (idx < 0) {
        idx = freegame_persist_find_empty_slot();
    }
    if (idx < 0) {
        QS_LOG("\r\n [GM] FreeGame persist: no empty slot for gameId=%u", (unsigned)inst->id);
        return;
    }

    FreeGamePersistSlot_t* s = &g_FreeGamePersistManager.slots[idx];
    memset(s, 0, sizeof(FreeGamePersistSlot_t));
    freegame_persist_slot_pack(s, inst);
}

void FreeGamePersist_TryRestore(GameInstance_t* inst)
{
#ifdef _LocalDebug
    (void)inst;
    return;
#else
    if (!inst || inst->id == GAME_ID_INVALID) return;

    int32_t idx = freegame_persist_find_slot_for_game(inst->id);
    if (idx < 0) return;

    FreeGamePersistSlot_t* s = &g_FreeGamePersistManager.slots[idx];
    if (s->slotMagic != FREE_GAME_PERSIST_SLOT_MAGIC) return;
    if (!freegame_persist_slot_is_active(s)) {
        memset(s, 0, sizeof(FreeGamePersistSlot_t));
        return;
    }

    freegame_persist_slot_unpack(inst, s);
#endif
}
