#include "GameManager.h"
#include "idea_qs.h"
#include "Test.h"
#include <string.h>

GameManager_t g_GameManager = {0};

GameInstance_t* find_slot(void)
{
    for (int8_t i = 0; i < GAME_INSTANCE_ID_MAX; i++) 
    {
        if (!g_GameManager.instances[i].isInitialized)
        {
            return &g_GameManager.instances[i];
        }
    }
    return NULL;
}

int8_t GameManager_Init(void)
{
    memset(&g_GameManager, 0, sizeof(g_GameManager));
    g_GameManager.currentGameId = GAME_ID_INVALID;

    // 持久化 DebugInfo 数据；本地调试模式下关闭。
#ifndef _LocalDebug
    (void)idea_database_connect(&g_GameManager.debugInfo, sizeof(g_GameManager.debugInfo));
    if (g_GameManager.debugInfo.dwFlag != GAMEMANAGER_DEBUGINFO_FLAG_MAGIC)
    {
        DebugInfo_reset(&g_GameManager.debugInfo);
        g_GameManager.debugInfo.dwFlag = GAMEMANAGER_DEBUGINFO_FLAG_MAGIC;
        QS_LOG("\r\n [GM] First set done: flag=%u", g_GameManager.debugInfo.dwFlag);
    }
    else
    {
        QS_LOG("\r\n [GM] Reboot compare pass: flag=%u (expect=%u), keep persisted debug data", g_GameManager.debugInfo.dwFlag,GAMEMANAGER_DEBUGINFO_FLAG_MAGIC);
    }
#else
    DebugInfo_reset(&g_GameManager.debugInfo);
    g_GameManager.debugInfo.dwFlag = GAMEMANAGER_DEBUGINFO_FLAG_MAGIC;
    QS_LOG("\r\n [GM] _LocalDebug enabled: skip debugInfo persist");
#endif
    //QS_LOG("\r\n [GM] Init done flag=%u play=%u win=%u total=%u",g_GameManager.debugInfo.dwFlag,g_GameManager.debugInfo.dwPlayScore,g_GameManager.debugInfo.dwWinScore,g_GameManager.debugInfo.dwTotalPlayTime);

    return 1;
}

void GameManager_CleanupInstance(GameInstance_t* instance)
{
    if (!instance || !instance->isInitialized) return;

    memset(instance->name, 0, sizeof(instance->name));
    instance->isInitialized = 0;
    instance->isActive = 0;
    instance->id = GAME_ID_INVALID;
}

void GameManager_Cleanup(void)
{
    for (int8_t i = 0; i < GAME_INSTANCE_ID_MAX; i++)
    {
        if (g_GameManager.instances[i].isInitialized)
        {
            GameManager_CleanupInstance(&g_GameManager.instances[i]);
        }
    }
    memset(&g_GameManager, 0, sizeof(g_GameManager));
}

int8_t GameManager_InitInstance(GameInstance_t* instance)
{
    if (!instance || instance->isInitialized) return 0;

    instance->isInitialized = 1;
    instance->isActive = 0;

    GameConfig_Init(&instance->gameConfig);
    FreeGameInfo_t_Init(&instance->freeGameInfo);
    return 1;
}

GameInstanceId_t GameManager_RegisterGame(const int8_t* gameName, GameInstanceId_t gameId)
{
    
    if (!gameName) return GAME_ID_INVALID;

    GameInstanceId_t existing = GameManager_FindGameByName(gameName);
    if (existing != GAME_ID_INVALID) return existing;

    GameInstance_t* slot = find_slot();
    if (!slot) return GAME_ID_INVALID;

    GameInstanceId_t newId = gameId;

    memset(slot, 0, sizeof(GameInstance_t));
    slot->id = gameId;
    memcpy(slot->name, gameName, 64);

    if (!GameManager_InitInstance(slot))
    {
        memset(slot, 0, sizeof(GameInstance_t));
        return GAME_ID_INVALID;
    }

    g_GameManager.instanceCount++;

    if (g_GameManager.currentGameId == GAME_ID_INVALID) {
        GameManager_SwitchGame(newId);
    }

    /* InitInstance 已清空 freeGameInfo，此处用 Flash 里同 gameId 的进度覆盖（支持断电续免费） */
    FreeGamePersist_TryRestore(slot);

    return newId;
}

int8_t GameManager_UnregisterGame(GameInstanceId_t gameId)
{
    GameInstance_t* instance = GameManager_GetInstance(gameId);
    if (!instance) return 0;

    /* 游戏卸载时不再保留该路的免费断电恢复数据 */
    FreeGamePersist_ClearByGameId(gameId);

    if (g_GameManager.currentGameId == gameId) {
        g_GameManager.currentGameId = GAME_ID_INVALID;
    }

    GameManager_CleanupInstance(instance);
    g_GameManager.instanceCount--;
    return 1;
}

int8_t GameManager_SwitchGame(GameInstanceId_t gameId)
{
    GameInstance_t* instance = GameManager_GetInstance(gameId);
    if (!instance) return 0;

    if (g_GameManager.currentGameId != GAME_ID_INVALID) {
        GameInstance_t* current = GameManager_GetInstance(g_GameManager.currentGameId);
        if (current) current->isActive = 0;
    }

    instance->isActive = 1;
    g_GameManager.currentGameId = gameId;
    return 1;
}

GameInstanceId_t GameManager_GetCurrentGameId(void)
{
    return g_GameManager.currentGameId;
}

GameInstance_t* GameManager_GetInstance(GameInstanceId_t gameId)
{
    if (gameId == GAME_ID_INVALID) return NULL;
    for (uint32_t i = 0; i < GAME_INSTANCE_ID_MAX; i++) {
        if (g_GameManager.instances[i].isInitialized ==1) 
        {
            if (g_GameManager.instances[i].id == gameId)
            {
                return &g_GameManager.instances[i];
            }
        }
    }
    return NULL;
}

GameInstance_t* GameManager_GetCurrentInstance(void)
{
    return GameManager_GetInstance(g_GameManager.currentGameId);
}

int8_t GameManager_GetInstanceCount(void)
{
    return g_GameManager.instanceCount;
}

GameInstanceId_t GameManager_FindGameByName(const int8_t* gameName)
{
    if (!gameName) return GAME_ID_INVALID;
    for (uint32_t i = 0; i < GAME_INSTANCE_ID_MAX; i++) {
        if (g_GameManager.instances[i].isInitialized &&
            strcmp((const char*)g_GameManager.instances[i].name, (const char*)gameName) == 0) {
            return g_GameManager.instances[i].id;
        }
    }
    return GAME_ID_INVALID;
}
