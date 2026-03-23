#ifndef _GenerationResult_H_
#define _GenerationResult_H_

#include "CommonStruct.h"
#include "GameManager.h"

typedef void (*GenerationResultDummy_t)(void);

void GenerationResult_GenerateNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId);

void GenerationResult_GenerateFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId);

void GenerationResult_GenerateBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId);

void GenerationResult_GenerateLose(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId);

#endif // _GenerationResult_H_

