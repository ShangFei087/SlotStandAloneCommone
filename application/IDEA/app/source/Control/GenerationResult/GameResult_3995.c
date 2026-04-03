#include "GameResultRegistry.h"

#include "../Matrix/Matrix_u_TriggersById.h"
void GameResult_3995_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	if (inst == NULL || mxu == NULL || matrixBet == NULL || idVec == NULL) return;
	MatrixTriggerOps_t* ops = MatrixTriggerOps_t_Get(gameId);
	if (ops == NULL || ops->applyTriggers == NULL || ops->computeLineWin == NULL)
	{
		return;
	}

	NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, mxu, inst->gameConfig.header.rowCount);
	*matrixBet = computeLineWildWins_3995(mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info->WildPosArray[0]);
	ops->applyTriggers(mxu, &inst->gameConfig, gameId, &matrixBet);
}

void GameResult_3995_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId)
{
	// 输局结果生成
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}

// Bull 触发后的符号替换：把指定符号替换为 Wild(9)
void BullReplaceSymbol(Matrix_u* pMatrix, GameInstance_t* inst, uint8_t type)
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
void GameResult_3995_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId)
{
	if (info == NULL || inst == NULL || freeMxu == NULL) return;

	Matrix_u mxu;
	Matrix_u_reset(&mxu);
	uint16_t idVec[GE_MaxIDNum] = { 0 };
	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig, inst->gameConfig.header.Scatter);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	int8_t bullCount = 0; // Bull 符号累计数量
	int8_t bullCollectArray[4] = { 4,7,13,15 }; // Bull 收集阈值数组：达到后执行逐级替换
	int8_t symbolChangeArray[4] = { 6,7,8,9 }; // Bull 替换目标符号数组

	int8_t wildMultipliedArray[3] = { 2,3 }; // Wild 倍数表
	int8_t wildMultiplied = 0;// 当前轮 Wild 倍数
	int8_t wildCount = 0; // Wild 符号数量统计（当前未使用）
	int8_t wildindex = 0; // Wild 位置索引（当前未使用）

	// 转轮编码规则：1=>累加 Bull，2=>累加免费下注，3=>结束转轮
	int32_t WheelArray[12] = { 10001,20100,30001,20150,10001,20100,30001,20200,10001,20250,30001,20200 };
	// Bonus 可落点位置数组（长度=3）
	int8_t freeGamePosArray[3] = { 2,6,10 };
	int8_t turnTime = 0;// 转轮执行次数（最多 5 次）
	int8_t randNum = 0;
	int32_t WheelData = 0;// 当前转轮编码值
	int8_t DataKind = 0;// 编码类别：1/2/3
	while (turnTime <5)
	{
		
		if (turnTime == 4)
		{
			info->BonusData[turnTime] = freeGamePosArray[JRandFrom(0,2)];
			break;
		}

		randNum = JRandFrom(0, 11);
		info->BonusData[turnTime] = randNum;
		WheelData = WheelArray[randNum];
		DataKind = WheelData / 10000;
		if (DataKind == 3)
		{
			break;
		}
		else
		{
			if (DataKind == 2)
			{
				info->nFreeBet += WheelData % 20000;
			}
			else
			{
				bullCount += WheelData % 10000;
			}
		}

		// 根据转轮结果滚动/调整 WheelArray
		for (int8_t i = 0; i < 12; i++)
		{
			WheelData = WheelArray[i];
			DataKind = WheelData / 10000;
			if (DataKind == 2)
			{
				int addDate = WheelData % 20000 * 2;
				WheelArray[i] = 20000 + addDate;
			}
			else
			{
				if (DataKind == 1)
				{
					WheelArray[i] += 1;
				}
			}
		}
		++turnTime;
	}
	++turnTime;
	info->BlindSymbol = turnTime;

	for (uint8_t index = 0; index < info->nFreeNum; index++)
	{
		Matrix_u_reset(&mxu);
		NatureAlg_GenRndMxu(inst->gameConfig.header.freeRollTableId, &mxu, inst->gameConfig.header.rowCount);

		// 统计当前盘面 Bull 符号数量
		bullCount += Matrix_u_getTypeNum(&mxu, inst->gameConfig, 9);
		// 根据阈值从高到低逐级执行替换
		for (int8_t i = 3; i >= 0; --i)
		{
			if (bullCount >= bullCollectArray[i])
			{
				// 达到阈值后，把对应符号替换为 Wild(9)
				for (int8_t j = 0; j <= i; ++j)
				{
					BullReplaceSymbol(&mxu, inst,symbolChangeArray[j]);
				}
			}
		}

		bullCount += Matrix_u_getTypeNum(&mxu, inst->gameConfig, inst->gameConfig.header.Wild);
		for (uint32_t i = 0; i < inst->gameConfig.header.wheelChessNum; i++)
		{
			if (mxu.dataArray[i] == inst->gameConfig.header.Wild)
			{
				wildMultiplied = wildMultipliedArray[JRandFrom(0, 1)];
				info->WildPosArray[index][i] = wildMultiplied;
			}
		}
		int32_t matrixBet = computeLineWildWins_3995(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info->WildPosArray[index]);

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
void GameResult_3995_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId)
{
	if (info == NULL) return;

	int dataArray[GE_WheelChess4X5Num] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16 ,17, 18, 19 };
	uint8_t PosVec[GE_WheelChess4X5Num];
	uint8_t PosSize = GE_WheelChess4X5Num;

	// 初始化可用位置集合
	for (uint8_t i = 0; i < PosSize; i++)
	{
		PosVec[i] = dataArray[i];
	}

	uint8_t randNum = 0;
	uint8_t randPos = 0;

	// 追加随机插入 Bonus
	uint8_t insertCount = 0;
	if (PosSize > 1)
	{
		insertCount = JRandFrom(1, PosSize - 1);
	}
	// insertCount = 8;  // 固定插入数量示例（调试用）

	// 随机插入 Bonus 到未占用的位置
	for (uint8_t i = 0; i < insertCount; ++i)
	{
		// 随机选择位置索引
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

void GameResult_3995_ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec)
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
		pRes->BlindSymbol = info->BlindSymbol; // 盲符号/转轮次数
		for (uint8_t i = 0; i < GE_MaxFreeNum; ++i)
		{
			pRes->FreeBetArray[i] = info->FreeBetArray[i];
		}
		// 输出 BonusData（按盲符号数量）
		for (uint8_t i = 0; i < info->BlindSymbol; ++i)
		{
			pRes->BonusData[i] = info->BonusData[i];
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
		for (uint8_t i = 0; i < Mxu->idVecSize; ++i)
		{
			pRes->IDVec[i] = info->FreeIDVec[0][i];
		}

	}
	else if (resType == RT_Win)
	{
		for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
		{
			pRes->WildPosArray[i] = info->WildPosArray[0][i];
		}

	}
	else if (resType == RT_Lose)
	{

	}
	else if (resType == RT_Jackpot)
	{

	}
	else
	{
		// 其他结果类型（预留）
	}
}

int8_t* GameResult_3995_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst)
{
	char* strRes = (char*)malloc(2048);
	size_t used = 0;
	int8_t* idVecStr = NULL;
	int8_t* matrixStr = NULL;
	int8_t curwheelChessNum = inst->gameConfig.header.wheelChessNum;
	int8_t* bonusStr;
	int8_t* wildStr;
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

		bonusStr = ArrayU16ToString(outRes->BonusData, outRes->BlindSymbol, 1);
		append_format(strRes, 2048, &used, "\"WheelData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
		free(bonusStr);
	}
	
	wildStr = ByteArrayToString(outRes->WildPosArray, curwheelChessNum);
	append_format(strRes, 2048, &used, "\"WildData\":%s,", wildStr ? (const char*)wildStr : "[]");
	free(wildStr);

	
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

void GameResult_3995_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId)
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
		//WildData
		for (int32_t i = 0; i < outRes->BlindSymbol; i++)
		{
			res[pos++] = outRes->BonusData[i];
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