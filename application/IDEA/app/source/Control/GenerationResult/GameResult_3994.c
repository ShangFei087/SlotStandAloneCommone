#include "GameResultRegistry.h"


void GameResult_3994_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	GameResult_Generic_Normal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

void GameResult_3994_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId)
{
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}

void GameResult_3994_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId)
{
	(void)betVal;
	(void)gameId;

	if (info == NULL || inst == NULL || freeMxu == NULL) return;
	RoundInfo_t_Reset(info);
	Matrix_u mxu, tempMxu;
	Matrix_u tempMxuArray[11];
	Matrix_u_reset(&mxu);
	uint16_t idVec[GE_MaxIDNum] = { 0 };

	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig, inst->gameConfig.header.Scatter);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	for (uint8_t freeindex = 0; freeindex < info->nFreeNum; freeindex++)
	{
		Matrix_u_reset(&mxu);
		Matrix_u_reset(&tempMxu);
		//生成任意矩阵
		NatureAlg_GenRndMxu(inst->gameConfig.header.freeRollTableId, &mxu, inst->gameConfig.header.rowCount);
		Matrix_u_copy(&tempMxu, &mxu);
#ifdef _IMHERE
		uint8_t tempdata[15] = { 1,5,8,1,9,5,1,1,4,3,4,3,6,7,1 };
		Matrix_u_setIntData(&tempMxu, inst->gameConfig, tempdata);
#endif // _IMHERE


		//如果有wild则一整列变成wild
		for (uint8_t i = 1; i < inst->gameConfig.header.colCount; i++)
		{
			for (int8_t j = 0; j < inst->gameConfig.header.rowCount; j++)
			{
				uint8_t index = i + (j * inst->gameConfig.header.colCount);
				if (Matrix_u_getIntData(&tempMxu, inst->gameConfig, index) == inst->gameConfig.header.Wild)
				{
					Matrix_u_insertCol(&tempMxu, inst->gameConfig.header.Wild, i);
					break;
				}
			}
		}

		//高分图标转换次一级图标为自己,从最高到最低
		//8 7 6 5 4  一共五个需要转换的图标
		uint8_t hightSymbols[5] = { 8, 7, 6, 5, 4 };
		//当前需要判断的高分图标
		uint8_t curHightSymbol = 0;
		//次一级高分图标
		uint8_t nextHightSymbol = 0;
		//上下左右下标
		int8_t dirindex = 0;
		for (uint8_t s = 0; s < 4; s++)
		{
			curHightSymbol = hightSymbols[s];
			nextHightSymbol = hightSymbols[s + 1];
			//判断4个方向，越界,wild,不是次一级不需要转换
			for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; i++)
			{
				if (Matrix_u_getIntData(&tempMxu, inst->gameConfig, i) == curHightSymbol)
				{
					//上方向
					dirindex = i - inst->gameConfig.header.colCount;
					if ((dirindex > 0 && dirindex < inst->gameConfig.header.wheelChessNum) &&
						Matrix_u_getIntData(&tempMxu, inst->gameConfig, dirindex) != inst->gameConfig.header.Wild &&
						Matrix_u_getIntData(&tempMxu, inst->gameConfig, dirindex) == nextHightSymbol)
					{
						tempMxu.dataArray[dirindex] = nextHightSymbol;
					}
					//下方向
					dirindex = i + inst->gameConfig.header.colCount;
					if ((dirindex > 0 && dirindex < inst->gameConfig.header.wheelChessNum) &&
						Matrix_u_getIntData(&tempMxu, inst->gameConfig, dirindex) != inst->gameConfig.header.Wild &&
						Matrix_u_getIntData(&tempMxu, inst->gameConfig, dirindex) == nextHightSymbol)
					{
						tempMxu.dataArray[dirindex] = nextHightSymbol;
					}
					//左方向
					dirindex = i - 1;
					if ((dirindex > 0 && dirindex < inst->gameConfig.header.wheelChessNum) &&
						Matrix_u_getIntData(&tempMxu, inst->gameConfig, dirindex) != inst->gameConfig.header.Wild &&
						Matrix_u_getIntData(&tempMxu, inst->gameConfig, dirindex) == nextHightSymbol)
					{
						tempMxu.dataArray[dirindex] = nextHightSymbol;
					}
					//右方向
					dirindex = i + 1;
					if ((dirindex > 0 && dirindex < inst->gameConfig.header.wheelChessNum) &&
						Matrix_u_getIntData(&tempMxu, inst->gameConfig, dirindex) != inst->gameConfig.header.Wild &&
						Matrix_u_getIntData(&tempMxu, inst->gameConfig, dirindex) == nextHightSymbol)
					{
						tempMxu.dataArray[dirindex] = nextHightSymbol;
					}
				}
			}
		}

		int32_t matrixBet = Matrix_u_computerMatrixById(&tempMxu, idVec, &inst->gameConfig, (uint32_t)gameId, info);
		Matrix_u_copy(&tempMxuArray[freeindex], &tempMxu);
		info->nFreeBet += matrixBet;
		Matrix_u_copy(&info->pFreeMxu[freeindex], &mxu);
		info->FreeBetArray[freeindex] = matrixBet;
		if (matrixBet > 0)
		{
			for (uint8_t i = 0; i < tempMxu.idVecSize; ++i)
			{
				info->FreeIDVec[freeindex][i] = idVec[i];
			}
		}
		info->pFreeMxu[freeindex].idVecSize = tempMxu.idVecSize;
		info->pFreeMxu[freeindex].resultType = tempMxu.resultType;
	}
}

