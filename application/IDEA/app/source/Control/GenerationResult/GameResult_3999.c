#include "GameResultRegistry.h"


void GameResult_3999_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,uint16_t* idVec,GameInstanceId_t gameId)
{
	GameResult_Generic_Normal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

void GameResult_3999_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,uint16_t* idVec,GameInstanceId_t gameId)
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
	uint16_t idVec[GE_MaxIDNum] = { 0 };

	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig, inst->gameConfig.header.Scatter);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	for (uint8_t index = 0; index < info->nFreeNum; index++)
	{
		while (1)
		{
			Matrix_u_reset(&mxu);
			NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu, inst->gameConfig.header.rowCount);

			// 随机一列填满 wild
			uint8_t rndCol = JRandFrom(0, 4);
			Matrix_u_insertCol(&mxu, inst->gameConfig.header.Wild, rndCol);

			int32_t matrixBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info);
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

			uint8_t scatterCount2 = Matrix_u_getTypeNum(&mxu, inst->gameConfig, inst->gameConfig.header.Scatter);
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

	info->nBonusBet = info->JPBetArray[0];
	info->nBonusType = info->JPTypeArray[0];
}

void GameResult_3999_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId)
{
	GameResult_3999_GenBonus(info, betVal, inst, jackpotMxu, gameId);
}

int8_t* GameResult_3999_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst)
{
	char* strRes = (char*)malloc(2048);
	size_t used = 0;
	int8_t* idVecStr = NULL;
	int8_t* matrixStr = NULL;

	int8_t curwheelChessNum = inst->gameConfig.header.wheelChessNum;
	// 返回堆内存，调用方负责 free；失败返回 NULL。
	if (strRes == NULL || outRes == NULL) return NULL;

	strRes[0] = '\0';
	append_format(strRes, 2048, &used, "{");

	append_format(strRes, 2048, &used, "\"OpenType\":%d,", outRes->openType);
	append_format(strRes, 2048, &used, "\"ResultType\":%d,", outRes->resType);

	idVecStr = ArrayU16ToString(outRes->IDVec, GE_MaxIDNum, 0);
	append_format(strRes, 2048, &used, "\"IDVec\":%s,", idVecStr ? (const char*)idVecStr : "[]");

	matrixStr = ByteArrayToString(outRes->matrix.dataArray, curwheelChessNum);
	append_format(strRes, 2048, &used, "\"Matrix\":%s,", matrixStr ? (const char*)matrixStr : "[]");

	if (outRes->resType == RT_FreeWin)
	{
		append_format(strRes, 2048, &used, "\"TotalFreeBet\":%d,", outRes->nTotalFreeBet);
		append_format(strRes, 2048, &used, "\"TotalFreeTime\":%d,", outRes->nTotalFreeTime);

		int8_t* freeBetStr = ArrayToString((int32_t*)outRes->FreeBetArray, outRes->nTotalFreeTime, 1);
		append_format(strRes, 2048, &used, "\"FreeBetArray\":%s,", freeBetStr ? (const char*)freeBetStr : "[]");
		free(freeBetStr);
	}
	int8_t* wildStr;
	if (outRes->openType == OT_Give)
	{
		
	}

	uint8_t bonusCount = 0;
	uint8_t wildColCountArray[4] = { 1, 2, 3,3 };//3个转盘图标可以得到1列wild图标，4个转盘可以的2列......
	int8_t* bonusStr;
	// gameId 失配时给默认值，避免空实例导致访问非法内存。
	if (inst != NULL)
	{
		bonusCount = Matrix_u_getTypeNum(&outRes->matrix, inst->gameConfig, inst->gameConfig.header.Bonus);
	}
	if (outRes->resType == RT_BonusWin)
	{
		append_format(strRes, 2048, &used, "\"BonusType\":%d,", outRes->nBonusType);
		append_format(strRes, 2048, &used, "\"BonusBet\":%d,", outRes->nBonusBet);
		bonusStr = ArrayU16ToString(outRes->BonusData, curwheelChessNum, 1);
		append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
		free(bonusStr);
	}

	//中了彩金
	if (outRes->nJPCount > 0)
	{
		int8_t* jpTypeStr = ByteArrayToString((int8_t*)outRes->JPTypeArray, (int8_t)outRes->nJPCount);
		int8_t* jpBetStr = ArrayToString((int32_t*)outRes->JPBetArray, outRes->nJPCount, 1);
		append_format(strRes, 2048, &used, "\"JPCount\":%d,", outRes->nJPCount);
		append_format(strRes, 2048, &used, "\"JPTypeArray\":%s,", jpTypeStr ? (const char*)jpTypeStr : "[]");
		append_format(strRes, 2048, &used, "\"JPBetArray\":%s,", jpBetStr ? (const char*)jpBetStr : "[]");
		append_format(strRes, 2048, &used, "\"TotalJackpotBet\":%d,", outRes->nTotalJackpotBet);
		free(jpTypeStr);
		free(jpBetStr);
	}

	append_format(strRes, 2048, &used, "\"TotalBet\":%d", outRes->nMatrixBet);

	append_format(strRes, 2048, &used, "}");

	free(idVecStr);
	free(matrixStr);
	return (int8_t*)strRes;
}

void GameResult_3999_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId)
{
	int32_t pos = 0;
	uint8_t wheelChessNum = inst->gameConfig.header.wheelChessNum;

	res[pos++] = (int32_t)gameId;						// ret
	res[pos++] = (int32_t)outRes->openType;				// OpenType
	res[pos++] = (int32_t)outRes->resType;				// ResultType
	res[pos++] = outRes->matrix.idVecSize;				// lineNum
	res[pos++] = outRes->nMatrixBet;					// TotalBet
	res[pos++] = wheelChessNum;							// MatrixLength
	// IDVec
	for (int32_t i = 0; i < outRes->matrix.idVecSize; i++)
	{
		res[pos++] = outRes->IDVec[i];
	}
	//Matrix
	for (int32_t i = 0; i < wheelChessNum; i++)
	{
		res[pos++] = outRes->matrix.dataArray[i];
	}

	if (outRes->resType == RT_FreeWin)
	{
		res[pos++] = outRes->nTotalFreeTime;					// nTotalFreeTime
		res[pos++] = outRes->nTotalFreeBet;					// nTotalFreeBet
		//FreeBetArray
		for (int32_t i = 0; i < outRes->nTotalFreeTime; i++)
		{
			res[pos++] = outRes->FreeBetArray[i];
		}
	}

	if (outRes->openType == OT_Give)
	{

	}

	if (outRes->resType == RT_BonusWin)
	{
		res[pos++] = outRes->nBonusBet;						// nBonusBet
		res[pos++] = outRes->nBonusType;						// nBonusType
		for (int32_t i = 0; i < wheelChessNum; i++)
		{
			res[pos++] = outRes->BonusData[i];
		}
	}

	//res[pos++] = OutResult_GetLocalJPBetTotal(outRes);
	//res[pos++] = (int32_t)outRes->JPTypeArray[0];

}
