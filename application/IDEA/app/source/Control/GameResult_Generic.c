#include "GameResultRegistry.h"
#include "NatureAlg.h"

// 生成随机一局
void GameResult_Generic_Normal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId)
{
	if (inst == NULL || mxu == NULL || matrixBet == NULL || idVec == NULL) return;

	NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, mxu);
	*matrixBet = Matrix_u_computerMatrixById(mxu, idVec, &inst->gameConfig, (uint32_t)gameId);
}
//正常赢
void GameResult_Generic_NormalWin(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId)
{
	(void)info; (void)inst; (void)mxu; (void)betVal; (void)matrixBet; (void)idVec; (void)gameId;
}
//正常输
void GameResult_Generic_NormalLose(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId)
{
	(void)info; (void)inst; (void)mxu; (void)betVal; (void)matrixBet; (void)idVec; (void)gameId;
}

// 输局候选：循环生成直到 resultType == RT_Lose
void GameResult_Generic_Lose(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId)
{
	if (inst == NULL || loseMxu == NULL || idVec == NULL) return;

	while (1)
	{
		Matrix_u_reset(loseMxu);
		NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, loseMxu);
		Matrix_u_computerMatrixById(loseMxu, idVec, &inst->gameConfig, (uint32_t)gameId);
		if (loseMxu->resultType == RT_Lose)
		{
			break;
		}
	}
}

