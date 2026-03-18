#include "DllInterface.h"

#include "ComputerData.h"
#include "CMD_Fish.h"
#include "Test.h"
#include "../GameAlgo/common/JRand.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
GameInstance_t* g_CurrentGameInstance = NULL;  // 定义并初始化

GameInstance_t* get_instance(GameId_t gameId)
{
	if (gameId == GAME_ID_INVALID)
	{
		if (!g_CurrentGameInstance)
		{
			g_CurrentGameInstance = GameManager_GetCurrentInstance();
		}
		return g_CurrentGameInstance;
	}
	return GameManager_GetInstance(gameId);
}

static void append_format(char* buffer, size_t buffer_size, size_t* used, const char* format, ...)
{
	va_list args;
	int32_t written = 0;
	size_t remain = 0;

	// 统一的安全追加：所有 JSON/日志字符串都走这里，防止越界写入。
	if (buffer == NULL || used == NULL || format == NULL || *used >= buffer_size) return;

	remain = buffer_size - *used;
	va_start(args, format);
	written = vsnprintf(buffer + *used, remain, format, args);
	va_end(args);

	if (written < 0)
	{
		buffer[buffer_size - 1] = '\0';
		*used = buffer_size - 1;
		return;
	}

	if ((size_t)written >= remain)
	{
		buffer[buffer_size - 1] = '\0';
		*used = buffer_size - 1;
		return;
	}

	*used += (size_t)written;
}

GameId_t DLL_GameInit(int8_t* gameName, GameId_t gameId)
{

	GameManager_Init();
	GameId_t id = GameManager_RegisterGame(gameName, gameId);
	g_CurrentGameInstance = GameManager_GetCurrentInstance();
	return id;
}

int8_t DLL_GameSwitch(GameId_t gameId)
{
	int8_t ok = GameManager_SwitchGame(gameId);
	if (ok) g_CurrentGameInstance = GameManager_GetCurrentInstance();
	return ok;
}

BOOL DLL_GameUnregister(GameId_t gameId)
{
	return GameManager_UnregisterGame(gameId);
}

GameId_t DLL_GetCurrentGameId(void)
{
	return GameManager_GetCurrentGameId();
}

