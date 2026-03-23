#ifndef _GameResultRegistry_H_
#define _GameResultRegistry_H_

#include "CommonStruct.h"
#include "GameManager.h"

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

// 初始化内置游戏 ops（1700/3999/3998）
void GameResultRegistry_InitDefaults(void);

#endif // _GameResultRegistry_H_