void GameResult_3994_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId)
{
	uint8_t spinsLeft = 3;
	uint8_t spinTime = 0;
	uint8_t RewardRollTableId = 4;
	uint8_t xingxingSymbol = 12; //猩猩下标
	uint8_t bananaSymbol = 13;//香蕉下标
	uint8_t xingxingSymbolNum = 0; //猩猩个数
	if (info == NULL || inst == NULL || bonusMxu == NULL) return;
	RoundInfo_t_Reset(info);
	Matrix_u mxu;
	while (1)
	{
		while (spinsLeft > 0)
		{
			--spinsLeft;//2 1 0

			//随机大奖矩阵 inst->gameConfig.header.RewardRollTableId=4
			NatureAlg_GenRndMxu(RewardRollTableId, &mxu, inst->gameConfig.header.rowCount);
			Matrix_u_copy(&info->pFreeMxu[spinTime], &mxu);
			//随机猩猩神像图标		//30%可以得猩猩神像
			if (xingxingSymbolNum < 4 && JRandFrom(0, 10000) >3000)
			{
				uint8_t randxingxingcout = JRandFrom(1, 3);
				Matrix_u_insertSymbol(&mxu, inst->gameConfig, xingxingSymbol, randxingxingcout);
				spinsLeft = 3;
				xingxingSymbolNum += randxingxingcout;
			}

			for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; i++)
			{
				if (Matrix_u_getIntData(&mxu, inst->gameConfig, i) == bananaSymbol)
				{
					uint32_t data = JRandFrom(15, 33) * inst->gameConfig.header.lineCount;
					info->FreeIDVec[spinTime][i] =30000+data;
					info->nBonusBet += data;
					info->BonusData[spinTime] += data;
				}

				if (Matrix_u_getIntData(&mxu, inst->gameConfig, i) == xingxingSymbol)
				{
					info->FreeIDVec[spinTime][i] = 20001;
				}
			}

			spinTime++;
		}

		if (spinTime > 10)
		{
			spinsLeft = 3;
			spinTime = 0;
			RoundInfo_t_Reset(info);
			continue;
		}
		info->BlindSymbol = spinTime-1; //BlindSymbol代替 大奖旋转次数
		break;
	}
}

