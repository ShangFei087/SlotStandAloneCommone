#include "GameConfig.h"

#include "GameManager.h"
void GameConfig_Init(SlotGameConfig_t* slotConfig)
{
    if (slotConfig == NULL) return;
    memset(slotConfig, 0, sizeof(SlotGameConfig_t));
}

void GameConfig_Copy(SlotGameConfig_t* pDest, SlotGameConfig_t* pSrc)
{
    // 复制固定大小的结构部分
    memcpy(&pDest->header, &pSrc->header, sizeof(GameConfigHeader_t));
    memcpy(&pDest->paytable, &pSrc->paytable, sizeof(PaytableConfig_t));

}


// 注册单个游戏并拷贝配置
int8_t RegisterGameWithConfig(const int8_t* gameName, GameInstanceId_t gameId, const SlotGameConfig_t* config)
{
    GameInstanceId_t registeredGameId = GAME_ID_INVALID;
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

void GameConfigRegistry_InitDefaults()
{
    SlotGameConfig_t tempConfig;
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
    tempConfig.header.colCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.wheelChessNum = 15;
    tempConfig.header.MaxIDNyn = 15;
    tempConfig.header.freeGameMax = 20;
    tempConfig.header.Wild = 9;
    tempConfig.header.Scatter = 10;
    tempConfig.header.Bonus = 11;
    tempConfig.header.normalRollTableId = 0;
    tempConfig.header.freeRollTableId = 0;
    if (!RegisterGameWithConfig("ZhuZaiJinBi_1700", 1700, &tempConfig))
    {
        return 0;
    }

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
    tempConfig.header.colCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.wheelChessNum = 15;
    tempConfig.header.MaxIDNyn = 20;
    tempConfig.header.freeGameMax = 12;
    tempConfig.header.Wild = 9;
    tempConfig.header.Scatter = 10;
    tempConfig.header.Bonus = 11;
    tempConfig.header.normalRollTableId = 1;
    tempConfig.header.freeRollTableId = 1;
    if (!RegisterGameWithConfig("CaiFuZhiMen_3999", 3999, &tempConfig))
    {
        return 0;
    }

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
    tempConfig.header.colCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.wheelChessNum = 15;
    tempConfig.header.MaxIDNyn = 20;
    tempConfig.header.freeGameMax = 6;
    tempConfig.header.Wild = 8;
    tempConfig.header.Bonus = 9;
    tempConfig.header.Scatter = 10;
    tempConfig.header.normalRollTableId = 2;
    tempConfig.header.freeRollTableId = 3;
    if (!RegisterGameWithConfig("XingYunZhiLun_3998", 3998, &tempConfig))
    {
        return 0;
    }

    // gameId=3993: MeiZhouHeiBao_3993
    GameConfig_Init(&tempConfig);
    tempConfig.header.id = 3;
    tempConfig.header.enabled = 0;
    tempConfig.header.gameMode = 0;
    tempConfig.header.difficulty = 0;
    tempConfig.header.minBet = 0;
    tempConfig.header.maxBet = 0;
    tempConfig.header.ChessTypeNum = 13;
    tempConfig.header.ChessNorTypeNum = 10;
    tempConfig.header.lineCount = 25;
    tempConfig.header.colCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.wheelChessNum = 15;
    tempConfig.header.MaxIDNyn = 25;
    tempConfig.header.freeGameMax = 20;
    tempConfig.header.Wild = 10;
    tempConfig.header.Scatter = 11;
    tempConfig.header.Bonus = 12;
    tempConfig.header.normalRollTableId = 4;
    tempConfig.header.freeRollTableId = 4;
    if (!RegisterGameWithConfig("MeiZhouHeiBao_3993", 3993, &tempConfig))
    {
        return 0;
    }

    // gameId=3995: HuoYanGongNiu_3995
    GameConfig_Init(&tempConfig);
    tempConfig.header.id = 4;
    tempConfig.header.enabled = 0;
    tempConfig.header.gameMode = 0;
    tempConfig.header.difficulty = 0;
    tempConfig.header.minBet = 0;
    tempConfig.header.maxBet = 0;
    tempConfig.header.ChessTypeNum = 14;
    tempConfig.header.ChessNorTypeNum = 11;
    tempConfig.header.lineCount = 50;
    tempConfig.header.colCount = 5;
    tempConfig.header.rowCount = 4;
    tempConfig.header.wheelChessNum = 20;
    tempConfig.header.MaxIDNyn = 50;
    tempConfig.header.freeGameMax = 20;
    tempConfig.header.Wild = 11;
    tempConfig.header.Scatter = 12;
    tempConfig.header.Bonus = 13;
    tempConfig.header.normalRollTableId = 5;
    tempConfig.header.freeRollTableId = 6;
    if (!RegisterGameWithConfig("HuoYanGongNiu_3995", 3995, &tempConfig))
    {
        return 0;
    }

    // gameId=3997: CaiFuZhiJia_3997
    GameConfig_Init(&tempConfig);
    tempConfig.header.id = 5;
    tempConfig.header.enabled = 0;
    tempConfig.header.gameMode = 0;
    tempConfig.header.difficulty = 0;
    tempConfig.header.minBet = 0;
    tempConfig.header.maxBet = 0;
    tempConfig.header.ChessTypeNum = 12;
    tempConfig.header.ChessNorTypeNum = 9;
    tempConfig.header.lineCount = 20;
    tempConfig.header.colCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.wheelChessNum = 15;
    tempConfig.header.MaxIDNyn = 20;
    tempConfig.header.freeGameMax = 10;
    tempConfig.header.Wild = 9;
    tempConfig.header.Scatter = 10;
    tempConfig.header.Bonus = 11;
    tempConfig.header.normalRollTableId = 7;
    tempConfig.header.freeRollTableId = 7;
    if (!RegisterGameWithConfig("CaiFuZhiJia_3997", 3997, &tempConfig))
    {
        return 0;
    }

    // gameId=3996: CaiFuHuoChe_3996
    GameConfig_Init(&tempConfig);
    tempConfig.header.id = 6;
    tempConfig.header.enabled = 0;
    tempConfig.header.gameMode = 0;
    tempConfig.header.difficulty = 0;
    tempConfig.header.minBet = 0;
    tempConfig.header.maxBet = 0;
    tempConfig.header.ChessTypeNum = 12;
    tempConfig.header.ChessNorTypeNum = 9;
    tempConfig.header.lineCount = 30;
    tempConfig.header.colCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.wheelChessNum = 15;
    tempConfig.header.MaxIDNyn = 30;
    tempConfig.header.freeGameMax = 12;
    tempConfig.header.Wild = 9;
    tempConfig.header.Scatter = 10;
    tempConfig.header.Bonus = 11;
    tempConfig.header.normalRollTableId = 8;
    tempConfig.header.freeRollTableId = 9;
    if (!RegisterGameWithConfig("CaiFuHuoChe_3996", 3996, &tempConfig))
    {
        return 0;
    }
}

