#include "GameResultRegistry.h"
#include "../Matrix/Matrix_u_TriggersById.h"
#define SYM_BULL        10  // 公牛
#define SYM_GOLDEN_BULL 14  // 金牛
// 轴2/3/4 上 Wild 最多赋 2 个 ×2/×3（普通/免费共用）
void AssignWildMul_3995(Matrix_u* mxu, GameInstance_t* inst, int8_t* wildRow)
{
	uint8_t cand[GE_WheelChessMaxNum];
	uint8_t candNum = 0;
	uint8_t n = inst->gameConfig.header.wheelChessNum;
	const int8_t mulTable[2] = { 2, 3 };
	memset(wildRow, 0, GE_WheelChessMaxNum);
	for (uint8_t i = 0; i < n; ++i)
	{
		if (mxu->dataArray[i] != inst->gameConfig.header.Wild)
			continue;
		uint8_t col = (uint8_t)(i % COL_MAX);
		if (col < 1 || col > 3)
			continue;
		cand[candNum++] = i;
	}
	uint8_t pickNum = candNum;
	if (pickNum > 2)
		pickNum = 2;
	for (uint8_t k = 0; k < pickNum; ++k)
	{
		uint8_t r = (uint8_t)JRandFrom(0, candNum - 1);
		uint8_t pos = cand[r];
		wildRow[pos] = mulTable[JRandFrom(0, 1)];
		cand[r] = cand[candNum - 1];
		candNum--;
	}
}

void GameResult_3995_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	(void)betVal;
	if (inst == NULL || mxu == NULL || matrixBet == NULL || idVec == NULL)
		return;
	MatrixTriggerOps_t* ops = MatrixTriggerOps_t_Get(gameId);
	if (ops == NULL || ops->applyTriggers == NULL || ops->computeLineWin == NULL)
		return;
	NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, mxu, inst->gameConfig.header.rowCount);
	AssignWildMul_3995(mxu, inst, info->WildPosArray[0]);
	*matrixBet = (int32_t)computeLineWildWins_3995(mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info->WildPosArray[0]);
	ops->applyTriggers(mxu, &inst->gameConfig, gameId, (uint32_t*)matrixBet, info);
}

