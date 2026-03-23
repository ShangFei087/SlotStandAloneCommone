#include "GameResultRegistry.h"

#include <string.h>

// Generic implementations (shared across all games).
void GameResult_Generic_Normal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);
void GameResult_Generic_Lose(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId);
void GameResult_Generic_NormalWin(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);
void GameResult_Generic_NormalLose(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);

// Per-game merged implementations: GenNormal / GenFree / GenBonus / GenLose.
void GameResult_1700_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);
void GameResult_1700_GenFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId);

void GameResult_1700_GenBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId);
void GameResult_1700_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId);

void GameResult_3999_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);
void GameResult_3999_GenFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId);

void GameResult_3999_GenBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId);
void GameResult_3999_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId);

void GameResult_3998_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);
void GameResult_3998_GenFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId);

void GameResult_3998_GenBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId);
void GameResult_3998_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId);

#define GAME_RESULT_REG_MAX 16

typedef struct
{
	GameInstanceId_t gameId;
	GameResultOps_t ops;
} GameResultRegEntry_t;

static GameResultRegEntry_t gReg[GAME_RESULT_REG_MAX];
static int32_t gRegCount = 0;

int8_t GameResultRegistry_Register(GameInstanceId_t gameId, const GameResultOps_t* ops)
{
	if (ops == NULL) return 0;
	if (gameId == GAME_INSTANCE_ID_INVALID) return 0;

	// 已存在则覆盖
	for (int32_t i = 0; i < gRegCount; ++i)
	{
		if (gReg[i].gameId == gameId)
		{
			gReg[i].ops = *ops;
			return 1;
		}
	}

	if (gRegCount >= GAME_RESULT_REG_MAX) return 0;

	gReg[gRegCount].gameId = gameId;
	gReg[gRegCount].ops = *ops;
	gRegCount++;
	return 1;
}

const GameResultOps_t* GameResultRegistry_Get(GameInstanceId_t gameId)
{
	if (gameId == GAME_INSTANCE_ID_INVALID) return NULL;
	for (int32_t i = 0; i < gRegCount; ++i)
	{
		if (gReg[i].gameId == gameId)
		{
			return &gReg[i].ops;
		}
	}
	return NULL;
}

void GameResultRegistry_InitDefaults(void)
{
	memset(gReg, 0, sizeof(gReg));
	gRegCount = 0;

	GameResultOps_t ops1700 = {0};
	ops1700.genNormal = GameResult_1700_GenNormal;
	ops1700.genNormalWin = GameResult_Generic_NormalWin;
	ops1700.genNormalLose = GameResult_Generic_NormalLose;
	ops1700.genFree = GameResult_1700_GenFree;
	ops1700.genBonus = GameResult_1700_GenBonus;
	ops1700.genLose = GameResult_1700_GenLose;
	(void)GameResultRegistry_Register(1700, &ops1700);

	GameResultOps_t ops3999 = {0};
	ops3999.genNormal = GameResult_3999_GenNormal;
	ops3999.genNormalWin = GameResult_Generic_NormalWin;
	ops3999.genNormalLose = GameResult_Generic_NormalLose;
	ops3999.genFree = GameResult_3999_GenFree;
	ops3999.genBonus = GameResult_3999_GenBonus;
	ops3999.genLose = GameResult_3999_GenLose;
	(void)GameResultRegistry_Register(3999, &ops3999);

	GameResultOps_t ops3998 = {0};
	ops3998.genNormal = GameResult_3998_GenNormal;
	ops3998.genNormalWin = GameResult_Generic_NormalWin;
	ops3998.genNormalLose = GameResult_Generic_NormalLose;
	ops3998.genFree = GameResult_3998_GenFree;
	ops3998.genBonus = GameResult_3998_GenBonus;
	ops3998.genLose = GameResult_3998_GenLose;
}