void GameResult_3994_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId)
{
	uint8_t spinsLeft = 3;
	uint8_t spinTime = 0;
	uint8_t RewardRollTableId = 4;
	uint8_t xingxingSymbol = 12; //猩猩下标
	uint8_t bananaSymbol = 13;//香蕉下标
	uint8_t xingxingSymbolNum = 0; //猩猩个数
	uint8_t jackpotxingxingSymbolArray[3] = { 15,10,5 };//彩金需要的猩猩神像个数数组
	uint8_t randxingxingSymbolArray[3] = { 3,2,1 };//每次随机猩猩神像个数数组
	uint8_t jackpotxingxingSymbol = jackpotxingxingSymbolArray[info->JPTypeArray[0]]; //彩金需要的猩猩神像个数

	if (info == NULL || inst == NULL || jackpotMxu == NULL) return;
	RoundInfo_t_ResetexceptJp(info);
	Matrix_u mxu;
	while (1)
	{
		while (spinsLeft > 0)
		{
			--spinsLeft;//2 1 0

			//随机大奖矩阵 inst->gameConfig.header.RewardRollTableId=4
			NatureAlg_GenRndMxu(RewardRollTableId, &mxu, inst->gameConfig.header.rowCount);
			Matrix_u_copy(&info->pFreeMxu[spinTime], &mxu);
			//随机猩猩神像图标		//30%可以得猩猩神像
			if (xingxingSymbolNum <jackpotxingxingSymbol && JRandFrom(0, 10000) <7000)
			{
				uint8_t multiply= randxingxingSymbolArray[info->JPTypeArray[0]];
				uint8_t randxingxingcout = JRandFrom(1* multiply, 3* multiply);
				Matrix_u_insertSymbol(&mxu, inst->gameConfig, xingxingSymbol, randxingxingcout);
				spinsLeft = 3;
				xingxingSymbolNum += randxingxingcout;
			}

			for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; i++)
			{
				if (Matrix_u_getIntData(&mxu, inst->gameConfig, i) == bananaSymbol)
				{
					uint32_t data = JRandFrom(15, 20) * inst->gameConfig.header.lineCount;
					info->FreeIDVec[spinTime][i] = 30000 + data;
					info->nBonusBet += data;
					info->BonusData[spinTime] += data;
				}

				if (Matrix_u_getIntData(&mxu, inst->gameConfig, i) == xingxingSymbol)
				{
					info->FreeIDVec[spinTime][i] = 20001;
				}
			}

			spinTime++;
		}

		if (spinTime > 10&& jackpotxingxingSymbol>xingxingSymbolNum)
		{
			spinsLeft = 3;
			spinTime = 0;
			RoundInfo_t_ResetexceptJp(info);
			continue;
		}
		info->BlindSymbol = spinTime; //BlindSymbol代替 大奖旋转次数
		break;
	}
}

void GameResult_3994_ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec)
{
	pRes->resType = resType;
	Matrix_u_copy(&pRes->matrix, Mxu);
	for (uint8_t i = 0; i < Mxu->idVecSize; ++i)
	{
		pRes->IDVec[i] = idVec[i];
	}

	if (resType == RT_FreeWin)
	{
		pRes->nTotalFreeBet = info->nFreeBet;
		pRes->nTotalFreeTime = info->nFreeNum;
		pRes->matrix.idVecSize = Mxu->idVecSize;
		for (uint8_t i = 0; i < GE_MaxFreeNum; ++i)
		{
			pRes->FreeBetArray[i] = info->FreeBetArray[i];
		}

	}
	else if (resType == RT_BonusWin)
	{
		pRes->nBonusBet = info->nBonusBet;
		pRes->nBonusType = info->nBonusType;
		pRes->BlindSymbol = info->BlindSymbol;
		for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
		{
			pRes->BonusData[i] = info->BonusData[i];
		}

		pRes->matrix.idVecSize = Mxu->idVecSize;

		for (uint8_t i = 0; i < pRes->BlindSymbol; ++i)
		{
			memcpy(pRes->BonusPosArray[i],info->FreeIDVec[i], sizeof(pRes->BonusPosArray[i]));
		}

	}
	else if (resType == RT_Win)
	{

	}
	else if (resType == RT_Lose)
	{

	}
	else if (resType == RT_Jackpot)
	{
		pRes->nBonusBet = info->nBonusBet;
		pRes->BlindSymbol = info->BlindSymbol;
		pRes->matrix.idVecSize = Mxu->idVecSize;
		for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
		{
			pRes->BonusData[i] = info->BonusData[i];
		}
		for (uint8_t i = 0; i < pRes->BlindSymbol; ++i)
		{
			memcpy(pRes->BonusPosArray[i], info->FreeIDVec[i], sizeof(pRes->BonusPosArray[i]));
		}

		pRes->nJPCount = info->nJPCount;
		for (int i = 0; i < GAME_Local_JP_MAX; ++i)
		{
			pRes->JPBetArray[i] = info->JPBetArray[i];
			pRes->JPTypeArray[i] = info->JPTypeArray[i];
		}
		pRes->nTotalJackpotBet = info->nTotalJackpotBet;

		pRes->matrix.idVecSize = Mxu->idVecSize;
	}
	else
	{
		//失败结果
	}
}

