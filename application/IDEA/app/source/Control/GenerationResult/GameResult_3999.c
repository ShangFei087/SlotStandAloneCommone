#include "GameResultRegistry.h"
#include <Control/CommonStruct.h>
#include <Control/GameManager.h>

void GameResult_3999_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId)
{
	GameResult_Generic_Normal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

void GameResult_3999_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId)
{
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}

void GameResult_3999_GenFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId)
{
	(void)betVal;
	(void)gameId;

	if (info == NULL || inst == NULL || freeMxu == NULL) return;

	Matrix_u mxu;
	Matrix_u_reset(&mxu);
	int32_t idVec[GE_MaxIDNum] = { 0 };

	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig.header.Scatter);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	for (uint8_t index = 0; index < info->nFreeNum; index++)
	{
		while (1)
		{
			Matrix_u_reset(&mxu);
			NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu);

			// 随机一列填满 wild
			uint8_t rndCol = JRandFrom(0, 4);
			Matrix_u_insertCol(&mxu, inst->gameConfig.header.Wild, rndCol);

			int32_t matrixBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId);
			info->nFreeBet += matrixBet;

			Matrix_u_copy(&info->pFreeMxu[index], &mxu);
			info->FreeBetArray[index] = matrixBet;

			if (matrixBet > 0)
			{
				for (uint8_t i = 0; i < mxu.idVecSize; ++i)
				{
					info->FreeIDVec[index][i] = idVec[i];
				}
			}

			uint8_t scatterCount2 = Matrix_u_getTypeNum(&mxu, inst->gameConfig.header.Scatter);
			if (info->nFreeNum + scatterCount2 <= 20)
			{
				info->nFreeNum += scatterCount2;
				break;
			}
			continue;
		}

		info->pFreeMxu[index].idVecSize = mxu.idVecSize;
		info->pFreeMxu[index].resultType = mxu.resultType;
	}
}

void GameResult_3999_GenBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId)
{
	(void)betVal;
	(void)bonusMxu;
	(void)gameId;

	if (info == NULL || inst == NULL) return;

	uint8_t bonusMulpitlyArray[4] = { 50,100,150,200 };
	uint32_t randNum = JRandFrom(0, 3);
	uint8_t bonusMulpitly = bonusMulpitlyArray[randNum];

	info->nBonusBet = inst->gameConfig.header.lineCount * bonusMulpitly;
	info->nBonusType = (int8_t)randNum;
}

