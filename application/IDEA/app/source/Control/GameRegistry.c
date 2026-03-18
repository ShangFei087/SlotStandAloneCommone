#include "GameRegistry.h"

#include "GameConfig.h"
#include "LotteryManager.h"
#include "ComputerData.h"

// 注册单个游戏并拷贝配置
static int8_t GameRegistry_RegisterGameWithConfig(const int8_t* gameName, GameId_t gameId, const SlotGameConfig_t* config)
{
    GameId_t registeredGameId = GAME_ID_INVALID;
    GameInstance_t* instance = NULL;

    if (gameName == NULL || config == NULL)
    {
        return 0;
    }

    registeredGameId = GameManager_RegisterGame(gameName, gameId);
    if (registeredGameId == GAME_ID_INVALID)
    {
        return 0;
    }

    instance = GameManager_GetInstance(registeredGameId);
    if (instance == NULL)
    {
        return 0;
    }

    GameConfig_Copy(&instance->gameConfig, (SlotGameConfig_t*)config);
    return 1;
}

int8_t GameRegistry_InitAndRegisterDefaults(GameId_t* pLastGameId)
{
    SlotGameConfig_t tempConfig;
    GameId_t lastGameId = GAME_ID_INVALID;
    int32_t baseValueArray[3] = { 3000, 1500, 500 };
    int32_t maxValueArray[3] = { JPWeight[1], JPWeight[2], JPWeight[3] };

    // 每次初始化都从干净状态开始，避免热重载后残留旧实例。
    GameManager_Init();

    // 奖池基础参数集中在此处，统一主程序与协议初始化行为。
    LotteryManager_Init();
    LotteryManager_SetBaseValue(&gLotteryManager, baseValueArray, maxValueArray);

    // gameId=1700: ZhuZaiJinBi_1700
    GameConfig_Init(&tempConfig);
    tempConfig.header.id = 0;
    tempConfig.header.enabled = 0;
    tempConfig.header.gameMode = 0;
    tempConfig.header.difficulty = 0;
    tempConfig.header.minBet = 0;
    tempConfig.header.maxBet = 0;
    tempConfig.header.ChessTypeNum = 12;
    tempConfig.header.ChessNorTypeNum = 9;
    tempConfig.header.lineCount = 15;
    tempConfig.header.reelCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.MaxIDNyn = 15;
    tempConfig.header.freeGameMax = 20;
    tempConfig.header.Wild = 9;
    tempConfig.header.Scatter = 10;
    tempConfig.header.Bonus = 11;
    tempConfig.header.normalRollTableId = 0;
    tempConfig.header.freeRollTableId = 0;
    if (!GameRegistry_RegisterGameWithConfig("ZhuZaiJinBi_1700", 1700, &tempConfig))
    {
        return 0;
    }
    lastGameId = 1700;

    // gameId=3999: CaiFuZhiMen_3999
    GameConfig_Init(&tempConfig);
    tempConfig.header.id = 1;
    tempConfig.header.enabled = 0;
    tempConfig.header.gameMode = 0;
    tempConfig.header.difficulty = 0;
    tempConfig.header.minBet = 0;
    tempConfig.header.maxBet = 0;
    tempConfig.header.ChessTypeNum = 12;
    tempConfig.header.ChessNorTypeNum = 9;
    tempConfig.header.lineCount = 20;
    tempConfig.header.reelCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.MaxIDNyn = 20;
    tempConfig.header.freeGameMax = 12;
    tempConfig.header.Wild = 9;
    tempConfig.header.Scatter = 10;
    tempConfig.header.Bonus = 11;
    tempConfig.header.normalRollTableId = 1;
    tempConfig.header.freeRollTableId = 1;
    if (!GameRegistry_RegisterGameWithConfig("CaiFuZhiMen_3999", 3999, &tempConfig))
    {
        return 0;
    }
    lastGameId = 3999;

    // gameId=3998: XingYunZhiLun_3998
    GameConfig_Init(&tempConfig);
    tempConfig.header.id = 2;
    tempConfig.header.enabled = 0;
    tempConfig.header.gameMode = 0;
    tempConfig.header.difficulty = 0;
    tempConfig.header.minBet = 0;
    tempConfig.header.maxBet = 0;
    tempConfig.header.ChessTypeNum = 10;
    tempConfig.header.ChessNorTypeNum = 8;
    tempConfig.header.lineCount = 20;
    tempConfig.header.reelCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.MaxIDNyn = 20;
    tempConfig.header.freeGameMax = 6;
    tempConfig.header.Wild = 8;
    tempConfig.header.Bonus = 9;
    tempConfig.header.Scatter = 10;
    tempConfig.header.normalRollTableId = 2;
    tempConfig.header.freeRollTableId = 3;
    if (!GameRegistry_RegisterGameWithConfig("XingYunZhiLun_3998", 3998, &tempConfig))
    {
        return 0;
    }
    lastGameId = 3998;

    if (pLastGameId != NULL)
    {
        *pLastGameId = lastGameId;
    }

    return 1;
}
