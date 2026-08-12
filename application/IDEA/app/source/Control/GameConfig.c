#include "GameConfig.h"

#include "GameManager.h"
void GameConfig_Init(SlotGameConfig_t* slotConfig)
{
    if (slotConfig == NULL) return;
    memset(slotConfig, 0, sizeof(SlotGameConfig_t));
}

void GameConfig_Copy(SlotGameConfig_t* pDest, SlotGameConfig_t* pSrc)
{
    memcpy(&pDest->header, &pSrc->header, sizeof(GameConfigHeader_t));
    memcpy(&pDest->paytable, &pSrc->paytable, sizeof(PaytableConfig_t));

}


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
    // gameId=3995: HuoYanGongNiu_3995
    GameConfig_Init(&tempConfig);
    tempConfig.header.id = 0;
    tempConfig.header.enabled = 0;
    tempConfig.header.gameMode = 0;
    tempConfig.header.difficulty = 0;
    tempConfig.header.minBet = 0;
    tempConfig.header.maxBet = 0;
    tempConfig.header.ChessTypeNum = 15;
    tempConfig.header.ChessNorTypeNum = 11;
    tempConfig.header.lineCount = 50;
    tempConfig.header.colCount = 5;
    tempConfig.header.rowCount = 3;
    tempConfig.header.wheelChessNum = 15;
    tempConfig.header.MaxIDNyn = 50;
    tempConfig.header.freeGameMax = 20;
    tempConfig.header.Wild = 11;
    tempConfig.header.Scatter = 12;
    tempConfig.header.Bonus = 13;
    tempConfig.header.normalRollTableId = 0;
    tempConfig.header.freeRollTableId = 1;
    tempConfig.header.localJpMaxCount = 3;
    if (!RegisterGameWithConfig("HuoYanGongNiu_3995", 3995, &tempConfig))
    {
        return 0;
    }

	// gameId=3994: FeiZhouHeiXX_3994
	GameConfig_Init(&tempConfig);
	tempConfig.header.id = 1;
	tempConfig.header.enabled = 0;
	tempConfig.header.gameMode = 0;
	tempConfig.header.difficulty = 0;
	tempConfig.header.minBet = 0;
	tempConfig.header.maxBet = 0;
	tempConfig.header.ChessTypeNum = 14;
	tempConfig.header.ChessNorTypeNum = 8;
	tempConfig.header.lineCount = 25;
	tempConfig.header.colCount = 5;
	tempConfig.header.rowCount = 3;
	tempConfig.header.wheelChessNum = 15;
	tempConfig.header.MaxIDNyn = 25;
	tempConfig.header.freeGameMax = 11;
	tempConfig.header.Wild = 9;
	tempConfig.header.Scatter = 10;
	tempConfig.header.Bonus = 11;
	tempConfig.header.normalRollTableId = 2;
    tempConfig.header.freeRollTableId = 3;
	tempConfig.header.localJpMaxCount = 1;
	if (!RegisterGameWithConfig("FeiZhouHeiXX_3994", 3994, &tempConfig))
	{
		return 0;
	}

    // gameId=3993: MeiZhouHeiBao_3993
    GameConfig_Init(&tempConfig);
    tempConfig.header.id = 2;
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
    tempConfig.header.normalRollTableId = 5;
    tempConfig.header.freeRollTableId = 6;
    tempConfig.header.localJpMaxCount = 1;
    if (!RegisterGameWithConfig("MeiZhouHeiBao_3993", 3993, &tempConfig))
    {
        return 0;
    }
}
