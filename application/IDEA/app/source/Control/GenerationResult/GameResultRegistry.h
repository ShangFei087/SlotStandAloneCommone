#ifndef _GameResultRegistry_H_
#define _GameResultRegistry_H_

#include "Control/CommonStruct.h"
#include "Control/GameManager.h"
#include "Control/NatureAlg.h"

void GameResult_Generic_Normal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, int32_t* idVec, GameInstanceId_t gameId);
void GameResult_Generic_Lose(GameInstance_t* inst, Matrix_u* loseMxu, int32_t* idVec, GameInstanceId_t gameId);
void GameResult_Generic_NormalWin(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, int32_t* idVec, GameInstanceId_t gameId);
void GameResult_Generic_NormalLose(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, int32_t* idVec, GameInstanceId_t gameId);

void GenerationResult_GenerateNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, int32_t* idVec, GameInstanceId_t gameId);
void GenerationResult_GenerateFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId);
void GenerationResult_GenerateBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId);
void GenerationResult_GenerateLose(GameInstance_t* inst, Matrix_u* loseMxu, int32_t* idVec, GameInstanceId_t gameId);

typedef void (*GenNormalFn)(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);
typedef void (*GenNormalWinFn)(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);
typedef void (*GenNormalLoseFn)(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);
typedef void (*GenFreeFn)(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId);
typedef void (*GenBonusFn)(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId);
typedef void (*GenLoseFn)(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId);

typedef struct
{
	GenNormalFn genNormal;
	GenNormalWinFn genNormalWin;
	GenNormalLoseFn genNormalLose;
	GenFreeFn genFree;
	GenBonusFn genBonus;
	GenLoseFn genLose;
} GameResultOps_t;

int8_t GameResultRegistry_Register(GameInstanceId_t gameId, const GameResultOps_t* ops);
const GameResultOps_t* GameResultRegistry_Get(GameInstanceId_t gameId);

// 初始化内置游戏
void GameResultRegistry_InitDefaults(void);

#endif // _GameResultRegistry_H_

