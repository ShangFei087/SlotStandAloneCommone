#include "GameResultRegistry.h"

#include "../Matrix/Matrix_u_TriggersById.h"

void GameResult_3993_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, int32_t* idVec, GameInstanceId_t gameId)
{
	// 当前与通用逻辑一致
	GameResult_Generic_Normal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

void GameResult_3993_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, int32_t* idVec, GameInstanceId_t gameId)
{
	// 当前与通用逻辑一致，预留每游戏独立扩展点
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}
// 替换黑豹图标
void BPReplaceSymbol(Matrix_u* pMatrix, GameInstance_t* inst, uint8_t type)
{
	for (int8_t i = 0; i < inst->gameConfig.header.wheelChessNum; ++i)
	{
		if (pMatrix->dataArray[i] == type)
		{
			pMatrix->dataArray[i] = 9;
		}
	}
}
//生成一局免费奖
void GameResult_3993_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId)
{
	if (info == NULL || inst == NULL || freeMxu == NULL) return;

	Matrix_u mxu;
	Matrix_u_reset(&mxu);
	int32_t idVec[GE_MaxIDNum] = { 0 };

	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig, inst->gameConfig.header.Scatter);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	int8_t wildMultipliedArray[3] = { 2,3,5 }; //wild倍数数组
	int8_t wildMultiplied = 0;//wild倍数
	int8_t wildCount = 0; //wild图标数量
	int8_t wildindex = 0; //wild下标
	int8_t blackPantherCount = 0; //黑豹图标数量
	int8_t BPCollectArray[4] = { 4,10,18,28 }; //黑豹图标收集区间数组
	int8_t symbolChangeArray[4] = { 5,6,7,8 }; //黑豹替换图标的数组
	for (uint8_t index = 0; index < info->nFreeNum; index++)
	{
		Matrix_u_reset(&mxu);
		NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu, inst->gameConfig.header.rowCount);

		//统计黑豹图标数量
		blackPantherCount += Matrix_u_getTypeNum(&mxu, inst->gameConfig, 9);
		//需要黑豹替换图标的区间
		for (int8_t i = 3; i >=0; --i)
		{
			if (blackPantherCount >= BPCollectArray[i])
			{
				//黑豹替换图标
				for (int8_t j = 0; j <= i; ++j)
				{
					BPReplaceSymbol(&mxu, inst, symbolChangeArray[j]);
				}
			}
		}

		blackPantherCount += Matrix_u_getTypeNum(&mxu, inst->gameConfig, inst->gameConfig.header.Wild);
		for (uint32_t i = 0; i < inst->gameConfig.header.wheelChessNum; i++)
		{
			if (mxu.dataArray[i] == inst->gameConfig.header.Wild)
			{
				wildMultiplied = wildMultipliedArray[JRandFrom(0, 2)];
				info->WildPosArray[index][i] = wildMultiplied;
			}
		}
		int32_t matrixBet = computeLineFreeWins_3993(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info->WildPosArray[index]);

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

		info->pFreeMxu[index].idVecSize = mxu.idVecSize;
		info->pFreeMxu[index].resultType = mxu.resultType;
	}
}
//生成一局大奖
void GameResult_3993_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId)
{
	if (info == NULL) return;

	int dataArray[GE_WheelChess3x5Num] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	uint8_t PosVec[GE_WheelChess3x5Num];
	uint8_t PosSize = GE_WheelChess3x5Num;

	// 复制原始数组
	for (uint8_t i = 0; i < PosSize; i++)
	{
		PosVec[i] = dataArray[i];
	}

	uint8_t randNum = 0;
	uint8_t randPos = 0;

	// 先填满bonus位置
	for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; ++i)
	{
		if (bonusMxu->dataArray[i] == inst->gameConfig.header.Bonus)
		{
			info->BonusData[i] = JRandFrom(10, 40) * inst->gameConfig.header.lineCount;
			info->nBonusBet += info->BonusData[i];

			// 从 PosVec 中删除已使用的位置
			for (int j = 0; j < PosSize; j++)
			{
				if (PosVec[j] == i)
				{
					for (int k = j; k < PosSize - 1; k++)
					{
						PosVec[k] = PosVec[k + 1];
					}
					PosSize--;
					break;
				}
			}
		}
	}

	// 随机bonus
	uint8_t insertCount = 0;
	if (PosSize > 1)
	{
		insertCount = JRandFrom(1, PosSize-1);
	}
	// insertCount = 8;  // 测试用，实际应注释

	// 再填其他位置
	for (uint8_t i = 0; i < insertCount; ++i)
	{
		// 随机选择一个位置索引
		randNum = JRandFrom(0, PosSize - 1);
		// 获取实际的列位置
		randPos = PosVec[randNum];
		info->BonusData[randPos] = JRandFrom(10, 40) * inst->gameConfig.header.lineCount;
		info->nBonusBet += info->BonusData[randPos];

		// 从数组中删除已使用的位置
		for (int j = randNum; j < PosSize - 1; j++)
		{
			PosVec[j] = PosVec[j + 1];
		}
		PosSize--;
	}
}

int8_t* OutResToJsonn_3993(OutResult_t* outRes, GameInstance_t* inst)
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

	idVecStr = ArrayToString((int32_t*)outRes->IDVec, GE_MaxIDNum, 0);
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
		wildStr = ByteArrayToString(outRes->WildPosArray, curwheelChessNum);
		append_format(strRes, 2048, &used, "\"WildData\":%s,", wildStr ? (const char*)wildStr : "[]");
		free(wildStr);
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
		bonusStr = ArrayToString((int32_t*)outRes->BonusData, curwheelChessNum, 1);
		append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
		free(bonusStr);
	}

	//中了彩金
	if (outRes->nJPBet > 0)
	{
		append_format(strRes, 2048, &used, "\"JPType\":%d", outRes->nJPType);

		append_format(strRes, 2048, &used, "\"JPBet\":%d", outRes->nJPBet);
	}

	append_format(strRes, 2048, &used, "\"TotalBet\":%d", outRes->nMatrixBet);

	append_format(strRes, 2048, &used, "}");

	free(idVecStr);
	free(matrixStr);
	return (int8_t*)strRes;
}