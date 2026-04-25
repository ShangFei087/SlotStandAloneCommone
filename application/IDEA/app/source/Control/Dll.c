#include "DllInterface.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ComputerData.h"
#include "GenerationResult/GameResultRegistry.h"
#include "TableControl.h"
#include "LotteryManager.h"
#include "Test.h"
#include "GameManager.h"
#include "../GameAlgo/common/JRand.h"


// 前置声明：避免先调用后定义触发隐式声明告警/错误。
GameInstance_t* get_instance(GameInstanceId_t gameId);
int32_t GetIDBetValue(int32_t ID);

GameInstance_t* g_CurrentGameInstance = NULL;  // 定义并初始化

// 设置区域与 RTP 档位；rtpPermyriad<=0 表示使用区域默认档。
int32_t DLL_SetRtpDifficulty(uint8_t region, int32_t rtpPermyriad)
{
	return TableControl_SetRtpDifficulty(region, rtpPermyriad);
}

// 按等级设置当前区域 RTP 档位（1~5，对应低到高 RTP 档）。
int32_t DLL_SetDifficultyLevel(int32_t level)
{
	return TableControl_SetDifficultyLevel(level);
}

// 调试:设置免费和大奖不同概率配置
void DLL_SetRtpPassOverride(int32_t freePassPermyriad, int32_t bonusPassPermyriad)
{
	TableControl_SetRtpPassOverride(freePassPermyriad, bonusPassPermyriad);
}

// 调试:设置本地彩金放行概率覆盖（传负数清除覆盖）
void DLL_SetJackpotPassOverride(int32_t jackpotPassPermyriad)
{
	TableControl_SetJackpotPassOverride(jackpotPassPermyriad);
}

// 上报联网 Jackpot 派彩给 TableControl 统计模块。
void DLL_OnJackpotOnlineWin(int32_t jpOnlineBet)
{
	GameInstance_t* inst = get_instance(GAME_ID_INVALID);
	TableControl_OnJackpotOnlineWin(jpOnlineBet);
	if (inst != NULL && jpOnlineBet > 0)
	{
		g_GameManager.debugInfo.dwJackpotOnlineTime++;
		g_GameManager.debugInfo.dwJackpotOnlineWinScore += jpOnlineBet;
		g_GameManager.debugInfo.dwWinScore += jpOnlineBet;
	}
}

// 读取当前生效难度配置（覆盖值优先于档位默认值）。
void DLL_GetRtpDifficulty(RtpProfileConfig* outProfile)
{
	TableControl_GetRtpDifficulty(outProfile);
}

// 根据 gameId 获取实例，传无效ID时返回当前实例。
GameInstance_t* get_instance(GameInstanceId_t gameId)
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

// 切换游戏实例并刷新当前实例指针。
int8_t DLL_GameSwitch(GameInstanceId_t gameId)
{
	int8_t ok = GameManager_SwitchGame(gameId);
	if (ok) g_CurrentGameInstance = GameManager_GetCurrentInstance();
	return ok;
}

// 生成一局结果
void GenerateARound(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	GenerationResult_GenerateNormal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}

//应用调试模式
void ApplyDebugMode(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, uint16_t* idVec, GameInstanceId_t gameId)
{
	//调试模式
#ifdef _DebugControlMode
	if (gDebugControlMode.mode == DCM_PointResData)
	{
		while (1)
		{
			Matrix_u_reset(mxu);
			NatureAlg_GenRndMxu(inst->gameConfig.header.normalRollTableId, mxu, inst->gameConfig.header.rowCount);
#ifdef _IMHERE
			
#endif // _IMHERE
			//全线计算
			//matrixBet=Matrix_u_computerMatrix_243(mxu, idVec);
			//有线计算
			* matrixBet = Matrix_u_computerMatrixById(mxu, idVec, &inst->gameConfig, (uint32_t)gameId, info);
			
			if (mxu->resultType == gDebugControlMode.resType)
			{
				break;
			}
		}

		switch (mxu->resultType)
		{
		case RT_Lose:
		{

		}
		break;
		case RT_Win:
		{

		}
		break;
		case RT_FreeWin:
		{
			//生成免费局数
			GenerationResult_GenerateFree(info, betVal, inst, mxu, gameId);
		}
		break;
		case RT_BonusWin:
		{
			//生成大奖结果
			GenerationResult_GenerateBonus(info, betVal, inst, mxu, gameId);
		}
		break;
		case RT_Jackpot:
		{
			//生成本地彩金结果
			GenerationResult_GenerateJackpot(info, betVal, inst, mxu, gameId);
		}
		break;
		default:
		{

		}
		break;
		}
	}
#endif
}

