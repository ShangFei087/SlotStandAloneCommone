#include "GameResultRegistry.h"

#include <string.h>

//------------------------------------------外部调用------------------------------------------//
//根据矩阵的结果类型，生成不同结果
void GenerationResult_GenerateNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	GameResultOps_t* ops = GameResultRegistry_Get(gameId);
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
	case RT_Jackpot:
	{
		if (ops->genJackpot) ops->genJackpot(info, betVal, inst, mxu, gameId);
	}
	break;
	default:
	{
	}
	break;
	}
}
//调试模式生成免费结果
void GenerationResult_GenerateFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->genFree == NULL) return;
	ops->genFree(info, betVal, inst, freeMxu, gameId);
}
//调试模式生成大奖结果
void GenerationResult_GenerateBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->genBonus == NULL) return;
	ops->genBonus(info, betVal, inst, bonusMxu, gameId);
}
void GenerationResult_GenerateJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->genJackpot == NULL) return;
	ops->genJackpot(info, betVal, inst, bonusMxu, gameId);
}
//输
void GenerationResult_GenerateLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->genLose == NULL) return;
	ops->genLose(inst, loseMxu, idVec, gameId);
}
//应用一局结果到OutRes
void GenerationResult_ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec, GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->applyRound == NULL) return;
	ops->applyRound(pRes, resType, info, Mxu, idVec);
}
//应用免费结果到OutRes
void GenerationResult_ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx, GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->applyFree == NULL) return;
	ops->applyFree(pRes, info, freeIdx);
}
//控制台输出结果
int8_t* GenerationResult_OutResToJson(OutResult_t* outRes, GameInstance_t* inst, GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->outResJson == NULL || outRes == NULL || inst == NULL) return NULL;
	return ops->outResJson(outRes, inst);
}
//嵌入式输出结果
void GenerationResult_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId)
{
	const GameResultOps_t* ops = GameResultRegistry_Get(gameId);
	if (ops == NULL || ops->outResSenv == NULL) return;
	ops->outResSenv(outRes, inst, res, gameId);
}
//--------------------------------------------公共--------------------------------------------//
// 生成阵列
void GameResult_Generic_Normal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	if (inst == NULL || mxu == NULL || matrixBet == NULL || idVec == NULL) return;

	NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, mxu, inst->gameConfig.header.rowCount);
	*matrixBet = Matrix_u_computerMatrixById(mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info);
}
//正常赢
void GameResult_Generic_NormalWin(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	(void)info; (void)inst; (void)mxu; (void)betVal; (void)matrixBet; (void)idVec; (void)gameId;
}
//正常输
void GameResult_Generic_NormalLose(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	(void)info; (void)inst; (void)mxu; (void)betVal; (void)matrixBet; (void)idVec; (void)gameId;
}
//强制输
void GameResult_Generic_Lose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId)
{
	if (inst == NULL || loseMxu == NULL || idVec == NULL) return;

	while (1)
	{
		Matrix_u_reset(loseMxu);
		NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, loseMxu, inst->gameConfig.header.rowCount);
		Matrix_u_computerMatrixById(loseMxu, idVec, &inst->gameConfig, (uint32_t)gameId, NULL);
		if (loseMxu->resultType == RT_Lose)
		{
			break;
		}
	}
}
//应用输出结果
void GameResult_Generic_ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec)
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
			pRes->nTotalJackpotBet = pRes->JPBetArray[i];
		}
	}
	else
	{
		//失败结果
	}
}
//应用免费结果
void GameResult_Generic_ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx)
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
	pRes->resType = resType;
}
//通用控制台输出结果
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

