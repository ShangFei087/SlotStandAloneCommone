#include "GameResultRegistry.h"


void GameResult_3998_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,uint16_t* idVec,GameInstanceId_t gameId)
{
	GameResult_Generic_Normal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

void GameResult_3998_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,uint16_t* idVec,GameInstanceId_t gameId)
{
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}

void GameResult_3998_GenBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId)
{
	(void)betVal;

	if (info == NULL || inst == NULL || bonusMxu == NULL) return;

	Matrix_u mxu;
	Matrix_u_reset(&mxu);

	uint16_t idVec[GE_MaxIDNum] = { 0 };
	uint8_t bonusCount = Matrix_u_getTypeNum(bonusMxu, inst->gameConfig, inst->gameConfig.header.Bonus);
	Matrix_u_copy(&mxu, bonusMxu);

	info->nBonusBet = 0;
	uint32_t randNum = 0;
	uint8_t bonusMulpitly = 0;
	uint8_t bonusMulpitlyArray[4] = { 50,100,150,200 };

	// 随机大奖
	randNum = JRandFrom(0, 10000);

#ifdef _DebugControlMode
	if (gDebugControlMode.mode == DCM_PointResData)
	{
		if (gDebugControlMode.bonusType == 0)
		{
			// Wild 游戏功能
			info->nBonusType = 0;

			uint8_t wildColCountArray[4] = { 1, 2, 3,3 }; // 3 个转盘图标得到 1 列 wild 图标，4 个得到 2 列...
			uint8_t wildArray[COL_MAX] = { 0, 1, 2, 3 , 4 };
			uint8_t  wildPosVec[COL_MAX];
			uint8_t  wildSize = COL_MAX;

			uint8_t randNum = 0;
			uint8_t randPos = 0;
			// 复制原始数组
			for (uint8_t i = 0; i < wildSize; i++)
			{
				wildPosVec[i] = wildArray[i];
			}

			for (uint8_t i = 0; i < wildColCountArray[bonusCount - 3]; ++i)
			{
				// 随机选择一个位置索引
				randNum = JRandFrom(0, wildSize - 1);
				// 获取实际的列位置
				randPos = wildPosVec[randNum];

				info->BonusData[i] = randPos;
				Matrix_u_insertCol(&mxu, inst->gameConfig.header.Wild, randPos);

				// 从数组中删除已使用的位置
				for (int j = randNum; j < wildSize - 1; j++)
				{
					wildPosVec[j] = wildPosVec[j + 1];
				}
				wildSize--;
			}

			info->BlindSymbol = bonusCount;
			info->nBonusBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info);

			bonusMxu->idVecSize = mxu.idVecSize;
			for (uint8_t i = 0; i < mxu.idVecSize; ++i)
			{
				info->FreeIDVec[0][i] = idVec[i];
			}
		}
		else
		{
			if (gDebugControlMode.bonusType == 1)
			{
				// 神秘游戏
				info->nBonusType = 1;
				uint8_t BlindSymbol = JRandFrom(0, 7);
				uint8_t BlindBoxCountArray[3] = { 6, 8, 11 };
				uint8_t BlindBoxCount = BlindBoxCountArray[bonusCount - 3];

				int dataArray[15] = { 0, 1, 2, 3 , 4 ,5,6,7,8,9,10,11,12,13,14 };
				uint8_t  PosVec[15];
				uint8_t  PosSize = 15;
				// 复制原始数组
				for (uint8_t i = 0; i < PosSize; i++)
				{
					PosVec[i] = dataArray[i];
				}

				uint8_t randNum = 0;
				uint8_t randPos = 0;
				//先填满bonus位置
				for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; ++i)
				{
					if (bonusMxu->dataArray[i] == inst->gameConfig.header.Bonus)
					{
						info->BonusData[i] = 1;
						// 从数组中删除已使用的位置
						for (int j = randNum; j < PosSize - 1; j++)
						{
							PosVec[j] = PosVec[j + 1];
						}
						PosSize--;
					}
				}

				//在填其他位置
				for (uint8_t i = 0; i < BlindBoxCount - bonusCount; ++i)
				{
					// 随机选择一个位置索引
					randNum = JRandFrom(0, PosSize - 1);
					// 获取实际的列位置
					randPos = PosVec[randNum];
					info->BonusData[randPos] = 1;
					// 从数组中删除已使用的位置
					for (int j = randNum; j < PosSize - 1; j++)
					{
						PosVec[j] = PosVec[j + 1];
					}
					PosSize--;
				}

				for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; ++i)
				{
					if (info->BonusData[i] == 1)
					{
						mxu.dataArray[i] = BlindSymbol;
					}
				}

#ifdef _IMHERE

				/*uint8_t mxudata[GE_WheelChessMaxNum] =
				{
				   0,1,7,7,8,
				   9,2,9,0,2,
				   5,7,6,2,9
				};
				Matrix_u_setIntData(&mxu, inst->gameConfig, mxudata);
				BlindSymbol = 0;
				uint8_t temp[GE_WheelChessMaxNum] =
				{
				   0,0,0,0,0,
				   1,1,1,0,0,
				   1,0,0,0,1
				};
				for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; ++i)
				{
					info->BonusData[i] = temp[i];
					if (temp[i] > 0)
					{
						mxu.dataArray[i] = BlindSymbol;
					}
				}*/
#endif 

				info->nBonusBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info);
				bonusMxu->idVecSize = mxu.idVecSize;
				for (uint8_t i = 0; i < mxu.idVecSize; ++i)
				{
					info->FreeIDVec[0][i] = idVec[i];
				}

				info->BlindSymbol = BlindSymbol; //代替一下
			}
			else
			{
				if (gDebugControlMode.bonusType == 2)
				{
					// 乘数游戏
					info->nBonusType = 2;
					uint8_t WheelArray[3][4] = { { 3, 4, 5, 7 } ,{6,7,8,10},{9,10,11,15} };
					uint8_t WheelMulptiy = WheelArray[bonusCount - 3][JRandFrom(0, 3)];
					info->BlindSymbol = WheelMulptiy; //代替一下
					info->nBonusBet = WheelMulptiy * inst->gameConfig.header.lineCount * bonusCount;
				}
				else
				{
					// 大奖游戏
					info->nBonusType = 3;
					randNum = JRandFrom(0, 4);
					bonusMulpitly = bonusMulpitlyArray[randNum];
					info->BlindSymbol = bonusMulpitly; //代替一下
					info->nBonusBet = inst->gameConfig.header.lineCount * bonusMulpitly;
				}
			}
		}

		return;
	}