// 保存展会模式矩阵数据（仅缓存，不立即参与结算）。
int32_t DLL_SaveExhibitionData(const int32_t* pMatrixData, uint32_t dataLen, int32_t* pAppliedLen)
{
	if (pAppliedLen != NULL)
	{
		*pAppliedLen = 0;
	}

	if (pMatrixData == NULL || dataLen == 0)
	{
		return -1;
	}

#ifndef _ExhibitionMode
	return -2; // 编译时未开启展会模式
#else
	GameInstance_t* inst = get_instance(GAME_ID_INVALID);
	uint32_t expectedLen = 0;

	if (inst == NULL)
	{
		return -3;
	}

	expectedLen = (uint32_t)inst->gameConfig.header.colCount * (uint32_t)inst->gameConfig.header.rowCount;
	if (dataLen != expectedLen || dataLen > GE_WheelChessMaxNum)
	{
		return -4;
	}
	{
		// 统一交给 CommonStruct 模块缓存，等待 DLL_GetGameResultById 消费。
		int32_t setRet = ExhibitionMode_SetMatrix(pMatrixData, dataLen, &inst->gameConfig, pAppliedLen);
		if (setRet != 0)
		{
			return setRet;
		}
	}

	if (pAppliedLen != NULL)
	{
		*pAppliedLen = (int32_t)dataLen;
	}

	QS_LOG("\r\nExhibitionMode cache ok, len:%d", (int32_t)dataLen);
	return 0;
#endif
}

//应用展会模式
int32_t DLL_ApplyExhibitionMode(GameInstance_t* inst, Matrix_u* mxu, uint16_t* idVec, RoundInfo_t* ri, int32_t betVal, int32_t* matrixBet, GameInstanceId_t gameId)
{
#ifndef _ExhibitionMode
	return 0;
#else
	uint8_t exhibitionMatrixData[GE_WheelChessMaxNum] = { 0 };
	uint32_t expectedLen = (uint32_t)inst->gameConfig.header.colCount * (uint32_t)inst->gameConfig.header.rowCount;
	// 若有待消费展会矩阵，则本局优先使用（仅生效一次）。
	if (!ExhibitionMode_TryConsumeMatrix(exhibitionMatrixData, expectedLen))
	{
		return 0;
	}
	Matrix_u_reset(mxu);
	Matrix_u_setIntData(mxu, inst->gameConfig, exhibitionMatrixData);
	*matrixBet = Matrix_u_computerMatrixById(mxu, idVec, &inst->gameConfig, (uint32_t)gameId, ri);

	switch (mxu->resultType)
	{
	case RT_FreeWin:
		GenerationResult_GenerateFree(ri, betVal, inst, mxu, gameId);
		break;
	case RT_BonusWin:
		GenerationResult_GenerateBonus(ri, betVal, inst, mxu, gameId);
		break;
	case RT_Jackpot:
		GenerationResult_GenerateJackpot(ri, betVal, inst, mxu, gameId);
		break;
	default:
		break;
	}
	return 1;
#endif
}

//应用RoundInfo到输出结果
void ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, uint16_t* idVec, GameInstanceId_t gameId)
{
	GenerationResult_ApplyMatrixToOutResByRound(pRes, resType, info, Mxu, idVec, gameId);
}

//应用赠送局到输出结果
void ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx, GameInstanceId_t gameId)
{
	GenerationResult_ApplyMatrixToOutResForFree(pRes, info, freeIdx, gameId);
}