//应用RoundInfo到输出结果
void ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu)
{
	// pRes->nMatrixBet = 0;
	 //pRes->nTotalFreeBet = 0;
	pRes->resType = resType;

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
		for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
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
	else
	{
		//失败结果
	}
}
//生成一个免费局
void GenerateARoundWithFreeWinById(RoundInfo_t* info, GameId_t gameId, Matrix_u* freeMxu)
{
	Matrix_u mxu;
	Matrix_u_reset(&mxu);
	int32_t idVec[GE_MaxIDNum] = { 0 };
	GameInstance_t* inst = get_instance(gameId);

	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig.header.Scatter);

	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	switch (gameId)
	{
	case 1700:
		for (uint8_t index = 0; index < info->nFreeNum; index++)
		{
			Matrix_u_reset(&mxu);
			NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu);
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
			info->pFreeMxu[index].idVecSize = mxu.idVecSize;
			info->pFreeMxu[index].resultType = mxu.resultType;
		}
		break;
	case 3999:
		for (uint8_t index = 0; index < info->nFreeNum; index++)
		{
			while (1)
			{
				Matrix_u_reset(&mxu);
				NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu);
				//随机一列填满wild
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

				uint8_t scatterCount = Matrix_u_getTypeNum(&mxu, inst->gameConfig.header.Scatter);
				if (info->nFreeNum + scatterCount <= 20)
				{
					info->nFreeNum += scatterCount;
					break;
				}
				continue;
			}

			info->pFreeMxu[index].idVecSize = mxu.idVecSize;
			info->pFreeMxu[index].resultType = mxu.resultType;
		}
		break;
	case 3998:
		scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig.header.Bonus);
		info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
		Matrix_u tempmxu;
		Matrix_u_reset(&tempmxu);
		//收集wild位置
		for (uint8_t i = 0; i < GE_WheelChessNum; i++)
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
			//填入wild位置
			for (int32_t i = 0; i < GE_WheelChessNum; i++)
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

			//收集wild位置,最后一把不收集
			if (index < info->nFreeNum - 1)
			{
				for (uint8_t i = 0; i < GE_WheelChessNum; i++)
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
		break;
	}


}
//生成一个大奖局
void GenerateARoundWithBonusWinById(RoundInfo_t* info, GameId_t gameId, Matrix_u* bonusMxu)
{
	Matrix_u mxu;
	Matrix_u_reset(&mxu);
	int32_t idVec[GE_MaxIDNum] = { 0 };
	GameInstance_t* inst = get_instance(gameId);

	uint8_t bonusCount = Matrix_u_getTypeNum(bonusMxu, inst->gameConfig.header.Bonus);
	Matrix_u_copy(&mxu, bonusMxu);
	info->nBonusBet = 0;
	uint32_t randNum = 0;
	uint8_t bonusMulpitly = 0;
	uint8_t bonusMulpitlyArray[4] = { 25,50,100,200 };
	switch (gameId)
	{
	case 1700:
		info->nBonusBet = 1000;
		break;
	case 3999:
		randNum = JRandFrom(0, 3);
		bonusMulpitly = bonusMulpitlyArray[randNum];
		info->nBonusBet = inst->gameConfig.header.lineCount * bonusMulpitly;
		info->nBonusType = randNum;
		break;
	case 3998:
		//随机大奖
		randNum = JRandFrom(0, 10000);
#ifdef _DebugControlMode
		if (gDebugControlMode.mode == DCM_PointResData)
		{
			if (gDebugControlMode.bonusType == 0)
			{
				//Wild游戏功能
				info->nBonusType = 0;
				uint8_t wildColCountArray[4] = { 1, 2, 3,3 };//3个转盘图标可以得到1列wild图标，4个转盘可以的2列......
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
				if (gDebugControlMode.bonusType == 1)
				{
					//神秘游戏
					info->nBonusType = 1;
					uint8_t  BlindSymbol = JRandFrom(0, 7);
					uint8_t BlindBoxCountArray[3] = { 6, 8, 11 };//3个转盘图标可以得到6个盲盒，4个转盘可以的8个盲盒图标......
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
					for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
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
						//// 随机选择一个位置索引
						randNum = JRandFrom(0, PosSize - 1);
						//获取实际的列位置
						randPos = PosVec[randNum];
						info->BonusData[randPos] = 1;
						// 从数组中删除已使用的位置
						for (int j = randNum; j < PosSize - 1; j++)
						{
							PosVec[j] = PosVec[j + 1];
						}
						PosSize--;

					}

					for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
					{
						if (info->BonusData[i] == 1)
						{
							mxu.dataArray[i] = BlindSymbol;
						}
					}
#ifdef _IMHERE
					Matrix_u_copy(&mxu, bonusMxu);
					BlindSymbol = 0;
					uint8_t temp[GE_WheelChessNum] =
					{
					   0,0,1,0,0,1,0,1,1,0,0,1,0,0,0
					};
					for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
					{
						info->BonusData[i] = temp[i];
						if (temp[i] > 0)
						{
							mxu.dataArray[i] = BlindSymbol;
						}
					}

					//0, 0, 6, 5, 0,
					//0, 0, 0, 0, 8,
					//0, 0, 7, 5, 0

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
						//乘数游戏
						info->nBonusType = 2;
						uint8_t WheelArray[3][4] = { { 3, 4, 5, 7 } ,{6,7,8,10},{9,10,11,15} };
						uint8_t WheelMulptiy = WheelArray[bonusCount - 3][JRandFrom(0, 3)];
						info->BlindSymbol = WheelMulptiy; //代替一下
						info->nBonusBet = WheelMulptiy * inst->gameConfig.header.lineCount * bonusCount;
					}
					else
					{
						if (gDebugControlMode.bonusType == 3)
						{
							//大奖游戏
							info->nBonusType = 3;
							randNum = JRandFrom(0, 4);
							bonusMulpitly = bonusMulpitlyArray[randNum];
							info->BlindSymbol = bonusMulpitly; //代替一下
							info->nBonusBet = inst->gameConfig.header.lineCount * bonusMulpitly;
						}

					}
				}
			}

			return;
		}