#endif // _DebugControlMode

	// Wild 游戏功能
	if (randNum < 2500)
	{
	
		info->nBonusType = 0;
		uint8_t wildColCountArray[4] = { 1, 2, 3,3 };
		uint8_t wildArray[COL_MAX] = { 0, 1, 2, 3 , 4 };
		uint8_t  wildPosVec[COL_MAX];
		uint8_t  wildSize = COL_MAX;
		uint8_t randNum = 0;
		uint8_t randPos = 0;

		// 复制原始数组
		for (uint8_t i = 0; i < wildSize; i++)
		{
			wildPosVec[i] = wildArray[i];
		}

		for (uint8_t i = 0; i < wildColCountArray[bonusCount - 3]; ++i)
		{
			// 随机选择一个位置索引
			randNum = JRandFrom(0, wildSize - 1);
			//获取实际的列位置
			randPos = wildPosVec[randNum];
			info->BonusData[i] = randPos;
			Matrix_u_insertCol(&mxu, inst->gameConfig.header.Wild, randPos);
			// 从数组中删除已使用的位置
			for (int j = randNum; j < wildSize - 1; j++)
			{
				wildPosVec[j] = wildPosVec[j + 1];
			}
			wildSize--;
		}

		info->BlindSymbol = bonusCount;
		info->nBonusBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info);

		bonusMxu->idVecSize = mxu.idVecSize;
		for (uint8_t i = 0; i < mxu.idVecSize; ++i)
		{
			info->FreeIDVec[0][i] = idVec[i];
		}
	}
	else
	{
		// 神秘游戏
		if (randNum < 5000)
		{
			info->nBonusType = 1;
			uint8_t  BlindSymbol = JRandFrom(0, 7);
			uint8_t BlindBoxCountArray[3] = { 6, 8, 11 };
			uint8_t BlindBoxCount = BlindBoxCountArray[bonusCount - 3];

			int dataArray[15] = { 0, 1, 2, 3 , 4 ,5,6,7,8,9,10,11,12,13,14 };
			uint8_t  PosVec[15];
			uint8_t  PosSize = 15;

			// 复制原始数组
			for (uint8_t i = 0; i < PosSize; i++)
			{
				PosVec[i] = dataArray[i];
			}

			uint8_t randNum = 0;
			uint8_t randPos = 0;

			//先填满bonus位置
			for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; ++i)
			{
				if (bonusMxu->dataArray[i] == inst->gameConfig.header.Bonus)
				{
					info->BonusData[i] = 1;
					mxu.dataArray[i] = BlindSymbol;
					// 从数组中删除已使用的位置
					for (int j = randNum; j < PosSize - 1; j++)
					{
						PosVec[j] = PosVec[j + 1];
					}
					PosSize--;
				}
			}

			//在填其他位置
			for (uint8_t i = 0; i < BlindBoxCount - bonusCount; ++i)
			{
				// 随机选择一个位置索引
				randNum = JRandFrom(0, PosSize - 1);
				// 获取实际的列位置
				randPos = PosVec[randNum];
				info->BonusData[randPos] = 1;
				mxu.dataArray[randPos] = BlindSymbol;
				// 从数组中删除已使用的位置
				for (int j = randNum; j < PosSize - 1; j++)
				{
					PosVec[j] = PosVec[j + 1];
				}
				PosSize--;
			}

			info->nBonusBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info);
			bonusMxu->idVecSize = mxu.idVecSize;
			for (uint8_t i = 0; i < mxu.idVecSize; ++i)
			{
				info->FreeIDVec[0][i] = idVec[i];
			}
		}
		else
		{
			// 乘数游戏
			if (randNum < 7500)
			{
				
				info->nBonusType = 2;
				uint8_t WheelArray[3][4] = { { 3, 4, 5, 7 } ,{6,7,8,10},{9,10,11,15} };
				uint8_t WheelMulptiy = WheelArray[bonusCount - 3][JRandFrom(0, 3)];
				info->BlindSymbol = WheelMulptiy; //代替一下
				info->nBonusBet = WheelMulptiy * inst->gameConfig.header.lineCount * bonusCount;
			}
			else
			{
				info->nBonusType = 3;
				// 大奖游戏
				randNum = JRandFrom(0, 4);
				bonusMulpitly = bonusMulpitlyArray[randNum];
				info->BlindSymbol = bonusMulpitly; //代替一下
				info->nBonusBet = inst->gameConfig.header.lineCount * bonusMulpitly;
			}
		}
	}
}

