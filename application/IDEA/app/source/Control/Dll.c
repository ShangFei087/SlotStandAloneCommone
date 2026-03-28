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
// 嵌入式友好的轻量字符串拼接：避免依赖 vsnprintf 的库实现差异和较大体积。
static void append_raw(char* buffer, size_t buffer_size, size_t* used, const char* src)
{
	size_t i = 0;
	if (buffer == NULL || used == NULL || src == NULL || buffer_size == 0 || *used >= buffer_size) return;
	while (src[i] != '\0' && *used < (buffer_size - 1))
	{
		buffer[*used] = src[i];
		(*used)++;
		i++;
	}
	buffer[*used] = '\0';
}

static void append_char(char* buffer, size_t buffer_size, size_t* used, char c)
{
	if (buffer == NULL || used == NULL || buffer_size == 0 || *used >= (buffer_size - 1)) return;
	buffer[*used] = c;
	(*used)++;
	buffer[*used] = '\0';
}

static void append_uint64_dec(char* buffer, size_t buffer_size, size_t* used, uint64_t value)
{
	char tmp[21];
	int32_t idx = 0;
	if (value == 0)
	{
		append_char(buffer, buffer_size, used, '0');
		return;
	}
	while (value > 0 && idx < (int32_t)sizeof(tmp))
	{
		tmp[idx++] = (char)('0' + (value % 10));
		value /= 10;
	}
	while (idx > 0)
	{
		append_char(buffer, buffer_size, used, tmp[--idx]);
	}
}

static void append_int64_dec(char* buffer, size_t buffer_size, size_t* used, int64_t value)
{
	uint64_t abs_value = 0;
	if (value < 0)
	{
		append_char(buffer, buffer_size, used, '-');
		abs_value = (uint64_t)(-(value + 1)) + 1U;
	}
	else
	{
		abs_value = (uint64_t)value;
	}
	append_uint64_dec(buffer, buffer_size, used, abs_value);
}