int8_t* ArrayU16ToString(uint16_t* pArray, int32_t length, uint16_t keepZero)
{
	char* str = (char*)malloc(1024);
	size_t used = 0;

	if (str == NULL) return NULL;

	str[0] = '\0';
	append_format(str, 1024, &used, "[");

	for (int32_t i = 0; i < length; ++i)
	{
		if (keepZero || pArray[i] != 0)
		{
			// 统一以 int32_t 打印，避免不同平台对 uint16 的格式不一致
			append_format(str, 1024, &used, "%d,", (int32_t)pArray[i]);
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
int8_t* OutResToJson_Common(OutResult_t* outRes,GameInstance_t* inst)
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
	int8_t* wildStr;
	if (outRes->openType == OT_Give)
	{

	}

	uint8_t bonusCount = 0;
	uint8_t wildColCountArray[4] = { 1, 2, 3,3 };//3个转盘图标可以得到1列wild图标，4个转盘可以的2列......
	int8_t* bonusStr;

	if (outRes->resType == RT_BonusWin)
	{
		append_format(strRes, 2048, &used, "\"BonusType\":%d,", outRes->nBonusType);
		append_format(strRes, 2048, &used, "\"BonusBet\":%d,", outRes->nBonusBet);
		bonusStr = ArrayU16ToString(outRes->BonusData, curwheelChessNum, 1);
		append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
		free(bonusStr);

		//中了彩金
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
	}

	append_format(strRes, 2048, &used, "\"TotalBet\":%d", outRes->nMatrixBet);

	append_format(strRes, 2048, &used, "}");

	free(idVecStr);
	free(matrixStr);
	return (int8_t*)strRes;
}
//通用嵌入式输出结果
void OutResToSenv_Common(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId)
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

		res[pos++]=outRes->nJPCount;
		for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
		{
			res[pos++] = outRes->JPTypeArray[i];
		}
		for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
		{
			res[pos++] = outRes->JPBetArray[i] = 0;;
		}
		res[pos++] = outRes->nTotalJackpotBet;				// nTotalJackpotBet
	}
}


//--------------------------------------------解耦--------------------------------------------//
void GameResult_1700_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,uint16_t* idVec,GameInstanceId_t gameId);
void GameResult_1700_GenFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId);
void GameResult_1700_GenBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId);
void GameResult_1700_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId);
void GameResult_1700_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,uint16_t* idVec,GameInstanceId_t gameId);
void GameResult_1700_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId);
//--------------------------------------------财富类--------------------------------------------//
void GameResult_3999_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,uint16_t* idVec,GameInstanceId_t gameId);
void GameResult_3999_GenFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId);
void GameResult_3999_GenBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId);
void GameResult_3999_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId);
void GameResult_3999_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,uint16_t* idVec,GameInstanceId_t gameId);
int8_t* GameResult_3999_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst);
void GameResult_3999_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId);

void GameResult_3998_GenNormal(RoundInfo_t* info,GameInstance_t* inst,Matrix_u* mxu,int32_t betVal,int32_t* matrixBet,uint16_t* idVec,GameInstanceId_t gameId);
void GameResult_3998_GenFree(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* freeMxu,GameInstanceId_t gameId);
void GameResult_3998_GenBonus(RoundInfo_t* info,int32_t betVal,GameInstance_t* inst,Matrix_u* bonusMxu,GameInstanceId_t gameId);
void GameResult_3998_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId);
void GameResult_3998_GenLose(GameInstance_t* inst,Matrix_u* loseMxu,uint16_t* idVec,GameInstanceId_t gameId);
void GameResult_3998_ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec);
int8_t* GameResult_3998_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst);
void GameResult_3998_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId);

void GameResult_3997_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId);
void GameResult_3997_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId);
void GameResult_3997_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId);
void GameResult_3997_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId);
void GameResult_3997_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId);
void GameResult_3997_ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec);
void GameResult_3997_ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx);
int8_t* GameResult_3997_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst);
void GameResult_3997_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId);

void GameResult_3996_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId);
void GameResult_3996_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId);
void GameResult_3996_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId);
void GameResult_3996_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId);
void GameResult_3996_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId);
void GameResult_3996_ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec);
void GameResult_3996_ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx);
int8_t* GameResult_3996_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst);
void GameResult_3996_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId);
//--------------------------------------------动物类--------------------------------------------//
void GameResult_3995_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId);
void GameResult_3995_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId);
void GameResult_3995_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId);
void GameResult_3995_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId);
void GameResult_3995_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId);
void GameResult_3995_ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec);
int8_t* GameResult_3995_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst);
void GameResult_3995_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId);