void GameResult_3998_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId)
{

}

void GameResult_3998_GenFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId)
{
	(void)betVal;
	(void)gameId;

	if (info == NULL || inst == NULL || freeMxu == NULL) return;

	Matrix_u mxu;
	Matrix_u_reset(&mxu);

	Matrix_u tempmxu;
	Matrix_u_reset(&tempmxu);

	uint16_t idVec[GE_MaxIDNum] = { 0 };

	// 3998 免费：根据 Bonus 触发位置计算免费次数
	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig, inst->gameConfig.header.Bonus);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	// 收集 wild 位置（来自触发矩阵 freeMxu 上的 Bonus 和Wild）
	for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; i++)
	{
		if (Matrix_u_getIntData(freeMxu, inst->gameConfig, i) == inst->gameConfig.header.Bonus|| Matrix_u_getIntData(freeMxu, inst->gameConfig, i) == inst->gameConfig.header.Wild)
		{
			info->WildPosArray[0][i] = 1;
		}
	}

	for (uint8_t index = 0; index < info->nFreeNum; index++)
	{
		Matrix_u_reset(&mxu);
		Matrix_u_reset(&tempmxu);
		NatureAlg_GenRndMxu(inst->gameConfig.header.freeRollTableId, &mxu, inst->gameConfig.header.rowCount);

		Matrix_u_copy(&tempmxu, &mxu);
#ifdef _IMHERE
		uint8_t mxudata[GE_WheelChessMaxNum] =
		{
		   1,0,2,1,1,
		   4,3,0,3,7,
		   0,1,0,7,0
		};
		Matrix_u_setIntData(&tempmxu, inst->gameConfig, mxudata);
		uint8_t temp[GE_WheelChessMaxNum] =
		{
		   1,0,1,0,1,
		   0,1,0,0,0,
		   0,1,1,0,0
		};
		for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; ++i)
		{
			info->WildPosArray[index][i] = temp[i];
		}
