#ifndef _GAME_MANAGER_H_
#define _GAME_MANAGER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "qs.h"
#include "NatureAlg.h"
#include "CommonStruct.h"
#include "GameConfig.h"
#include "ComputerData.h"


#define GAME_INSTANCE_ID_INVALID 0

/* 游戏实例结构 */
typedef struct {
    GameInstanceId_t id;
    int8_t name[64];
    int8_t isActive;
    int8_t isInitialized;

    FreeGameInfo_t freeGameInfo;
    DebugInfo debugInfo;
    SlotGameConfig_t gameConfig;
} GameInstance_t;

/* 游戏管理器结构 */
typedef struct {
    GameInstance_t instances[GAME_INSTANCE_ID_MAX];
    GameInstanceId_t currentGameId;
    int8_t instanceCount;
} GameManager_t;


int8_t GameManager_Init(void);//初始化管理器
void GameManager_Cleanup(void);//重置管理器

GameInstanceId_t GameManager_RegisterGame(const int8_t* gameName, GameInstanceId_t gameId); //注册游戏
int8_t GameManager_UnregisterGame(GameInstanceId_t gameId); //注销游戏
int8_t GameManager_SwitchGame(GameInstanceId_t gameId);//切换游戏
GameInstanceId_t GameManager_GetCurrentGameId(void);//当前游戏Id

GameInstance_t* GameManager_GetInstance(GameInstanceId_t gameId);//查找实例
GameInstance_t* GameManager_GetCurrentInstance(void);//当前游戏实例

int8_t GameManager_GetInstanceCount(void); //已经初始化游戏的数量
GameInstanceId_t GameManager_FindGameByName(const int8_t* gameName); //根据名称查找实例 ID

int8_t GameManager_InitInstance(GameInstance_t* instance);//初始化游戏实例
void GameManager_CleanupInstance(GameInstance_t* instance); //重置游戏实例


#endif /* _GAME_MANAGER_H_ */