#endif // _DebugControlMode

		if (randNum < 2500)
		{
			//Wild游戏功能
			info->nBonusType = 0;
			uint8_t wildColCountArray[4] = { 1, 2, 3,3 };//3个转盘图标可以得到1列wild图标，4个转盘可以的2列......
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
				//神秘游戏
				info->nBonusType = 1;
				uint8_t  BlindSymbol = JRandFrom(0, 7);
				uint8_t BlindBoxCountArray[3] = { 6, 8, 11 };//3个转盘图标可以得到6个盲盒，4个转盘可以的8个盲盒图标......
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
				for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
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
					//// 随机选择一个位置索引
					randNum = JRandFrom(0, PosSize - 1);
					//获取实际的列位置
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
					//乘数游戏
					info->nBonusType = 2;
					uint8_t WheelArray[3][4] = { { 3, 4, 5, 7 } ,{6,7,8,10},{9,10,11,15} };
					uint8_t WheelMulptiy = WheelArray[bonusCount - 3][JRandFrom(0, 3)];
					info->BlindSymbol = WheelMulptiy; //代替一下
					info->nBonusBet = WheelMulptiy * inst->gameConfig.header.lineCount * bonusCount;
				}
				else
				{
					//大奖游戏
					info->nBonusType = 3;
					randNum = JRandFrom(0, 4);
					bonusMulpitly = bonusMulpitlyArray[randNum];
					info->BlindSymbol = bonusMulpitly; //代替一下
					info->nBonusBet = inst->gameConfig.header.lineCount * bonusMulpitly;
				}
			}
		}

		break;
	}
}
//为免费游戏应用矩阵
void ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx)
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
	memcpy(pRes->IDVec, info->FreeIDVec[freeIdx], sizeof(pRes->IDVec));
	pRes->resType = resType;
}

int32_t  GetIDBetValue(int32_t ID)
{
	/*if (ID % 100 == 99)
	{
		return gScatterBet[ID / 100];
	}
	int32_t scale = 1;
	if (ID > 100000)scale = ID / 100000;

	ID %= 1000;

	return  GameConfig_GetBet(g_CurrentGameInstance->gameConfig, ID % 100, (ID / 100)-2 ) * scale;*/
	return 0;
}
//请求结果
void GetNormalResult(player_data_item* pUserInfo, int32_t betVal, OutResult_t* outRes, int32_t* ret, GameId_t gameId)
{
	*ret = 4;
	int8_t opType = outRes->openType;
	uint8_t jpType = outRes->JPType;
	int32_t jpBet = outRes->nJPBet;
	OutResult_Init(outRes);
	outRes->JPType = jpType;
	outRes->nJPBet = jpBet;

	GameInstance_t* inst = get_instance(gameId);
	if (!inst) {
		*ret = -1;
		return;
	}

	RoundInfo_t ri;
	RoundInfo_t_Init(&ri);

	if (opType == OT_Give)
	{
		ApplyMatrixToOutResForFree(outRes, &inst->freeGameInfo.roundInfo, inst->freeGameInfo.nCurFreeIdx);
		inst->freeGameInfo.nCurFreeIdx++;
		inst->freeGameInfo.nRemainFreeBet -= outRes->nMatrixBet;
		//如果全部免费局获取完，将剩余免费次数设为0
		if (inst->freeGameInfo.nCurFreeIdx == inst->freeGameInfo.nTotalFreeTime)
		{

		}
		outRes->openType = OT_Give;
		*ret = 5;

		return;
	}
	else
	{
		//累计投注
		 //inst->debugInfo.dwPlayScore+= betVal;
		inst->debugInfo.dwPlayScore += betVal * 50;

		Matrix_u mxu;
		Matrix_u_reset(&mxu);
		int32_t idVec[GE_MaxIDNum] = { 0 };
		int32_t matrixBet = 0;

		NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu);
		//全线计算
		//matrixBet=Matrix_u_computerMatrix_243(mxu, idVec);
		//有线计算
		matrixBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId);

#ifdef _DebugControlMode
		//应用调试模式数据
		if (gDebugControlMode.mode == DCM_PointResData)
		{
			while (1)
			{
				Matrix_u_reset(&mxu);
				NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu);
#ifdef _IMHERE
				uint8_t temp[GE_WheelChessNum] =
				{
				   3,2,9,5,1,9,3,3,9,3,4,6,5,5,4
				};
				Matrix_u_setIntData(&mxu, temp);
#endif // _IMHERE
				//全线计算
				//matrixBet=Matrix_u_computerMatrix_243(mxu, idVec);
				//有线计算
				matrixBet = Matrix_u_computerMatrixById(&mxu, idVec, &inst->gameConfig, (uint32_t)gameId);
				if (mxu.resultType == gDebugControlMode.resType)
				{
					break;
				}
			}
		}