#endif
		// 填入 wild 位置
		for (int32_t i = 0; i < inst->gameConfig.header.wheelChessNum; i++)
		{
			if (info->WildPosArray[index][i] == 1)
			{
				tempmxu.dataArray[i] = inst->gameConfig.header.Wild;
			}
		}

		int32_t matrixBet = Matrix_u_computerMatrixById(&tempmxu, idVec, &inst->gameConfig, gameId, info);
		info->nFreeBet += matrixBet;

		Matrix_u_copy(&info->pFreeMxu[index], &mxu);
		info->FreeBetArray[index] = matrixBet;

		if (matrixBet > 0)
		{
			for (int32_t i = 0; i < tempmxu.idVecSize; ++i)
			{
				info->FreeIDVec[index][i] = idVec[i];
			}
		}

		// 收集 wild 位置（最后一把不收集）
		if (index < info->nFreeNum - 1)
		{
			for (uint8_t i = 0; i < inst->gameConfig.header.wheelChessNum; i++)
			{
				if (Matrix_u_getIntData(&tempmxu, inst->gameConfig, i) == inst->gameConfig.header.Wild)
				{
					info->WildPosArray[index + 1][i] = 1;
				}
			}
		}

		info->pFreeMxu[index].idVecSize = tempmxu.idVecSize;
		info->pFreeMxu[index].resultType = tempmxu.resultType;
	}
}

void GameResult_3998_ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec)
{
	{
		int32_t jpTotal = 0;

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
			for (uint8_t i = 0; i < Mxu->idVecSize; ++i)
			{
				pRes->IDVec[i] = info->FreeIDVec[0][i];
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
			//失败结果
		}
	}
}

int8_t* GameResult_3998_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst)
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

	if (outRes->openType == OT_Give)
	{
		int8_t* wildDataStr = ByteArrayToString(outRes->WildPosArray, curwheelChessNum);
		append_format(strRes, 2048, &used, "\"WildPosArray\":%s,", wildDataStr ? (const char*)wildDataStr : "[]");
		free(wildDataStr);
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
		append_format(strRes, 2048, &used, "\"BonusType\":%d,", outRes->nBonusType);
		switch (outRes->nBonusType)
		{
			//wild
		case 0:
		{
			// wild 模式依赖 3~6 个 bonus 触发，先夹断避免数组越界。
			if (bonusCount < 3) bonusCount = 3;
			if (bonusCount > 6) bonusCount = 6;
			bonusStr = ArrayU16ToString(outRes->BonusData, wildColCountArray[bonusCount - 3], 1);
			append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
			free(bonusStr);
		}
		break;
		//神秘
		case 1:
		{
			bonusStr = ArrayU16ToString(outRes->BonusData, curwheelChessNum, 1);
			append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
			append_format(strRes, 2048, &used, "\"BlindSymbol\":%d,", outRes->BlindSymbol);
			free(bonusStr);
		}
		break;
		//乘数
		case 2:
		{
			append_format(strRes, 2048, &used, "\"BonusMultiply\":%d,", outRes->BlindSymbol);
		}
		break;
		//大奖
		case 3:
		{
			append_format(strRes, 2048, &used, "\"BonusMultiply\":%d,", outRes->BlindSymbol);
		}
		break;
		}

	}

	if (outRes->resType == RT_Jackpot)
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

void GameResult_3998_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId)
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
		res[pos++] = outRes->nTotalFreeBet;						// nTotalFreeBet
		//FreeBetArray
		for (int8_t i = 0; i < outRes->nTotalFreeTime; i++)
		{
			res[pos++] = outRes->FreeBetArray[i];
		}
	}

	if (outRes->openType == OT_Give)
	{
		for (int8_t i = 0; i < wheelChessNum; i++)
		{
			res[pos++] = outRes->WildPosArray[i];
		}
	}

	if (outRes->resType == RT_BonusWin)
	{
		res[pos++] = outRes->nBonusBet;				// nBonusBet
		res[pos++] = outRes->nBonusType;			// nBonusType
		res[pos++] = outRes->BlindSymbol;			//当大奖为神秘游戏就是BlindSymbol ,乘数游戏或者彩金游戏时就是BonusMultiply
		for (int8_t i = 0; i < wheelChessNum; i++)
		{
			res[pos++] = outRes->BonusData[i];
		}
	}

	if (outRes->resType == RT_Jackpot)
	{
		res[pos++] = outRes->nJPCount;
		for (int8_t i = 0; i < GAME_Local_JP_MAX; ++i)
		{
			res[pos++] = outRes->JPTypeArray[i];
		}
		for (int8_t i = 0; i < GAME_Local_JP_MAX; ++i)
		{
			res[pos++] = outRes->JPBetArray[i];
		}
		res[pos++] = outRes->nTotalJackpotBet;
	}
}