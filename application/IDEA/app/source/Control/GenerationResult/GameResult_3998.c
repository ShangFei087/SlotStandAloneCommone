#include "GameResultRegistry.h"
#include <Control/CommonStruct.h>
#include <Control/GameManager.h>

void GameResult_3998_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId)
{
	GameResult_Generic_Normal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

void GameResult_3998_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId)
{
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}

void GameResult_3998_GenBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId)
{
	(void)betVal;

	if (info == NULL || inst == NULL || bonusMxu == NULL) return;

	Matrix_u mxu;
	Matrix_u_reset(&mxu);

	int32_t idVec[GE_MaxIDNum] = { 0 };
	uint8_t bonusCount = Matrix_u_getTypeNum(bonusMxu, inst->gameConfig.header.Bonus);
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

			if (bonusCount >= 6)
			{
				int a = 0;
				(void)a;
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
			info->nBonusBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId);

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
				for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
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

				for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
				{
					if (info->BonusData[i] == 1)
					{
						mxu.dataArray[i] = BlindSymbol;
					}
				}

#ifdef _IMHERE
				Matrix_u_copy(&mxu, bonusMxu);
				BlindSymbol = 0;
				uint8_t temp[GE_WheelChessMaxNum] =
				{
				   0,0,1,0,0,1,0,1,1,0,0,1,0,0,0
				};
				for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
				{
					info->BonusData[i] = temp[i];
					if (temp[i] > 0)
					{
						mxu.dataArray[i] = BlindSymbol;
					}
				}
#endif // _IMHERE

				info->nBonusBet = Matrix_u_computerMatrixById(&mxu, &idVec, &inst->gameConfig, (uint32_t)gameId);
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

	if (randNum < 2500)
	{
		// Wild 游戏功能
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

		if (bonusCount >= 6)
		{
			int a = 0;
			(void)a;
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
		info->nBonusBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId);

		bonusMxu->idVecSize = mxu.idVecSize;
		for (uint8_t i = 0; i < mxu.idVecSize; ++i)
		{
			info->FreeIDVec[0][i] = idVec[i];
		}
	}
	else
	{
		if (randNum < 5000)
		{
			// 神秘游戏
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
			for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
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

			info->nBonusBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId);
			bonusMxu->idVecSize = mxu.idVecSize;
			for (uint8_t i = 0; i < mxu.idVecSize; ++i)
			{
				info->FreeIDVec[0][i] = idVec[i];
			}
		}
		else
		{
			if (randNum < 7500)
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

	int32_t idVec[GE_MaxIDNum] = { 0 };

	// 3998 免费：根据 Bonus 触发位置计算免费次数
	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig.header.Bonus);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	// 收集 wild 位置（来自触发矩阵 freeMxu 上的 Bonus 位）
	for (uint8_t i = 0; i < GE_WheelChessMaxNum; i++)
	{
		if (Matrix_u_getIntData(freeMxu, i) == inst->gameConfig.header.Bonus)
		{
			info->WildPosArray[0][i] = 1;
		}
	}

	for (uint8_t index = 0; index < info->nFreeNum; index++)
	{
		Matrix_u_reset(&mxu);
		Matrix_u_reset(&tempmxu);
		NatureAlg_GenRndMxu(inst->gameConfig.header.freeRollTableId, &mxu);

		Matrix_u_copy(&tempmxu, &mxu);

		// 填入 wild 位置
		for (int32_t i = 0; i < GE_WheelChessMaxNum; i++)
		{
			if (info->WildPosArray[index][i] == 1)
			{
				tempmxu.dataArray[i] = inst->gameConfig.header.Wild;
			}
		}

		int32_t matrixBet = Matrix_u_computerMatrixById(&tempmxu, idVec, &inst->gameConfig, gameId);
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
			for (uint8_t i = 0; i < GE_WheelChessMaxNum; i++)
			{
				if (Matrix_u_getIntData(&tempmxu, i) == inst->gameConfig.header.Wild)
				{
					info->WildPosArray[index + 1][i] = 1;
				}
			}
		}

		info->pFreeMxu[index].idVecSize = tempmxu.idVecSize;
		info->pFreeMxu[index].resultType = tempmxu.resultType;
	}
}
