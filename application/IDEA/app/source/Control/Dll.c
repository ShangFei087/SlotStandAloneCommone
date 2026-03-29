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
void GenerateARound(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, int32_t* idVec, GameInstanceId_t gameId)
{
	GenerationResult_GenerateNormal(info, inst, mxu, betVal, matrixBet, idVec, gameId);
}
//应用调试模式
void ApplyDebugMode(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, int32_t* idVec, GameInstanceId_t gameId)
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
			uint8_t temp[GE_WheelChessMaxNum] =
			{
			   3,2,9,5,1,9,3,3,9,3,4,6,5,5,4
			};
			Matrix_u_setIntData(&mxu, inst->gameConfig, temp);
#endif // _IMHERE
			//全线计算
			//matrixBet=Matrix_u_computerMatrix_243(mxu, idVec);
			//有线计算
			* matrixBet = Matrix_u_computerMatrixById(mxu, idVec, &inst->gameConfig, (uint32_t)gameId);
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
		default:
		{

		}
		break;
		}
	}
#endif
}
//应用RoundInfo到输出结果
void ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, int32_t* idVec, GameInstanceId_t gameId)
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
	uint8_t jpType = outRes->nJPType;
	int32_t jpBet = outRes->nJPBet;
	OutResult_Init(outRes);
	outRes->nJPType = jpType;
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
		ApplyMatrixToOutResForFree(outRes, &inst->freeGameInfo.roundInfo, inst->freeGameInfo.nCurFreeIdx, gameId);
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
		Matrix_u mxu;
		Matrix_u_reset(&mxu);
		int32_t idVec[GE_MaxIDNum] = { 0 };
		int32_t matrixBet = 0;
		int32_t bRes = 0;
		int64_t fishValue = 0;                                       // 候选理论支付分
		const RtpProfileConfig* profile = TableControl_GetActiveProfile(); // 当前生效难度配置

		//生成结果
		GenerateARound(&ri, inst, &mxu, betVal, &matrixBet, &idVec, gameId);
		//应用调试模式。
		ApplyDebugMode(&ri, inst, &mxu, betVal, &matrixBet, &idVec, gameId);
		ri.resType = mxu.resultType;
		ri.nMatrixBet = matrixBet;

		// 累计下注
		g_GameManager.debugInfo.dwPlayScore += betVal * g_CurrentGameInstance->gameConfig.header.lineCount;

		TableControl_OnFireWeapon(pUserInfo, betVal, inst->gameConfig.header.lineCount); // 记录下注统计
		fishValue = (int64_t)betVal * (ri.nMatrixBet + ri.nFreeBet + ri.nBonusBet) + outRes->nJPBet;      // 计算候选总支付（含JP）
		bRes = TableControl_GetShotResult(pUserInfo, betVal, fishValue, inst->gameConfig.header.lineCount, &ri, profile, &outRes->nJPType, &outRes->nJPBet); //判定（含JP候选）
		TableControl_OnEndShot(pUserInfo, bRes, fishValue);

		
		if (!bRes)
		{
			outRes->nJPType = JT_None;
			outRes->nJPBet = 0;
			//强制输局
			GenerationResult_GenerateLose(inst, &mxu, &idVec, gameId);
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
				//存储免费信息
				FreeGameInfo_t_Reset(&inst->freeGameInfo);                    // 清理旧免费态
				RoundInfo_t_Copy(&inst->freeGameInfo.roundInfo, &ri);         // 保存本次免费round信息
				inst->freeGameInfo.nBetVal = betVal;                          // 记录触发时下注
				inst->freeGameInfo.nCurFreeIdx = 0;                           // 免费局游标归零
				inst->freeGameInfo.nTotalFreeTime = ri.nFreeNum;              // 记录免费总次数
				inst->freeGameInfo.nRemainFreeBet = ri.nFreeBet;              // 记录剩余免费总倍数
				//应用RoundInfo到outRes
				ApplyMatrixToOutResByRound(outRes, RT_FreeWin, &ri, &mxu, idVec, gameId); // 写入免费触发输出
				*ret = 2;                                                          // 返回免费触发编码
			}
			break;
			case RT_BonusWin:
			{
				outRes->resType = RT_BonusWin;      // 落地Bonus触发结果
				outRes->nMatrixBet = ri.nMatrixBet; // 回填触发局线奖倍数

				//应用RoundInfo到outRes
				ApplyMatrixToOutResByRound(outRes, RT_BonusWin, &ri, &mxu, idVec, gameId); // 写入Bonus输出数据
				*ret = 3;                                                           // 返回Bonus触发编码
			}
			break;
			case RT_Jackpot:
			{
				//outRes->resType = RT_Jackpot; // 落地Jackpot类型（预留）
			}
			break;
			default:
			{
				// 未知类型兜底，保持当前结果不额外处理。
			}
			break;
			}

			// 主结果通过后，本地彩金候选再提交落账。
			if (outRes->nJPBet > 0)
			{
				LotteryManager_CommitLottery(&gLotteryManager, outRes->nJPType, outRes->nJPBet);
			}
			outRes->openType = OT_Normal; // 付费局统一标记普通开局
			g_GameManager.debugInfo.dwWinScore += betVal * (outRes->nMatrixBet + outRes->nTotalFreeBet + outRes->nBonusBet) + outRes->nJPBet; // 累计本局总赢分
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
		g_GameManager.debugInfo.dwBaseWinScore += (int64_t)betValue * outRes->nMatrixBet;
		g_GameManager.debugInfo.dwFreeWinScore += (int64_t)betValue * outRes->nTotalFreeBet;
		g_GameManager.debugInfo.dwBonusWinScore += (int64_t)betValue * outRes->nBonusBet;
		g_GameManager.debugInfo.dwJackpotWinScore += outRes->nJPBet;
	}

	//本地彩金
	if (outRes->nJPBet > 0) 
	{
		g_GameManager.debugInfo.dwJackpotTime++;
	}
	g_GameManager.debugInfo.dwFreeGameBetError = inst->freeGameInfo.nRemainFreeBet;