//请求结果
void GetNormalResult(player_data_item* pUserInfo, int32_t betVal, OutResult_t* outRes, int32_t* ret, GameInstanceId_t gameId)
{
	*ret = 4;
	int8_t opType = outRes->openType;
	OutResult_Init(outRes);

	GameInstance_t* inst = get_instance(gameId);
	if (!inst) {
		*ret = -1;
		return;
	}

	RoundInfo_t ri;
	RoundInfo_t_Init(&ri);

	if (opType == OT_Give)
	{
		ApplyMatrixToOutResForFree(outRes, &inst->freeGameInfo.roundInfo, inst->freeGameInfo.nCurFreeIdx, gameId);
		inst->freeGameInfo.nCurFreeIdx++;
		inst->freeGameInfo.nRemainFreeBet -= outRes->nMatrixBet;
		//如果全部免费局获取完，将剩余免费次数设为0
		if (inst->freeGameInfo.nCurFreeIdx == inst->freeGameInfo.nTotalFreeTime)
		{
			QS_LOG("\r\ninst->freeGameInfo.nRemainFreeBet :%d", inst->freeGameInfo.nRemainFreeBet);
		}
		outRes->openType = OT_Give;
		*ret = 5;

		/* 每局赠送结束后更新游标/剩余倍数并落盘，断电后可从下一局 nCurFreeIdx 继续 */
		FreeGamePersist_Save(inst);

		return;
	}
	else
	{
		Matrix_u mxu;
		Matrix_u_reset(&mxu);
		uint8_t useExhibitionMatrix = 0;
		uint16_t idVec[GE_MaxIDNum] = { 0 };
		int32_t jpTypeList[GAME_Local_JP_MAX] = { 0 };
		int32_t jpValueList[GAME_Local_JP_MAX] = { 0 };
		int32_t matrixBet = 0;
		int32_t bRes = 0;
		int32_t totalPlayScore = betVal * inst->gameConfig.header.lineCount;
		int64_t fishValue = 0;                                       // 候选理论支付分
		const RtpProfileConfig* profile = TableControl_GetActiveProfile(); // 当前生效难度配置
		int32_t jpCount = 0;
		uint8_t jpCandidateType = JT_None;
		int32_t jpCandidateTotal = 0;

		// 统一在主流程内进行奖池注入与多彩金检测，避免调用方重复或遗漏。
		LotteryManager_OnPlay(&gLotteryManager, totalPlayScore);
		jpCount = LotteryManager_TryGetLotterys(&gLotteryManager, totalPlayScore, jpTypeList, jpValueList, GAME_Local_JP_MAX);
#ifdef _DebugControlMode
		if (gDebugControlMode.mode == DCM_PointResData && gDebugControlMode.resType == RT_Jackpot && jpCount <= 0)
		{
			int32_t forceJpType = (gDebugControlMode.jpType >= JT_Major && gDebugControlMode.jpType <= JT_Mini) ? gDebugControlMode.jpType : JT_Mini;
			int32_t forceJpBet = totalPlayScore * 50;
			if (forceJpBet <= 0)
			{
				forceJpBet = 1;
			}
			jpTypeList[0] = forceJpType;
			jpValueList[0] = forceJpBet;
			jpCount = 1;
		}
#endif
		if (jpCount > 0)
		{
			ri.nJPCount = (uint8_t)jpCount;
			jpCandidateType = (uint8_t)jpTypeList[0];
			for (int32_t i = 0; i < jpCount; ++i)
			{
				ri.JPTypeArray[i] = (uint8_t)jpTypeList[i];
				ri.JPBetArray[i] = jpValueList[i];
				jpCandidateTotal += jpValueList[i];
			}
			ri.nTotalJackpotBet = jpCandidateTotal;
		}

		
		//生成结果
		GenerateARound(&ri, inst, &mxu, betVal, &matrixBet, idVec, gameId);
		//应用调试模式。
		ApplyDebugMode(&ri, inst, &mxu, betVal, &matrixBet, idVec, gameId);
		//应用展会模式
		DLL_ApplyExhibitionMode(inst, &mxu, idVec, &ri, betVal, &matrixBet, gameId);

		ri.resType = mxu.resultType;
		ri.nMatrixBet = matrixBet;

		// 累计下注
		g_GameManager.debugInfo.dwPlayScore += betVal * g_CurrentGameInstance->gameConfig.header.lineCount;

		TableControl_OnFireWeapon(pUserInfo, betVal, inst->gameConfig.header.lineCount); // 记录下注统计
		fishValue = (int64_t)betVal * (ri.nMatrixBet + ri.nFreeBet + ri.nBonusBet) + jpCandidateTotal;      // 计算候选总支付（含JP）
		bRes = TableControl_GetShotResult(pUserInfo, betVal, fishValue, inst->gameConfig.header.lineCount, &ri, profile, &jpCandidateType, &jpCandidateTotal); //判定（含JP候选）
		TableControl_OnEndShot(pUserInfo, bRes, fishValue);

		if (!bRes)
		{
			//强制输局
			GenerationResult_GenerateLose(inst, &mxu, idVec, gameId);
			ri.nJPCount = 0;
			for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
			{
				ri.JPTypeArray[i] = JT_None;
				ri.JPBetArray[i] = 0;
			}
			ri.nTotalJackpotBet = 0;
			outRes->resType = RT_Lose;     // 标记输局
			outRes->nMatrixBet = 0;        // 线奖清零
			outRes->openType = OT_Normal;  // 仍属于普通付费开局
			ApplyMatrixToOutResByRound(outRes, RT_Lose, &ri, &mxu, idVec, gameId);
			*ret = 0;                      // 返回输局编码
			// 直接结束
		}
		else
		{
			*ret = (int32_t)ri.resType; // 先记录候选结果类型
			switch (ri.resType)         // 按候选结果写回 outRes
			{
			case RT_Lose:
			{
				outRes->resType = RT_Lose;   // 落地输局类型
				outRes->nMatrixBet = 0;      // 线奖为0
				//应用RoundInfo到outRes
				ApplyMatrixToOutResByRound(outRes, RT_Lose, &ri, &mxu, idVec, gameId); // 回填矩阵与id信息
				*ret = 0;                                                      // 返回输局编码
			}
			break;
			case RT_Win:
			{
				outRes->resType = RT_Win;           // 落地普通中奖
				outRes->nMatrixBet = ri.nMatrixBet; // 回填线奖倍数
				//应用RoundInfo到outRes
				ApplyMatrixToOutResByRound(outRes, RT_Win, &ri, &mxu, idVec, gameId); // 写入中奖细节
				*ret = 1;                                                     // 返回普通中奖编码
			}
			break;
			case RT_FreeWin:
			{
				outRes->resType = RT_FreeWin;       // 落地免费触发结果
				outRes->nMatrixBet = ri.nMatrixBet; // 回填触发局线奖倍数
				ri.nJPCount = 0;
				for (int32_t i = 0; i < GAME_Local_JP_MAX; ++i)
				{
					ri.JPTypeArray[i] = JT_None;
					ri.JPBetArray[i] = 0;
				}
				ri.nTotalJackpotBet = 0;
				outRes->nTotalJackpotBet = 0;
				//存储免费信息
				FreeGameInfo_t_Reset(&inst->freeGameInfo);                    // 清理旧免费态
				RoundInfo_t_Copy(&inst->freeGameInfo.roundInfo, &ri);         // 保存本次免费round信息
				inst->freeGameInfo.nBetVal = betVal;                          // 记录触发时下注
				inst->freeGameInfo.nCurFreeIdx = 0;                           // 免费局游标归零
				inst->freeGameInfo.nTotalFreeTime = ri.nFreeNum;              // 记录免费总次数
				inst->freeGameInfo.nRemainFreeBet = ri.nFreeBet;              // 记录剩余免费总倍数
				inst->freeGameInfo.nFreeTotalWin = (uint16_t)ri.nFreeBet;     // 记录免费总赢分（16位存储）
				//应用RoundInfo到outRes
				ApplyMatrixToOutResByRound(outRes, RT_FreeWin, &ri, &mxu, idVec, gameId); // 写入免费触发输出
				*ret = 2;                                                          // 返回免费触发编码
				/* 刚生成整套免费序列，立即持久化，避免触发后掉电丢进度 */
				FreeGamePersist_Save(inst);
			}
			break;
			case RT_BonusWin:
			{
				outRes->resType = RT_BonusWin;      // 落地Bonus触发结果
				outRes->nMatrixBet = ri.nMatrixBet; // 回填触发局线奖倍数

				//应用RoundInfo到outRes
				ApplyMatrixToOutResByRound(outRes, RT_BonusWin, &ri, &mxu, idVec, gameId); // 写入Bonus输出数据
				*ret = 3;                                                           
			}
			break;
			case RT_Jackpot:
			{
				int32_t jpTotal = 0;
				outRes->resType = RT_Jackpot;     
				outRes->nMatrixBet = ri.nMatrixBet; 
				outRes->nJPCount = ri.nJPCount;

				ApplyMatrixToOutResByRound(outRes, RT_Jackpot, &ri, &mxu, idVec, gameId);
				*ret = 4;
	
				for (int32_t i = 0; i < ri.nJPCount && i < GAME_Local_JP_MAX; ++i)
				{
					if (ri.JPBetArray[i] > 0)
					{
						LotteryManager_CommitLottery(&gLotteryManager, ri.JPTypeArray[i], ri.JPBetArray[i]);
					}
				}
			}
			break;
			default:
			{
				// 未知类型兜底，保持当前结果不额外处理。
			}
			break;
			}

			outRes->openType = OT_Normal; // 付费局统一标记普通开局
			g_GameManager.debugInfo.dwWinScore += betVal * (outRes->nMatrixBet + outRes->nTotalFreeBet + outRes->nBonusBet)+ outRes->nTotalJackpotBet; // 累计本局总赢分
		}
	}
}