void GameResult_3993_GenNormal(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId);
void GameResult_3993_GenFree(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* freeMxu, GameInstanceId_t gameId);
void GameResult_3993_GenBonus(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* bonusMxu, GameInstanceId_t gameId);
void GameResult_3993_GenJackpot(RoundInfo_t* info, int32_t betVal, GameInstance_t* inst, Matrix_u* jackpotMxu, GameInstanceId_t gameId);
void GameResult_3993_GenLose(GameInstance_t* inst, Matrix_u* loseMxu, uint16_t* idVec, GameInstanceId_t gameId);
int8_t* GameResult_3993_OutResToJsonn(OutResult_t* outRes, GameInstance_t* inst);
void GameResult_3993_OutResToSenv(OutResult_t* outRes, GameInstance_t* inst, int32_t* res, GameInstanceId_t gameId);


typedef struct
{
	GameInstanceId_t gameId;
	GameResultOps_t ops;
} GameResultRegEntry_t;

static GameResultRegEntry_t gReg[GAME_INSTANCE_ID_MAX];
static int32_t gRegCount = 0;

int8_t GameResultRegistry_Register(GameInstanceId_t gameId, const GameResultOps_t* ops)
{
	if (ops == NULL) return 0;
	if (gameId == GAME_ID_INVALID) return 0;

	// 已存在则覆盖
	for (int32_t i = 0; i < gRegCount; ++i)
	{
		if (gReg[i].gameId == gameId)
		{
			gReg[i].ops = *ops;
			return 1;
		}
	}

	if (gRegCount > GAME_INSTANCE_ID_MAX) return 0;

	gReg[gRegCount].gameId = gameId;
	gReg[gRegCount].ops = *ops;
	gRegCount++;
	return 1;
}

const GameResultOps_t* GameResultRegistry_Get(GameInstanceId_t gameId)
{
	if (gameId == GAME_ID_INVALID) return NULL;
	for (int32_t i = 0; i < gRegCount; ++i)
	{
		if (gReg[i].gameId == gameId)
		{
			return &gReg[i].ops;
		}
	}
	return NULL;
}