void GameResult_3995_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId)
{
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}
// 把盘面上指定符号全部换成公牛(10)
void BullReplaceSymbol(Matrix_u* pMatrix, GameInstance_t* inst, uint8_t type)
{
	for (int8_t i = 0; i < inst->gameConfig.header.wheelChessNum; ++i)
	{
		if (pMatrix->dataArray[i] == type)
			pMatrix->dataArray[i] = SYM_BULL;
	}
}
// 免费游戏：转盘 + 免费局
void GameResult_3995_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId)
{
	if (info == NULL || inst == NULL || freeMxu == NULL)
		return;
	Matrix_u OriginalMx;   // 本局刚转出的原始盘 → 存入 pFreeMxu
	Matrix_u tempmxu;      // 替换后的结算盘 → 只用于算奖
	Matrix_u_reset(&OriginalMx);
	Matrix_u_reset(&tempmxu);
	uint16_t idVec[GE_MaxIDNum] = { 0 };
	uint16_t goldbullmeter = 0;  // 或转盘结束后先把转盘累计放进 meter
	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig, inst->gameConfig.header.Scatter);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;
	// 阈值：≥4鹰 ≥7豹 ≥13狼 ≥15鹿 → 公牛
	int8_t bullCollectArray[4] = { 4, 7, 13, 15 };
	int8_t symbolChangeArray[4] = { 9, 8, 7, 6 };
	// 万位：1金牛 / 2奖金倍数 / 3免费；奖金低位=押注倍数(初始2~5)
	int32_t WheelArray[12] = {
		10001, 20002, 30001, 20003,
		10001, 20004, 30001, 20005,
		10001, 20002, 30001, 20003
	};
	int8_t goldPos[3] = { 0, 4, 8 };
	int8_t freeGamePosArray[3] = { 2, 6, 10 };
	int8_t turnTime = 0;
	int8_t randNum = 0;
	int32_t WheelData = 0;
	int8_t DataKind = 0;
	// 若 betVal 已是总押注，改为：int32_t totalbetBase = betVal;
	int32_t totalbetBase = betVal * (int32_t)inst->gameConfig.header.lineCount;
	// —— 转盘 ——
	while (turnTime < 5)
	{
		// 第 5 次：非免费切片改为免费，再强制停在免费格
		if (turnTime == 4)
		{
			for (int8_t i = 0; i < 12; ++i)
			{
				if (WheelArray[i] / 10000 != 3)
					WheelArray[i] = 30001;
			}
			info->BonusData[turnTime] = (uint16_t)freeGamePosArray[JRandFrom(0, 2)];
			break;
		}
		randNum = (int8_t)JRandFrom(0, 11);
		info->BonusData[turnTime] = (uint16_t)randNum;
		WheelData = WheelArray[randNum];
		DataKind = (int8_t)(WheelData / 10000);
		if (DataKind == 3)
		{
			break; // 转到免费 → 退出转盘
		}
		else if (DataKind == 2)
		{
			info->nFreeBet += (WheelData % 20000) * totalbetBase;
		}
		else if (DataKind == 1)
		{
			goldbullmeter += (uint16_t)(WheelData % 10000);
			
		}
		// 奖金切片：倍数 +1
		for (int8_t i = 0; i < 12; ++i)
		{
			if (WheelArray[i] / 10000 == 2)
				WheelArray[i] += 1;
		}
		// 金牛切片：每次升级 3 个格子各 +1
		for (int8_t k = 0; k < 3; ++k) 	WheelArray[goldPos[k]] += 1;

		++turnTime;
	}
	++turnTime;
	info->BlindSymbol = turnTime;
	
	// —— 免费局 ——
	for (uint8_t index = 0; index < info->nFreeNum; ++index)
	{
		Matrix_u_reset(&OriginalMx);
		NatureAlg_GenRndMxu(inst->gameConfig.header.freeRollTableId, &OriginalMx, inst->gameConfig.header.rowCount);
		// 结算盘从原始盘拷出，只改 tempmxu
		Matrix_u_copy(&tempmxu, &OriginalMx);
		// 金牛计量用原始盘
		goldbullmeter += Matrix_u_getTypeNum(&OriginalMx, inst->gameConfig, SYM_GOLDEN_BULL);
		info->nGoldBullCount[index] = goldbullmeter;
		// 仅在结算盘上替换
		BullReplaceSymbol(&tempmxu, inst, SYM_GOLDEN_BULL); // 14→10
		for (int8_t i = 3; i >= 0; --i)
		{
			if (info->nGoldBullCount[index] >= bullCollectArray[i])
			{
				for (int8_t j = 0; j <= i; ++j)
					BullReplaceSymbol(&tempmxu, inst, symbolChangeArray[j]);
				break;
			}
		}
		AssignWildMul_3995(&tempmxu, inst, info->WildPosArray[index]);
		int32_t matrixBet = (int32_t)computeLineWildWins_3995(&tempmxu, idVec, &inst->gameConfig, (uint32_t)gameId, info->WildPosArray[index]);
		info->nFreeBet += matrixBet;
		info->FreeBetArray[index] = matrixBet;
		if (matrixBet > 0)
		{
			for (uint8_t i = 0; i < tempmxu.idVecSize; ++i)
				info->FreeIDVec[index][i] = idVec[i];
		}
		// pFreeMxu 存原始盘；线奖信息来自结算盘
		Matrix_u_copy(&info->pFreeMxu[index], &OriginalMx);
		info->pFreeMxu[index].idVecSize = tempmxu.idVecSize;
		info->pFreeMxu[index].resultType = tempmxu.resultType;

	}
}
// 奖励/彩金结果生成
void GameResult_3995_GenBonus(RoundInfo_t * info, int32_t betVal, GameInstance_t * inst, Matrix_u * bonusMxu, GameInstanceId_t gameId)
{
	(void)betVal;
	(void)bonusMxu;
	(void)gameId;
	if (info == NULL) return;
	int dataArray[GE_WheelChess3x5Num] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	uint8_t PosVec[GE_WheelChess3x5Num];
	uint8_t PosSize = GE_WheelChess3x5Num;
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

void GameResult_3995_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId)
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
		if (jackpotMxu->dataArray[i] == inst->gameConfig.header.Bonus)
		{
			uint16_t score = JRandFrom(10, 30) * inst->gameConfig.header.lineCount;
			info->BonusData[i] = 1000 + score;
			info->nBonusBet += score;

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
		insertCount = JRandFrom(3, PosSize - 1);
	}
	// insertCount = 8;  // 固定插入数量示例（调试用）
	//先插入彩金图标
	if (info->nJPCount > 0)
	{
		if (insertCount > info->nJPCount)
		{
			insertCount -= info->nJPCount;
		}
		else
		{
			insertCount = 0;
		}
		for (uint8_t i = 0; i < info->nJPCount; ++i)
		{
			// 随机选择一个可用位置索引
			randNum = JRandFrom(0, PosSize - 1);
			// 获取该索引对应的实际落点位置
			randPos = PosVec[randNum];
			info->BonusData[randPos] = 4000 + info->JPTypeArray[i];

			// 删除已使用的位置并向后移位
			for (int j = randNum; j < PosSize - 1; j++)
			{
				PosVec[j] = PosVec[j + 1];
			}
			PosSize--;
		}
	}

	// 随机插入 Bonus 到未占用的位置
	for (uint8_t i = 0; i < insertCount; ++i)
	{
		// 随机选择一个可用位置索引
		randNum = JRandFrom(0, PosSize - 1);
		// 获取该索引对应的实际落点位置
		randPos = PosVec[randNum];
		uint16_t score = JRandFrom(10, 30) * inst->gameConfig.header.lineCount;
		info->BonusData[randPos] = 1000 + score;
		info->nBonusBet += score;

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
		pRes->nBonusBet = info->nBonusBet;
		for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
		{
			pRes->BonusData[i] = info->BonusData[i];
		}

		pRes->nJPCount = info->nJPCount;
		for (int i = 0; i < GAME_Local_JP_MAX; ++i)
		{
			pRes->JPBetArray[i] = info->JPBetArray[i];
			pRes->JPTypeArray[i] = info->JPTypeArray[i];
		}
		pRes->nTotalJackpotBet = info->nTotalJackpotBet;

		pRes->matrix.idVecSize = Mxu->idVecSize;
		for (uint8_t i = 0; i < Mxu->idVecSize; ++i)
		{
			pRes->IDVec[i] = info->FreeIDVec[0][i];
		}
	}
	else
	{
		// 其他结果类型（预留）
	}
}
//应用免费结果
void GameResult_3995_ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx)
{
	OutResult_reset(pRes);
	int8_t resType = 0;
	//设定结果类型
	if (info->FreeBetArray[freeIdx] > 0)
	{
		resType = RT_Win;
	}
	else
	{
		resType = RT_Lose;
	}

	//设定矩阵数据
	Matrix_u_copy(&pRes->matrix, &info->pFreeMxu[freeIdx]);
	pRes->nMatrixBet = info->FreeBetArray[freeIdx];
	// FreeIDVec 的元素类型为 uint16_t，直接按类型拷贝到 outRes->IDVec。
	for (uint8_t i = 0; i < GE_MaxIDNum; ++i)
	{
		pRes->IDVec[i] = info->FreeIDVec[freeIdx][i];
	}
	memcpy(pRes->WildPosArray, info->WildPosArray[freeIdx], sizeof(pRes->WildPosArray));
	pRes->nGoldBullCount = info->nGoldBullCount[freeIdx];
	pRes->resType = resType;
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

	if (outRes->openType == OT_Give)
	{
		append_format(strRes, 2048, &used, "\"GoldBullCount\":%d,", outRes->nGoldBullCount);
	}

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
	//WildData
	for (int32_t i = 0; i < wheelChessNum; i++)
	{
		res[pos++] = outRes->WildPosArray[i];
	}

	if (outRes->resType == RT_FreeWin)
	{
		res[pos++] = outRes->nTotalFreeTime;					// nTotalFreeTime
		res[pos++] = outRes->nTotalFreeBet;					// nTotalFreeBet
		res[pos++] = outRes->BlindSymbol;					//转盘次数			
		//FreeBetArray
		for (int32_t i = 0; i < outRes->nTotalFreeTime; i++)
		{
			res[pos++] = outRes->FreeBetArray[i];
		}
		//WildData / WheelData（转盘下标，长度=BlindSymbol）
		for (int32_t i = 0; i < outRes->BlindSymbol; i++)
		{
			res[pos++] = outRes->BonusData[i];
		}
	}

	if (outRes->openType == OT_Give)
	{
		res[pos++] = outRes->nGoldBullCount;   //金牛图标
	}

	if (outRes->resType == RT_BonusWin)
	{
		res[pos++] = outRes->nBonusBet;							// nBonusBet
		for (int32_t i = 0; i < wheelChessNum; i++)
		{
			res[pos++] = outRes->BonusData[i];
		}
	}

	if (outRes->resType == RT_Jackpot)
	{
		res[pos++] = outRes->nBonusBet;
		for (int32_t i = 0; i < wheelChessNum; i++)
		{
			res[pos++] = outRes->BonusData[i];
		}
		res[pos++] = outRes->nJPCount;
		for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
		{
			res[pos++] = outRes->JPTypeArray[i];
		}
		for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
		{
			res[pos++] = outRes->JPBetArray[i];
		}
		res[pos++] = outRes->nTotalJackpotBet;
	}
}