//生成指定Id结果
void DLL_GetGameResultById(player_data_item* pUserInfo, int32_t betValue, OutResult_t* outRes, int32_t* ret, GameInstanceId_t gameId)
{
	GetNormalResult(pUserInfo, betValue, outRes, ret, gameId);
	GameInstance_t* inst = get_instance(gameId);
	if (!inst) return;

	g_GameManager.debugInfo.dwTotalPlayTime++;
	if (outRes->resType == RT_Win) {
		g_GameManager.debugInfo.dwNormalWinTime++;
	}
	if (outRes->openType == OT_Normal) g_GameManager.debugInfo.dwNormalOpenTime++;
	else g_GameManager.debugInfo.dwGiveOpenTime++;
	if (outRes->resType == RT_FreeWin) {
		g_GameManager.debugInfo.dwFreeGameTime++;
	}
	else if (outRes->resType == RT_BonusWin) {
		g_GameManager.debugInfo.dwBonusTime++;
	}
	else if (outRes->resType == RT_Lose) {
		g_GameManager.debugInfo.dwLooseTime++;
	}

	// RTP 分项统一按“金额口径”累计，仅统计真实付费局（普通开局）。
	if (outRes->openType == OT_Normal) {
		g_GameManager.debugInfo.dwBaseWinScore += (uint32_t)(betValue * outRes->nMatrixBet);
		g_GameManager.debugInfo.dwFreeWinScore += (uint32_t)(betValue * outRes->nTotalFreeBet);
		g_GameManager.debugInfo.dwBonusWinScore += (uint32_t)(betValue * outRes->nBonusBet);
		g_GameManager.debugInfo.dwJackpotWinScore += (uint32_t)outRes->nTotalJackpotBet;
	}

	//本地彩金
	if (outRes->nTotalJackpotBet > 0) 
	{
		g_GameManager.debugInfo.dwJackpotTime++;
	}
	g_GameManager.debugInfo.dwFreeGameBetError = inst->freeGameInfo.nRemainFreeBet;

//玩家日志
#ifdef _WritePlayerLog
	const char* resTypeNameStrVec[] = { "Lose", "Win", "FreeGame", "BonusGame", "Jackpot", "JackpotOnline" };
	const char* optTypeNameStrVec[] = { "Normal", "Give" };
	const size_t resTypeCount = sizeof(resTypeNameStrVec) / sizeof(resTypeNameStrVec[0]);
	const size_t optTypeCount = sizeof(optTypeNameStrVec) / sizeof(optTypeNameStrVec[0]);
	const char* resTypeStr = "UnknownRes";
	const char* optTypeStr = "UnknownOpen";

	if ((int32_t)outRes->resType >= 0 && (size_t)outRes->resType < resTypeCount)
	{
		resTypeStr = resTypeNameStrVec[outRes->resType];
	}
	if ((int32_t)outRes->openType >= 0 && (size_t)outRes->openType < optTypeCount) 
	{
		optTypeStr = optTypeNameStrVec[outRes->openType];
	}

	char finalString[384] = { 0 };
	char resString[256] = { 0 };
	size_t resUsed = 0;
	size_t finalUsed = 0;

	if (outRes->resType == RT_Win)
	{
		append_format(resString, sizeof(resString), &resUsed, "  IDVec: [");

		for (int32_t j = 0; j < GE_MaxIDNum; ++j)
		{
			if (outRes->IDVec[j] > 0)
			{
				int32_t lineWin = GetIDBetValue(outRes->IDVec[j]);
				append_format(resString, sizeof(resString), &resUsed, "%d<%d> ", outRes->IDVec[j], lineWin);
			}
		}
		append_format(resString, sizeof(resString), &resUsed, "]");
	}

	if (outRes->openType == OT_Give)
	{
		
	}

	if (outRes->resType == RT_FreeWin) 
	 {
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
	append_format(finalString, sizeof(finalString), &finalUsed, "\r\noptTypeStr:%s resTypeStr:%s betValue:%d nMatrixBet:%d ",
		optTypeStr, resTypeStr, betValue, outRes->nMatrixBet);
	int64_t expScore = (int64_t)pUserInfo->Wins + ((int64_t)betValue * (int64_t)outRes->nMatrixBet);
	append_format(finalString, sizeof(finalString), &finalUsed, "totalscore:%lld expectation:%lld %s",
		(long long)pUserInfo->Wins, (long long)expScore, resString);


	QS_LOG("\r\n%s", finalString);
#ifdef _WritePlayerData
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

	QS_LOG("\r\n-----------------\n");
#endif

#endif
}

int32_t GetIDBetValue(int32_t ID)
{
	if (ID <= 0)
		return 0;

	GameInstance_t* inst = get_instance(GAME_ID_INVALID);
	if (inst == NULL)
		return 0;

	uint8_t betTableIdx = inst->gameConfig.header.id;
	int32_t chessType = ID % 100;
	int32_t nEliminateNum = (ID % 1000) / 100;

	if (nEliminateNum < 2 || nEliminateNum > 5)
		return 0;

	return (int32_t)GET_BET_VALUE(betTableIdx, chessType, nEliminateNum - 2);
}

//控制台结果输出
int8_t* OutResToJsonnById(OutResult_t* outRes, GameInstanceId_t gameId)
{
	GameInstance_t* inst = get_instance(gameId);
	if (inst == NULL || outRes == NULL)
	{
		return NULL;
	}
	return GenerationResult_OutResToJson(outRes, inst, gameId);
}

//嵌入式结果输出
void DLL_OutResToSenvById(OutResult_t* outRes, int32_t* res, GameInstanceId_t gameId)
{
	GameInstance_t* inst = get_instance(gameId);
	GenerationResult_OutResToSenv(outRes, inst, res, gameId);
}

// 获取指定游戏实例的调试统计信息。
void DLL_GetUserDebugInfo(DebugInfo* pDebugInfo)
{
	DebugInfo_accum(pDebugInfo, &g_GameManager.debugInfo);
}

// 关闭指定游戏实例并释放资源。
void DLL_GameClose(GameInstanceId_t gameId)
{
	GameManager_UnregisterGame(gameId);
}