//玩家日志
#ifdef _WritePlayerLog
	const char* resTypeNameStrVec[] = { "Lose", "Win", "FreeGame", "BonusGame" };
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
	append_format(finalString, sizeof(finalString), &finalUsed, " optTypeStr:%s resTypeStr:%s betValue:%d nMatrixBet:%d ",
		optTypeStr, resTypeStr, betValue, outRes->nMatrixBet);
	int64_t expScore = (int64_t)pUserInfo->Wins + ((int64_t)betValue * (int64_t)outRes->nMatrixBet);
	append_format(finalString, sizeof(finalString), &finalUsed, "totalscore:%lld expectation:%lld %s",
		(long long)pUserInfo->Wins, (long long)expScore, resString);


	QS_LOG("%s\n", finalString);
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

	QS_LOG("-----------------\n");
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

int8_t* OutResToJsonnById(OutResult_t* outRes, GameInstanceId_t gameId)
{
	GameInstance_t* inst = get_instance(gameId);
	GenerationResult_OutResToJson(outRes, inst, gameId);
}

void DLL_OutResToSenvById(OutResult_t* outRes, int32_t* res, GameInstanceId_t gameId)
{

}

// 获取指定游戏实例的调试统计信息。
void DLL_GetUserDebugInfo(DebugInfo* pDebugInfo, GameInstanceId_t gameId)
{
	GameInstance_t* inst = get_instance(gameId);
	if (inst) DebugInfo_accum(pDebugInfo, &g_GameManager.debugInfo);
}
// 关闭指定游戏实例并释放资源。
void DLL_GameClose(GameInstanceId_t gameId)
{
	GameManager_UnregisterGame(gameId);
}