static void append_format(char* buffer, size_t buffer_size, size_t* used, const char* format, ...)
{
	va_list args;
	size_t i = 0;

	if (buffer == NULL || used == NULL || format == NULL || buffer_size == 0) return;
	if (*used >= buffer_size)
	{
		*used = buffer_size - 1;
		buffer[*used] = '\0';
		return;
	}
	buffer[*used] = '\0';

	va_start(args, format);
	while (format[i] != '\0' && *used < (buffer_size - 1))
	{
		if (format[i] != '%')
		{
			append_char(buffer, buffer_size, used, format[i]);
			i++;
			continue;
		}

		i++;
		if (format[i] == '\0') break;

		if (format[i] == '%')
		{
			append_char(buffer, buffer_size, used, '%');
			i++;
			continue;
		}

		if (format[i] == 's')
		{
			const char* s = va_arg(args, const char*);
			append_raw(buffer, buffer_size, used, (s != NULL) ? s : "(null)");
			i++;
			continue;
		}

		if (format[i] == 'c')
		{
			int32_t c = va_arg(args, int32_t);
			append_char(buffer, buffer_size, used, (char)c);
			i++;
			continue;
		}

		if (format[i] == 'd' || format[i] == 'i')
		{
			int32_t v = va_arg(args, int32_t);
			append_int64_dec(buffer, buffer_size, used, (int64_t)v);
			i++;
			continue;
		}

		if (format[i] == 'u')
		{
			uint32_t v = va_arg(args, uint32_t);
			append_uint64_dec(buffer, buffer_size, used, (uint64_t)v);
			i++;
			continue;
		}

		if (format[i] == 'l' && format[i + 1] == 'l' && (format[i + 2] == 'd' || format[i + 2] == 'i'))
		{
			long long v = va_arg(args, long long);
			append_int64_dec(buffer, buffer_size, used, (int64_t)v);
			i += 3;
			continue;
		}

		// 未支持的格式占位符，按字面输出，避免静默丢信息。
		append_char(buffer, buffer_size, used, '%');
		append_char(buffer, buffer_size, used, format[i]);
		i++;
	}
	va_end(args);
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
			Matrix_u_setIntData(&mxu, temp);
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
void ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, int32_t* idVec)
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

	}
	else
	{
		//失败结果
	}
}
//应用赠送局到输出结果
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
	memcpy(pRes->WildPosArray, info->WildPosArray[freeIdx], sizeof(pRes->WildPosArray));
	pRes->resType = resType;
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
			ApplyMatrixToOutResByRound(outRes, RT_Lose, &ri, &mxu, idVec);
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
				ApplyMatrixToOutResByRound(outRes, RT_Lose, &ri, &mxu, idVec); // 回填矩阵与id信息
				*ret = 0;                                                      // 返回输局编码
			}
			break;
			case RT_Win:
			{
				outRes->resType = RT_Win;           // 落地普通中奖
				outRes->nMatrixBet = ri.nMatrixBet; // 回填线奖倍数
				//应用RoundInfo到outRes
				ApplyMatrixToOutResByRound(outRes, RT_Win, &ri, &mxu, idVec); // 写入中奖细节
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
				ApplyMatrixToOutResByRound(outRes, RT_FreeWin, &ri, &mxu, idVec); // 写入免费触发输出
				*ret = 2;                                                          // 返回免费触发编码
			}
			break;
			case RT_BonusWin:
			{
				outRes->resType = RT_BonusWin;      // 落地Bonus触发结果
				outRes->nMatrixBet = ri.nMatrixBet; // 回填触发局线奖倍数

				//应用RoundInfo到outRes
				ApplyMatrixToOutResByRound(outRes, RT_BonusWin, &ri, &mxu, idVec); // 写入Bonus输出数据
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

int8_t* OutResToJsonnById(OutResult_t* outRes, GameInstanceId_t gameId)
{
	char* strRes = (char*)malloc(2048);
	size_t used = 0;
	int8_t* idVecStr = NULL;
	int8_t* matrixStr = NULL;
	GameInstance_t* inst = get_instance(gameId);
	int8_t curwheelChessNum = inst->gameConfig.header.wheelChessNum;
	// 返回堆内存，调用方负责 free；失败返回 NULL。
	if (strRes == NULL || outRes == NULL) return NULL;

	strRes[0] = '\0';
	append_format(strRes, 2048, &used, "{");

	append_format(strRes, 2048, &used, "\"OpenType\":%d,", outRes->openType);
	append_format(strRes, 2048, &used, "\"ResultType\":%d,", outRes->resType);

	idVecStr = ArrayToString((int32_t*)outRes->IDVec, GE_MaxIDNum, 0);
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
		switch (gameId)
		{
		case 3993:
		{
			wildStr = ByteArrayToString(outRes->WildPosArray, curwheelChessNum);
			append_format(strRes, 2048, &used, "\"WildData\":%s,", wildStr ? (const char*)wildStr : "[]");
			free(wildStr);
		}
		break;
		default:
		{

		}
		break;
		}
	}

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
				bonusStr = ArrayToString((int32_t*)outRes->BonusData, wildColCountArray[bonusCount - 3], 1);
				append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
				free(bonusStr);
			}
			break;
			//神秘
			case 1:
			{
				bonusStr = ArrayToString((int32_t*)outRes->BonusData, curwheelChessNum, 1);
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
		break;
		default:
		{
			append_format(strRes, 2048, &used, "\"BonusType\":%d,", outRes->nBonusType);
			append_format(strRes, 2048, &used, "\"BonusBet\":%d,", outRes->nBonusBet);
			bonusStr = ArrayToString((int32_t*)outRes->BonusData, curwheelChessNum, 1);
			append_format(strRes, 2048, &used, "\"BonusData\":%s,", bonusStr ? (const char*)bonusStr : "[]");
			free(bonusStr);
		}
		break;
		}

	}

	//中了彩金
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