#endif
		ri.resType = mxu.resultType;
		*ret = (int32_t)mxu.resultType;
		switch (mxu.resultType)
		{
		case RT_Lose:
		{
			outRes->resType = RT_Lose;
			Matrix_u_copy(&outRes->matrix, &mxu);
			outRes->nMatrixBet = 0;
			for (uint8_t i = 0; i < mxu.idVecSize; ++i)
			{
				outRes->IDVec[i] = idVec[i];
			}
			*ret = 0;
		}
		break;
		case RT_Win:
		{
			outRes->resType = RT_Win;
			outRes->nMatrixBet = matrixBet;
			Matrix_u_copy(&outRes->matrix, &mxu);
			for (uint8_t i = 0; i < mxu.idVecSize; ++i)
			{
				outRes->IDVec[i] = idVec[i];
			}

			ApplyMatrixToOutResByRound(outRes, RT_Win, &ri, &mxu);

			*ret = 1;
		}
		break;
		case RT_FreeWin:
		{
			outRes->resType = RT_FreeWin;
			outRes->nMatrixBet = matrixBet;
			Matrix_u_copy(&outRes->matrix, &mxu);
			for (uint8_t i = 0; i < mxu.idVecSize; ++i)
			{
				outRes->IDVec[i] = idVec[i];
			}
			//生成免费局数
			GenerateARoundWithFreeWinById(&ri, gameId, &mxu);

			//存储免费信息
			FreeGameInfo_t_Reset(&inst->freeGameInfo);
			RoundInfo_t_Copy(&inst->freeGameInfo.roundInfo, &ri);
			inst->freeGameInfo.nBetVal = betVal;
			inst->freeGameInfo.nCurFreeIdx = 0;
			inst->freeGameInfo.nTotalFreeTime = ri.nFreeNum;
			inst->freeGameInfo.nRemainFreeBet = ri.nFreeBet;



			ApplyMatrixToOutResByRound(outRes, RT_FreeWin, &ri, &mxu);
			*ret = 2;
		}
		break;
		case RT_BonusWin:
		{
			outRes->resType = RT_BonusWin;
			outRes->nMatrixBet = matrixBet;
			Matrix_u_copy(&outRes->matrix, &mxu);
			for (uint8_t i = 0; i < mxu.idVecSize; ++i)
			{
				outRes->IDVec[i] = idVec[i];
			}
			GenerateARoundWithBonusWinById(&ri, gameId, &mxu);
			ApplyMatrixToOutResByRound(outRes, RT_BonusWin, &ri, &mxu);
			*ret = 3;
		}
		break;
		default:
			break;
		}

		outRes->openType = OT_Normal;
		inst->debugInfo.dwWinScore += betVal * (outRes->nMatrixBet + outRes->nTotalFreeBet + outRes->nBonusBet) + outRes->nJPBet;
	}
}

