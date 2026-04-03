#include "GameResultRegistry.h"

void GameResult_3997_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	GameResult_Generic_Normal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

void GameResult_3997_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId)
{
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}

void GameResult_3997_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId)
{
	if (info == NULL || inst == NULL || freeMxu == NULL) return;

	Matrix_u mxu;
	Matrix_u_reset(&mxu);
	uint16_t idVec[GE_MaxIDNum] = { 0 };

	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig, inst->gameConfig.header.Scatter);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeNum = 10;
	info->nFreeBet = 0;
	uint8_t WildMultiple = 2;
	for (uint8_t index = 0; index < info->nFreeNum; index++)
	{
		Matrix_u_reset(&mxu);
		NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu, inst->gameConfig.header.rowCount);
		// Wild 触发：累积 Wild 倍数
		WildMultiple += Matrix_u_getTypeNum(&mxu, inst->gameConfig, inst->gameConfig.header.Wild);
		info->WildPosArray[index][0] = WildMultiple;
		// 计算本轮矩阵收益并乘以 Wild 倍数
		int32_t matrixBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId);
		matrixBet *= WildMultiple;

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

void GameResult_3997_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId)
{
	if (info == NULL) return;

	int dataArray[GE_WheelChess3x5Num] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	uint8_t PosVec[GE_WheelChess3x5Num];
	uint8_t PosSize = inst->gameConfig.header.wheelChessNum;

	// 初始化可用的位置集合
	for (uint8_t i = 0; i < PosSize; i++)
	{
		PosVec[i] = dataArray[i];
	}

	uint8_t randNum = 0;
	uint8_t randPos = 0;

	// 扫描 Bonus 符号并计算对应彩金
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

	// 在可用位置上插入随机 Bonus
	uint8_t insertCount = 0;
	if (PosSize > 1)
	{
		insertCount = JRandFrom(1, PosSize - 1);
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

int8_t* GameResult_3997_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst)
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
	if (outRes->openType == OT_Give)
	{

	}

	int8_t* bonusStr;
	// 若为 Bonus 结果，则输出 Bonus 相关字段
	if (outRes->resType == RT_BonusWin)
	{
		append_format(strRes, 2048, &used, "\"BonusType\":%d,", outRes->nBonusType);
		append_format(strRes, 2048, &used, "\"BonusBet\":%d,", outRes->nBonusBet);
		bonusStr = ArrayU16ToString(outRes->BonusData, curwheelChessNum, 1);
		append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
		free(bonusStr);
	}

	// JP 奖金/积分（仅当有 JP 投注时输出）
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

void GameResult_3997_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId)
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

	//res[pos++] = outRes->nJPBet;				// nJPBet
	//res[pos++] = outRes->nJPType;				// nJPType
}