#include "GenerationResult.h"

#include "GameResultRegistry.h"

void GenerationResult_GenerateNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,int32_t* idVec,GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->genNormal == NULL)
	{
		return;
	}

	// 1) 生成“普通候选矩阵”：计算 mxu->resultType 与 matrixBet/idVec
	ops->genNormal(info, inst, mxu, betVal, matrixBet, idVec, gameId);

	// 2) 根据候选类型生成子局（Free / Bonus）
	if (mxu == NULL) return;

	switch (mxu->resultType)
	{
	case RT_Lose:
	{
		if (ops->genNormalLose)
		{
			ops->genNormalLose(info, inst, mxu, betVal, matrixBet, idVec, gameId);
		}
	}
	break;
	case RT_Win:
	{
		if (ops->genNormalWin)
		{
			ops->genNormalWin(info, inst, mxu, betVal, matrixBet, idVec, gameId);
		}
	}
	break;
	case RT_FreeWin:
	{
		if (ops->genFree) ops->genFree(info, betVal, inst, mxu, gameId);
	}
	break;
	case RT_BonusWin:
	{
		if (ops->genBonus) ops->genBonus(info, betVal, inst, mxu, gameId);
	}
	break;
	default:
	{
	}
	break;
	}
}

void GenerationResult_GenerateFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->genFree == NULL) return;
	ops->genFree(info, betVal, inst, freeMxu, gameId);
}

void GenerationResult_GenerateBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->genBonus == NULL) return;
	ops->genBonus(info, betVal, inst, bonusMxu, gameId);
}

void GenerationResult_GenerateLose(GameInstance_t* inst,Matrix_u* loseMxu,int32_t* idVec,GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->genLose == NULL) return;
	ops->genLose(inst, loseMxu, idVec, gameId);
}