void DLL_GetGameResultById(player_data_item* pUserInfo, int32_t betValue, OutResult_t* outRes, int32_t* ret, GameId_t gameId)
{
	GetNormalResult(pUserInfo, betValue, outRes, ret, gameId);
	GameInstance_t* inst = get_instance(gameId);
	if (!inst) return;


	inst->debugInfo.dwTotalPlayTime++;
	if (outRes->resType == RT_Win) {
		inst->debugInfo.dwNormalWinTime++;
		inst->debugInfo.dwNormalWinTotalBet += outRes->nMatrixBet;
	}
	if (outRes->openType == OT_Normal) inst->debugInfo.dwNormalOpenTime++;
	else inst->debugInfo.dwGiveOpenTime++;
	if (outRes->resType == RT_FreeWin) {
		inst->debugInfo.dwFreeGameTime++;
		inst->debugInfo.dwFreeGameTotalBet += outRes->nTotalFreeBet;
	}
	else if (outRes->resType == RT_BonusWin) {
		inst->debugInfo.dwBonusTime++;
		inst->debugInfo.dwBonusGameTotalBet += outRes->nBonusBet;
	}
	else if (outRes->resType == RT_Lose) {
		inst->debugInfo.dwLooseTime++;
	}
	//彩金
	if (outRes->nJPBet > 0) {
		inst->debugInfo.dwJackpotTime++;
		inst->debugInfo.dwJackpotTotalBet += outRes->nJPBet;
	}
	inst->debugInfo.dwFreeGameBetError = inst->freeGameInfo.nRemainFreeBet;
	//日志
#ifdef LocalDebug
	//int8_t* resTypeNameStrVec[] = { "输", "赢", "免费游戏", "奖池" };
	const char* resTypeNameStrVec[] = { "Lose", "Win", "FreeGame", "BonusGame" };
	const char* resTypeStr = resTypeNameStrVec[outRes->resType];

	//int8_t* optTypeNameStrVec[] = { "普通", "赠送" };
	const char* optTypeNameStrVec[] = { "Normal", "Give" };
	const char* optTypeStr = optTypeNameStrVec[outRes->openType];

	char finalString[1024] = { 0 };
	char resString[512] = { 0 };
	size_t resUsed = 0;
	size_t finalUsed = 0;

	if (outRes->resType == RT_Win) {
		//strcat(resString, "  补偿线：");
		append_format(resString, sizeof(resString), &resUsed, "  IDVec: [");

		for (int32_t j = 0; j < GE_MaxIDNum; ++j) {
			if (outRes->IDVec[j] > 0) {
				int32_t betValue = GetIDBetValue(outRes->IDVec[j]);
				append_format(resString, sizeof(resString), &resUsed, "%d<%d> ", outRes->IDVec[j], betValue);
			}
		}
		append_format(resString, sizeof(resString), &resUsed, "]");
	}

	if (outRes->openType == OT_Give) {

	}

	if (outRes->resType == RT_FreeWin) {
		//sprintf(freeStr, " 免费次数:%d", outRes->nTotalFreeTime);
		append_format(resString, sizeof(resString), &resUsed, " nTotalFreeTime:%d", outRes->nTotalFreeTime);

		//sprintf(freeStr, " 预计总免费注:%d", outRes->nTotalFreeBet);
		append_format(resString, sizeof(resString), &resUsed, " nTotalFreeBet:%d", outRes->nTotalFreeBet);
	}

	if (outRes->resType == RT_BonusWin) {
		//sprintf(bonusStr, " 奖池总注数:%d", outRes->nBonusBet);
		append_format(resString, sizeof(resString), &resUsed, " nBonusBet:%d", outRes->nBonusBet);
	}

	// 拼接最终字符串
	append_format(finalString, sizeof(finalString), &finalUsed, " optTypeStr:%s resTypeStr:%s betValue:%d nMatrixBet:%d ",
		optTypeStr, resTypeStr, betValue, outRes->nMatrixBet);
	int32_t expScore = (int32_t)pUserInfo->Wins + betValue * outRes->nMatrixBet;
	append_format(finalString, sizeof(finalString), &finalUsed, "totalscore:%d expectation:%d %s",
		(int32_t)pUserInfo->Wins, expScore, resString);


	QS_LOG("%s\n", finalString);

	int8_t* jsonStr = OutResToJsonnById(outRes, gameId);
	if (jsonStr != NULL)
	{
		QS_LOG("%s\n", jsonStr);
		free(jsonStr);
	}
	else
	{
		QS_LOG("{\"err\":\"OutResToJsonnById failed\"}\n");
	}

	QS_LOG("-----------------\n");

#endif

#ifdef LocalDebug
	int32_t testTime = 0;
	testTime++;
	if (testTime < _DebugInfoInterval)return;
	testTime = 0;
#endif
}

int8_t* ArrayToString(int32_t* pArray, int32_t length, int32_t keepZero)
{
	char* str = (char*)malloc(1024);
	size_t used = 0;

	if (str == NULL) return NULL;

	str[0] = '\0';
	append_format(str, 1024, &used, "[");

	for (int8_t i = 0; i < length; ++i)
	{
		if (keepZero || pArray[i] != 0)
		{
			append_format(str, 1024, &used, "%d,", pArray[i]);
		}
	}

	if (used > 1 && str[used - 1] == ',')
	{
		str[used - 1] = ']';
	}
	else
	{
		append_format(str, 1024, &used, "]");
	}

	return (int8_t*)str;
}

int8_t* ByteArrayToString(int8_t* pArray, int8_t length)
{
	char* str = (char*)malloc(1024);
	size_t used = 0;

	if (str == NULL) return NULL;

	str[0] = '\0';
	append_format(str, 1024, &used, "[");

	for (int8_t i = 0; i < length; ++i)
	{
		if (pArray[i] != 15)
		{
			append_format(str, 1024, &used, "%d,", pArray[i]);
		}
	}

	if (used > 1 && str[used - 1] == ',')
	{
		str[used - 1] = ']';
	}
	else
	{
		append_format(str, 1024, &used, "]");
	}

	return (int8_t*)str;
}