void GameResultRegistry_InitDefaults(void)
{
	memset(gReg, 0, sizeof(gReg));
	gRegCount = 0;

	//GameResultOps_t ops1700 = {0};
	//ops1700.genNormal = GameResult_1700_GenNormal;
	//ops1700.genNormalWin = GameResult_Generic_NormalWin;
	//ops1700.genNormalLose = GameResult_Generic_NormalLose;
	//ops1700.genFree = GameResult_1700_GenFree;
	//ops1700.genBonus = GameResult_1700_GenBonus;
	//ops1700.genJackpot = GameResult_1700_GenJackpot;
	//ops1700.genLose = GameResult_1700_GenLose;
	//ops1700.applyRound = GameResult_Generic_ApplyMatrixToOutResByRound;
	//ops1700.applyFree = GameResult_Generic_ApplyMatrixToOutResForFree;
	//ops1700.outResJson = OutResToJson_Common;
	//ops1700.outResSenv = GameResult_1700_OutResToSenv;
	//(void)GameResultRegistry_Register(1700, &ops1700);

	GameResultOps_t ops3999 = {0};
	ops3999.genNormal = GameResult_3999_GenNormal;
	ops3999.genNormalWin = GameResult_Generic_NormalWin;
	ops3999.genNormalLose = GameResult_Generic_NormalLose;
	ops3999.genFree = GameResult_3999_GenFree;
	ops3999.genBonus = GameResult_3999_GenBonus;
	ops3999.genJackpot = GameResult_3999_GenJackpot;
	ops3999.genLose = GameResult_3999_GenLose;
	ops3999.applyRound = GameResult_Generic_ApplyMatrixToOutResByRound;
	ops3999.applyFree = GameResult_Generic_ApplyMatrixToOutResForFree;
	ops3999.outResJson = GameResult_3999_OutResToJsonn;
	ops3999.outResSenv = GameResult_3999_OutResToSenv;
	(void)GameResultRegistry_Register(3999, &ops3999);

	GameResultOps_t ops3998 = {0};
	ops3998.genNormal = GameResult_3998_GenNormal;
	ops3998.genNormalWin = GameResult_Generic_NormalWin;
	ops3998.genNormalLose = GameResult_Generic_NormalLose;
	ops3998.genFree = GameResult_3998_GenFree;
	ops3998.genBonus = GameResult_3998_GenBonus;
	ops3998.genJackpot = GameResult_3998_GenJackpot;
	ops3998.genLose = GameResult_3998_GenLose;
	ops3998.applyRound = GameResult_3998_ApplyMatrixToOutResByRound;
	ops3998.applyFree = GameResult_Generic_ApplyMatrixToOutResForFree;
	ops3998.outResJson = GameResult_3998_OutResToJsonn;
	ops3998.outResSenv = GameResult_3998_OutResToSenv;
	(void)GameResultRegistry_Register(3998, &ops3998);

	GameResultOps_t ops3997 = { 0 };
	ops3997.genNormal = GameResult_3997_GenNormal;
	ops3997.genNormalWin = GameResult_Generic_NormalWin;
	ops3997.genNormalLose = GameResult_Generic_NormalLose;
	ops3997.genFree = GameResult_3997_GenFree;
	ops3997.genBonus = GameResult_3997_GenBonus;
	ops3997.genJackpot = GameResult_3997_GenJackpot;
	ops3997.genLose = GameResult_3997_GenLose;
	ops3997.applyRound = GameResult_3997_ApplyMatrixToOutResByRound;
	ops3997.applyFree = GameResult_3997_ApplyMatrixToOutResForFree;
	ops3997.outResJson = GameResult_3997_OutResToJsonn;
	ops3997.outResSenv = GameResult_3997_OutResToSenv;
	(void)GameResultRegistry_Register(3997, &ops3997);

	GameResultOps_t ops3996 = { 0 };
	ops3996.genNormal = GameResult_3996_GenNormal;
	ops3996.genNormalWin = GameResult_Generic_NormalWin;
	ops3996.genNormalLose = GameResult_Generic_NormalLose;
	ops3996.genFree = GameResult_3996_GenFree;
	ops3996.genBonus = GameResult_3996_GenBonus;
	ops3996.genJackpot = GameResult_3996_GenJackpot;
	ops3996.genLose = GameResult_3996_GenLose;
	ops3996.applyRound = GameResult_3996_ApplyMatrixToOutResByRound;
	ops3996.applyFree = GameResult_3996_ApplyMatrixToOutResForFree;
	ops3996.outResJson = GameResult_3996_OutResToJsonn;
	ops3996.outResSenv = GameResult_3996_OutResToSenv;
	(void)GameResultRegistry_Register(3996, &ops3996);


	GameResultOps_t ops3995 = { 0 };
	ops3995.genNormal = GameResult_3995_GenNormal;
	ops3995.genNormalWin = GameResult_Generic_NormalWin;
	ops3995.genNormalLose = GameResult_Generic_NormalLose;
	ops3995.genFree = GameResult_3995_GenFree;
	ops3995.genBonus = GameResult_3995_GenBonus;
	ops3995.genJackpot = GameResult_3995_GenJackpot;
	ops3995.genLose = GameResult_3995_GenLose;
	ops3995.applyRound = GameResult_3995_ApplyMatrixToOutResByRound;
	ops3995.applyFree = GameResult_Generic_ApplyMatrixToOutResForFree;
	ops3995.outResJson = GameResult_3995_OutResToJsonn;
	ops3995.outResSenv = GameResult_3995_OutResToSenv;
	(void)GameResultRegistry_Register(3995, &ops3995);

	GameResultOps_t ops3993 = { 0 };
	ops3993.genNormal = GameResult_3993_GenNormal;
	ops3993.genNormalWin = GameResult_Generic_NormalWin;
	ops3993.genNormalLose = GameResult_Generic_NormalLose;
	ops3993.genFree = GameResult_3993_GenFree;
	ops3993.genBonus = GameResult_3993_GenBonus;
	ops3993.genJackpot = GameResult_3993_GenJackpot;
	ops3993.genLose = GameResult_3993_GenLose;
	ops3993.applyRound = GameResult_Generic_ApplyMatrixToOutResByRound;
	ops3993.applyFree = GameResult_Generic_ApplyMatrixToOutResForFree;
	ops3993.outResJson = GameResult_3993_OutResToJsonn;
	ops3993.outResSenv = GameResult_3993_OutResToSenv;
	(void)GameResultRegistry_Register(3993, &ops3993);
}

