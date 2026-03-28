#include "GameResultRegistry.h"
#include <Control/CommonStruct.h>
#include <Control/GameManager.h>

void GameResult_3995_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, int32_t* idVec, GameInstanceId_t gameId)
{
	// 当前与通用逻辑一致
	GameResult_Generic_Normal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

void GameResult_3995_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, int32_t* idVec, GameInstanceId_t gameId)
{
	// 当前与通用逻辑一致，预留每游戏独立扩展点
	GameResult_Generic_Lose(inst, loseMxu, idVec, gameId);
}
// 公牛替换图标
void ReplaceSymbol(Matrix_u* pMatrix, uint8_t type)
{
	for (int8_t i = 0; i < GE_WheelChessMaxNum; ++i)
	{
		if (pMatrix->dataArray[i] == type)
		{
			pMatrix->dataArray[i] = 9;
		}
	}
}
//生成一局免费奖
void GameResult_3995_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId)
{
	if (info == NULL || inst == NULL || freeMxu == NULL) return;

	Matrix_u mxu;
	Matrix_u_reset(&mxu);
	int32_t idVec[GE_MaxIDNum] = { 0 };

	uint8_t scatterCount = Matrix_u_getTypeNum(freeMxu, inst->gameConfig.header.Scatter);
	info->nFreeNum = GET_FREE_TIME(inst->gameConfig.header.id, scatterCount - 3);
	info->nFreeBet = 0;

	int8_t wildMultipliedArray[3] = { 2,3,5 }; //wild倍数数组
	int8_t wildMultiplied = 0;//wild倍数
	int8_t wildCount = 0; //wild图标数量
	int8_t wildindex = 0; //wild下标
	int8_t blackPantherCount = 0; //公牛图标数量
	int8_t BPCollectArray[4] = { 4,10,18,28 }; //公牛图标收集区间数组
	int8_t symbolChangeArray[4] = { 5,6,7,8 }; //公牛替换图标的数组
	for (uint8_t index = 0; index < info->nFreeNum; index++)
	{
		Matrix_u_reset(&mxu);
		NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, &mxu);

		//统计黑豹图标数量
		blackPantherCount += Matrix_u_getTypeNum(&mxu, 9);
		//需要黑豹替换图标的区间
		for (int8_t i = 3; i >= 0; --i)
		{
			if (blackPantherCount >= BPCollectArray[i])
			{
				//黑豹替换图标
				for (int8_t j = 0; j <= i; ++j)
				{
					ReplaceSymbol(&mxu, symbolChangeArray[j]);
				}
			}
		}

		blackPantherCount += Matrix_u_getTypeNum(&mxu, inst->gameConfig.header.Wild);
		for (uint32_t i = 0; i < GE_WheelChessMaxNum; i++)
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
void GameResult_3995_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId)
{
	if (info == NULL) return;

	int dataArray[GE_WheelChessMaxNum] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 };
	uint8_t PosVec[GE_WheelChessMaxNum];
	uint8_t PosSize = GE_WheelChessMaxNum;

	// 复制原始数组
	for (uint8_t i = 0; i < PosSize; i++)
	{
		PosVec[i] = dataArray[i];
	}

	uint8_t randNum = 0;
	uint8_t randPos = 0;

	// 先填满bonus位置
	for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
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
		insertCount = JRandFrom(1, PosSize - 1);
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