int8_t* OutResToJsonnById(OutResult_t* outRes, GameId_t gameId)
{
	char* strRes = (char*)malloc(2048);
	size_t used = 0;
	int8_t* idVecStr = NULL;
	int8_t* matrixStr = NULL;

	// 返回堆内存，调用方负责 free；失败返回 NULL。
	if (strRes == NULL || outRes == NULL) return NULL;

	strRes[0] = '\0';
	append_format(strRes, 2048, &used, "{");

	append_format(strRes, 2048, &used, "\"OpenType\":%d,", outRes->openType);
	append_format(strRes, 2048, &used, "\"ResultType\":%d,", outRes->resType);

	idVecStr = ArrayToString((int32_t*)outRes->IDVec, GE_MaxIDNum, 0);
	append_format(strRes, 2048, &used, "\"IDVec\":%s,", idVecStr ? (const char*)idVecStr : "[]");

	matrixStr = ByteArrayToString(outRes->matrix.dataArray, GE_WheelChessNum);
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
		// 注释掉的代码保留原样
		//sprintf(temp, "\"ScatterData\":%s,", ArrayToString((int32_t*)outRes->ScatterData, 3, 1));
		//strcat(strRes, temp);
	}

	GameInstance_t* inst = get_instance(gameId);
	uint8_t bonusCount = 0;
	uint8_t wildColCountArray[4] = { 1, 2, 3,3 };//3个转盘图标可以得到1列wild图标，4个转盘可以的2列......
	int8_t* bonusStr;
	// gameId 失配时给默认值，避免空实例导致访问非法内存。
	if (inst != NULL)
	{
		bonusCount = Matrix_u_getTypeNum(&outRes->matrix, inst->gameConfig.header.Bonus);
	}
	if (outRes->resType == RT_BonusWin)
	{
		switch (gameId)
		{
		case 3998:
			append_format(strRes, 2048, &used, "\"BonusBet\":%d,", outRes->nBonusBet);
			append_format(strRes, 2048, &used, "\"BonusType\":%d,", outRes->nBonusType);
			switch (outRes->nBonusType)
			{
				//wild
			case 0:
				// wild 模式依赖 3~6 个 bonus 触发，先夹断避免数组越界。
				if (bonusCount < 3) bonusCount = 3;
				if (bonusCount > 6) bonusCount = 6;
				bonusStr = ArrayToString((int32_t*)outRes->BonusData, wildColCountArray[bonusCount - 3], 1);
				append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
				free(bonusStr);
				break;
				//神秘
			case 1:
				bonusStr = ArrayToString((int32_t*)outRes->BonusData, GE_WheelChessNum, 1);
				append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
				append_format(strRes, 2048, &used, "\"BlindSymbol\":%d,", outRes->BlindSymbol);
				free(bonusStr);
				break;
				//乘数
			case 2:
				append_format(strRes, 2048, &used, "\"BonusMultiply\":%d,", outRes->BlindSymbol);
				break;
				//大奖
			case 3:
				append_format(strRes, 2048, &used, "\"BonusMultiply\":%d,", outRes->BlindSymbol);
				break;
			}

			break;
		default:
			append_format(strRes, 2048, &used, "\"BonusType\":%d,", outRes->nBonusType);
			append_format(strRes, 2048, &used, "\"BonusBet\":%d,", outRes->nBonusBet);
			break;
		}

	}

	append_format(strRes, 2048, &used, "\"TotalBet\":%d", outRes->nMatrixBet);

	append_format(strRes, 2048, &used, "}");

	free(idVecStr);
	free(matrixStr);
	return (int8_t*)strRes;
}

void DLL_GetUserDebugInfo(DebugInfo* pDebugInfo, GameId_t gameId)
{
	GameInstance_t* inst = get_instance(gameId);
	if (inst) {
		DebugInfo_accum(pDebugInfo, &inst->debugInfo);
	}
}

void DLL_OnBegin(player_data_item* pUserInfo, GameId_t gameId)
{
	(void)pUserInfo;
	(void)gameId;
}

void DLL_OnBeginEx(player_data_item* pUserInfo, int8_t isReconnect, GameId_t gameId)
{
	(void)isReconnect;
	DLL_OnBegin(pUserInfo, gameId);
}

void DLL_GetDebugInfo(DebugInfo* pDebugInfo, GameId_t gameId)
{
	DLL_GetUserDebugInfo(pDebugInfo, gameId);
}

void DLL_DebugClearData(GameId_t gameId)
{
	GameInstance_t* inst = get_instance(gameId);
	if (inst) {
		DebugInfo_reset(&inst->debugInfo);
	}
}

void DLL_GameClose(GameId_t gameId)
{
	GameManager_UnregisterGame(gameId);
}

void DLL_GameCloseAll(void)
{
	GameManager_Cleanup();
}
