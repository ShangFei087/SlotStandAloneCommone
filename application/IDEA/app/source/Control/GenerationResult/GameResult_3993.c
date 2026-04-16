#include "GameResultRegistry.h"

#include "../Matrix/Matrix_u_TriggersById.h"

void GameResult_3993_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	// 正常结果生成
	GameResult_Generic_Normal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

void GameResult_3993_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId)
{
	// 输局结果生成
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}
// 黑豹触发后的符号替换：把指定符号替换为 Wild(9)
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
// 免费游戏结果生成
void GameResult_3993_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId)
{
	if (info == NULL || inst == NULL || freeMxu == NULL) return;

	Matrix_u mxu;
	Matrix_u_reset(&mxu);
	uint16_t idVec[GE_MaxIDNum] = { 0 };

	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig, inst->gameConfig.header.Scatter);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	int8_t wildMultipliedArray[3] = { 2,3,5 }; // Wild 倍数表
	int8_t wildMultiplied = 0; // 当前轮 Wild 倍数
	int8_t wildCount = 0; // Wild 计数（保留字段）
	int8_t wildindex = 0; // Wild 位置索引（保留字段）
	int8_t blackPantherCount = 0; // 黑豹符号累计数量
	int8_t BPCollectArray[4] = { 4,10,18,28 }; // 黑豹收集阈值数组：达到后执行逐级替换
	int8_t symbolChangeArray[4] = { 5,6,7,8 }; // 替换等级对应的符号类型
	for (uint8_t index = 0; index < info->nFreeNum; index++)
	{
		Matrix_u_reset(&mxu);
		NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu, inst->gameConfig.header.rowCount);

		// 统计盘面黑豹数量，并根据阈值触发逐级替换
		blackPantherCount += Matrix_u_getTypeNum(&mxu, inst->gameConfig, 9);
		// 从高到低逐级判断阈值
		for (int8_t i = 3; i >=0; --i)
		{
			if (blackPantherCount >= BPCollectArray[i])
			{
				// 达到阈值后，逐级执行符号替换（最终替换到 Wild(9)）
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
// 奖励/彩金结果生成
void GameResult_3993_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId)
{
	if (info == NULL) return;

	int dataArray[GE_WheelChess3x5Num] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	uint8_t PosVec[GE_WheelChess3x5Num];
	uint8_t PosSize = GE_WheelChess3x5Num;

	// 初始化可插入的位置集合
	for (uint8_t i = 0; i < PosSize; i++)
	{
		PosVec[i] = dataArray[i];
	}

	uint8_t randNum = 0;
	uint8_t randPos = 0;

	// 遍历 Bonus 符号并计算每个位置的彩金
	for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; ++i)
	{
		if (bonusMxu->dataArray[i] == inst->gameConfig.header.Bonus)
		{
			info->BonusData[i] = JRandFrom(10, 40) * inst->gameConfig.header.lineCount;
			info->nBonusBet += info->BonusData[i];

			// 如果位置已被占用，则从 PosVec 中删除该位置
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

	// 追加随机插入 Bonus（确保数量达到预期）
	uint8_t insertCount = 0;
	if (PosSize > 1)
	{
		insertCount = JRandFrom(1, PosSize-1);
	}
	// insertCount = 8;  // 固定插入数量示例（调试用）

	// 随机插入 Bonus 到未占用的位置
	for (uint8_t i = 0; i < insertCount; ++i)
	{
		// 随机选择一个可用位置索引
		randNum = JRandFrom(0, PosSize - 1);
		// 获取该索引对应的实际落点位置
		randPos = PosVec[randNum];
		info->BonusData[randPos] = JRandFrom(10, 40) * inst->gameConfig.header.lineCount;
		info->nBonusBet += info->BonusData[randPos];

		// 删除已使用的位置并向后移位
		for (int j = randNum; j < PosSize - 1; j++)
		{
			PosVec[j] = PosVec[j + 1];
		}
		PosSize--;
	}
}

void GameResult_3993_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId)
{
	GameResult_3993_GenBonus(info, betVal, inst, jackpotMxu, gameId);
}

int8_t* GameResult_3993_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst)
{
	char* strRes = (char*)malloc(2048);
	size_t used = 0;
	int8_t* idVecStr = NULL;
	int8_t* matrixStr = NULL;

	int8_t curwheelChessNum = inst->gameConfig.header.wheelChessNum;
	// JSON 缓冲分配失败时直接返回 NULL
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
		wildStr = ByteArrayToString(outRes->WildPosArray, curwheelChessNum);
		append_format(strRes, 2048, &used, "\"WildData\":%s,", wildStr ? (const char*)wildStr : "[]");
		free(wildStr);
	}

	uint8_t bonusCount = 0;
	uint8_t wildColCountArray[4] = { 1, 2, 3,3 };//3 个转盘图标得到 1 列 Wild，4 个得到 2 列...
	int8_t* bonusStr;
	// 如果 inst 不为空，则计算 Bonus 符号数量
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

	// JP 奖金/积分（仅当有 JP 投注时输出）
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

void GameResult_3993_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId)
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
		//WildPosArray
		for (int32_t i = 0; i < wheelChessNum; i++)
		{
			res[pos++] = outRes->WildPosArray[i];
		}
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