int8_t* GameResult_3994_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst)
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
		append_format(strRes, 2048, &used, "\"BonusBet\":%d,", outRes->nBonusBet);
		bonusStr = ArrayU16ToString(outRes->BonusData, curwheelChessNum, 1);
		append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
		for (uint8_t i = 0; i < outRes->BlindSymbol; i++)
		{
			bonusStr = ArrayU16ToString(outRes->BonusPosArray[i], curwheelChessNum, 1);
			append_format(strRes, 2048, &used, "\"BonusPos%d\":%s,",i, bonusStr ? (const char*)bonusStr : "[]");
		}
		free(bonusStr);
	}

	//中了彩金
	if (outRes->nJPCount > 0)
	{
		int8_t* jpTypeStr = ByteArrayToString((int8_t*)outRes->JPTypeArray, (int8_t)outRes->nJPCount);
		int8_t* jpBetStr = ArrayToString((int32_t*)outRes->JPBetArray, outRes->nJPCount, 1);
		append_format(strRes, 2048, &used, "\"BonusBet\":%d,", outRes->nBonusBet);
		bonusStr = ArrayU16ToString(outRes->BonusData, curwheelChessNum, 1);
		append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
		for (uint8_t i = 0; i < outRes->BlindSymbol; i++)
		{
			bonusStr = ArrayU16ToString(outRes->BonusPosArray[i], curwheelChessNum, 1);
			append_format(strRes, 2048, &used, "\"BonusPos%d\":%s,", i, bonusStr ? (const char*)bonusStr : "[]");
		}

		append_format(strRes, 2048, &used, "\"JPCount\":%d,", outRes->nJPCount);
		append_format(strRes, 2048, &used, "\"JPTypeArray\":%s,", jpTypeStr ? (const char*)jpTypeStr : "[]");
		append_format(strRes, 2048, &used, "\"JPBetArray\":%s,", jpBetStr ? (const char*)jpBetStr : "[]");
		append_format(strRes, 2048, &used, "\"TotalJackpotBet\":%d,", outRes->nTotalJackpotBet);

		free(jpTypeStr);
		free(jpBetStr);
		free(bonusStr);
	}

	append_format(strRes, 2048, &used, "\"TotalBet\":%d", outRes->nMatrixBet);

	append_format(strRes, 2048, &used, "}");

	free(idVecStr);
	free(matrixStr);
	return (int8_t*)strRes;
}

void GameResult_3994_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId)
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
		for (int32_t i = 0; i < wheelChessNum; i++)
		{
			res[pos++] = outRes->BonusData[i];
		}

		for (int32_t i = 0; i < outRes->BlindSymbol; i++) //bonusPos
		{
			for (int32_t j = 0; j < wheelChessNum; j++)
			{
				res[pos++] = outRes->BonusPosArray[i][j];
			}
		}
	}

	if (outRes->resType == RT_Jackpot)
	{
		res[pos++] = outRes->nBonusBet;						// nBonusBet
		for (int32_t i = 0; i < wheelChessNum; i++)
		{
			res[pos++] = outRes->BonusData[i];
		}

		for (int32_t i = 0; i < outRes->BlindSymbol; i++) //bonusPos
		{
			for (int32_t j = 0; j < wheelChessNum; j++)
			{
				res[pos++] = outRes->BonusPosArray[i][j];
			}
		}

		res[pos++] = outRes->nJPCount;
		for (int32_t i = 0; i < outRes->nJPCount; ++i)
		{
			res[pos++] = outRes->JPTypeArray[i];
		}
		for (int32_t i = 0; i < outRes->nJPCount; ++i)
		{
			res[pos++] = outRes->JPBetArray[i];
		}
		res[pos++] = outRes->nTotalJackpotBet;
	}
}