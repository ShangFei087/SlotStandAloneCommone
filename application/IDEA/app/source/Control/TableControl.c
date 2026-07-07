#include "TableControl.h" 
#include "TableControlPersistManager.h"

#include "../GameAlgo/common/JRand.h" 
#include <string.h> 
#include "Test.h"
#include "ComputerData.h"

/** 闸门模块唯一全局实例：统计、运行时、调参常量。 */
static TableControlState gTableControl = {
	{ 0 },
	{
		RTP_REGION_DOMESTIC,
		0,
		-1, -1, -1,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0,
		0, 0, 0, 0, 0, 0,
		0
	},
	{
		500,
		50,
		50,
		500,
		100,
		200,
		300,
		500,
		100,
		100,
		100,
		100,
		10000,
		10000
	}
};

static void TableControl_ResetPoolsAdaptiveAndWindow(void);

/**
 * @brief 将当前运行时刷入 Flash 镜像（非 _LocalDebug 时生效）。
 */
static void TableControl_FlushRuntimePersist(void)
{
#ifndef _LocalDebug
	TableControlPersist_Save();
#endif
}

void TableControl_ExportRuntime(TableControlRuntime* out)
{
	if (out == NULL) return;
	*out = gTableControl.runtime;
}

/** 将自适应概率偏移夹断到 tuning.adaptiveDeltaMax 范围内 */
static int32_t TableControl_ClampAdaptiveDelta(int32_t delta)
{
	if (delta > gTableControl.tuning.adaptiveDeltaMax) return gTableControl.tuning.adaptiveDeltaMax;
	if (delta < -gTableControl.tuning.adaptiveDeltaMax) return -gTableControl.tuning.adaptiveDeltaMax;
	return delta;
}

/** 校验持久化快照中的区域、档位、概率覆盖是否合法 */
static int8_t TableControl_IsRuntimeConfigValid(const TableControlRuntime* rt)
{
	if (rt == NULL) return 0;
	if (rt->region != RTP_REGION_DOMESTIC && rt->region != RTP_REGION_OVERSEAS) return 0;
	if (rt->rtpPermyriad < 0) return 0;
	if (rt->rtpPermyriad > 0 && RtpConfig_FindProfile(rt->region, rt->rtpPermyriad) == NULL) return 0;
	if (rt->overrideFreePass < -1 || rt->overrideFreePass > 10000) return 0;
	if (rt->overrideBonusPass < -1 || rt->overrideBonusPass > 10000) return 0;
	if (rt->overrideJackpotPass < -1 || rt->overrideJackpotPass > 10000) return 0;
	return 1;
}

void TableControl_ImportRuntime(const TableControlRuntime* in)
{
	if (in == NULL || !TableControl_IsRuntimeConfigValid(in)) return;

	gTableControl.runtime = *in;
	/* 池子/窗口数值不做额外范围校验，仅对自适应偏移做安全夹断 */
	gTableControl.runtime.adaptiveFreePassDelta = TableControl_ClampAdaptiveDelta(gTableControl.runtime.adaptiveFreePassDelta);
	gTableControl.runtime.adaptiveBonusPassDelta = TableControl_ClampAdaptiveDelta(gTableControl.runtime.adaptiveBonusPassDelta);
	gTableControl.runtime.adaptiveJackpotPassDelta = TableControl_ClampAdaptiveDelta(gTableControl.runtime.adaptiveJackpotPassDelta);
}

void TableControl_ResetRuntimeDefaults(void)
{
	gTableControl.runtime.region = RTP_REGION_DOMESTIC;
	gTableControl.runtime.rtpPermyriad = 0;
	gTableControl.runtime.overrideFreePass = -1;
	gTableControl.runtime.overrideBonusPass = -1;
	gTableControl.runtime.overrideJackpotPass = -1;
	TableControl_ResetPoolsAdaptiveAndWindow();
}

/**
 * @brief 切档后重置目标池、注入余量、自适应偏移与调参窗口（保留 region/rtp/override）。
 */
static void TableControl_ResetPoolsAdaptiveAndWindow(void)
{
	gTableControl.runtime.basePool = 0;
	gTableControl.runtime.freePool = 0;
	gTableControl.runtime.bonusPool = 0;
	gTableControl.runtime.jackpotPool = 0;
	gTableControl.runtime.netJackpotPool = 0;
	gTableControl.runtime.basePoolRemainder = 0;
	gTableControl.runtime.freePoolRemainder = 0;
	gTableControl.runtime.bonusPoolRemainder = 0;
	gTableControl.runtime.jackpotPoolRemainder = 0;
	gTableControl.runtime.netJackpotPoolRemainder = 0;
	gTableControl.runtime.adaptiveFreePassDelta = 0;
	gTableControl.runtime.adaptiveBonusPassDelta = 0;
	gTableControl.runtime.adaptiveJackpotPassDelta = 0;
	gTableControl.runtime.windowBet = 0;
	gTableControl.runtime.windowPaidBase = 0;
	gTableControl.runtime.windowPaidFree = 0;
	gTableControl.runtime.windowPaidBonus = 0;
	gTableControl.runtime.windowPaidJackpotBonus = 0;
	gTableControl.runtime.windowPaidJackpot = 0;
	gTableControl.runtime.windowRounds = 0;
}

/**
 * @brief 清空本地彩金候选（候选阶段未落账，无需回补）。
 * @param jpType [in/out] 本地彩金类型；会被清空为 JT_None。
 * @param jpBet [in/out] 本地彩金金额；会被清零。
 */
static void TableControl_RejectJackpotAndRefund(uint8_t* jpType, int32_t* jpBet)
{
	if (jpBet == NULL || *jpBet <= 0)
	{
		if (jpType != NULL) *jpType = JT_None;
		return;
	}

	*jpBet = 0;
	if (jpType != NULL) *jpType = JT_None;
}

/**
 * @brief 累计分项派彩统计，并同步窗口统计。
 * @param betVal 单线下注金额。
 * @param ri 本局回合结果（包含 resType、nMatrixBet、nFreeBet、nBonusBet）。
 * @param jpBet 本局实际放行的本地彩金金额（金额口径）。
 */
static void TableControl_AccumulatePaid(int32_t betVal, const RoundInfo_t* ri, int32_t jpBet) 
{
	if (ri == NULL) return; // 空指针保护：无候选结果时直接返回

	// Base 派彩在所有放行类型中都可能存在（包含触发 Free/Bonus 的触发局线奖）。
	gTableControl.stats.paidBase += (int64_t)betVal * ri->nMatrixBet;

	if (ri->resType == RT_FreeWin)
	{
		gTableControl.stats.paidFree += (int64_t)betVal * ri->nFreeBet;
	}
	else if (ri->resType == RT_BonusWin)
	{
		gTableControl.stats.paidBonus += (int64_t)betVal * ri->nBonusBet;
	}
	else if (ri->resType == RT_Jackpot)
	{
		gTableControl.stats.paidJackpotBonus += (int64_t)betVal * ri->nBonusBet;
		gTableControl.stats.paidJackpot += jpBet;
	}

	// 同步窗口统计（用于闭环调参）。
	gTableControl.runtime.windowPaidBase += (int64_t)betVal * ri->nMatrixBet;
	if (ri->resType == RT_FreeWin)
	{
		gTableControl.runtime.windowPaidFree += (int64_t)betVal * ri->nFreeBet;
	}
	else if (ri->resType == RT_BonusWin)
	{
		gTableControl.runtime.windowPaidBonus += (int64_t)betVal * ri->nBonusBet;
	}
	else if (ri->resType == RT_Jackpot)
	{
		gTableControl.runtime.windowPaidJackpotBonus += (int64_t)betVal * ri->nBonusBet;
		gTableControl.runtime.windowPaidJackpot += jpBet;
	}
}

/**
 * @brief 夹断分项占比到合法区间 [0,10000]。
 * @param sharePermyriad 分项占比万分比。
 * @return 夹断后的万分比值。
 */
static int32_t TableControl_ClampShare(int32_t sharePermyriad) 
{
	if (sharePermyriad < 0)
	{
		return 0;
	}
	if (sharePermyriad > 10000)
	{
		return 10000;
	}
	return sharePermyriad;
}
/**
 * @brief 按万分比概率判定是否命中。
 * @param passPermyriad 放行概率（万分比）。
 * @return 1=命中；0=未命中。
 */
static int32_t TableControl_HitPassRate(int32_t passPermyriad) 
{
	int32_t randNum = 0; // 随机数缓存
	if (passPermyriad < 0)
	{
		return 0;
	}
	if (passPermyriad > 10000)
	{
		passPermyriad = 10000;
	}

	randNum = JRandFrom(0, 10000); 
	return (randNum <= passPermyriad) ? 1 : 0; // 大于则返回 1，否则返回 0
}
/**
 * @brief 夹断概率到合法区间 [0,10000]。
 * @param passPermyriad 输入概率（万分比）。
 * @return 夹断后的概率（万分比）。
 */
static int32_t TableControl_ClampPass(int32_t passPermyriad) 
{
	if (passPermyriad < 0) return 0;
	if (passPermyriad > 10000) return 10000;
	return passPermyriad;
}
/**
 * @brief 按万分比计算回补金额。
 * @param paidAmount 原支付金额（金额口径）。
 * @param refundPermyriad 回补比例（万分比）。
 * @return 回补金额（金额口径）。
 */
static int64_t TableControl_CalcRefundByPermyriad(int64_t paidAmount, int32_t refundPermyriad) 
{
	if (paidAmount <= 0) return 0;
	refundPermyriad = TableControl_ClampPass(refundPermyriad);
	return (paidAmount * refundPermyriad) / 10000;
}

/** 
 * @brief 软池判定并在放行后扣减池值。
 * @details 当前策略：余额够付则扣池放行；否则在不超过负债下限时「确定性」扣池放行（无随机）。
 *          历史上「缺口越大放行率越低」的随机软放行代码保留在下方注释中备查，恢复时删掉当前扣池两行并取消注释即可。
 * @param pool [in/out] 当前分项池（Base/Free/Bonus/Jackpot 对应池）。
 * @param paidAmount 本次候选应支付金额（金额口径）。
 * @param totalBet 本局总下注金额（betVal * lineNum）。
 * @param debtBetFactor 允许负债倍数，用于计算最小负债下限：-debtBetFactor * totalBet。
 * @param scaleFactor 缺口转概率缩放（随机软放行模式下使用）；当前确定性模式下未使用，仅消除告警。
 * @return 1=放行（并真实扣池）；0=拒绝（池值保持不变）。
 */
static int32_t TableControl_SoftPoolAllowAndConsume(int64_t* pool, int64_t paidAmount, int32_t totalBet, int32_t debtBetFactor, int32_t scaleFactor) 
{
	int64_t afterPaid = 0; // 支付后的池子余额（可能为负）
	int64_t debtLimit = 0; // 允许的最小负债下限
	/* 随机软放行时代曾使用：int64_t shortage; int64_t denom; int32_t softPass; */

	if (pool == NULL) return 0; // 池指针非法：拒绝
	if (paidAmount <= 0) return 1; // 无需支付：直接放行

	if (*pool >= paidAmount)
	{
		*pool -= paidAmount; // 池子足够，直接扣除应付金额
		return 1; // 放行
	}

	afterPaid = *pool - paidAmount;
	debtLimit = -(int64_t)debtBetFactor * totalBet;
	if (afterPaid < debtLimit)
	{
		return 0; // 超过负债下限，直接硬拒绝
	}

	/* ---------- 旧版：缺口随机软放行（已停用，整段保留备查；与 softPoolScale* 配合）----------
	int64_t shortage = paidAmount - *pool;
	int64_t denom = (int64_t)totalBet * scaleFactor;
	if (denom <= 0)
	{
		return 0; // 分母异常，无法计算软概率，拒绝
	}
	// 缺口越大放行率越低：softPass ≈ 10000 * (1 - shortage/(totalBet*scale))
	int32_t softPass = 10000 - (int32_t)(shortage * 10000 / denom);
	softPass = TableControl_ClampPass(softPass);
	if (!TableControl_HitPassRate(softPass))
	{
		return 0; // 软概率未命中，拒绝
	}
	*pool -= paidAmount;
	return 1;
	---------- 旧版结束；以下为当前「负债内确定性放行」 ---------- */

	(void)scaleFactor; /* 确定性模式下未参与计算；恢复随机软放行时请删本行并启用上方注释块 */
	*pool -= paidAmount; // 负债允许范围内：确定性放行并扣池
	return 1;
}
/**
 * @brief 根据总 RTP 与分项占比计算分项目标 RTP。
 * @param totalRtpPermyriad 总 RTP（万分比）。
 * @param sharePermyriad 分项占比（万分比）。
 * @return 分项目标 RTP（万分比）。
 */
static int32_t TableControl_GetShareTargetPermyriad(int32_t totalRtpPermyriad, int32_t sharePermyriad) 
{
	return (int32_t)((int64_t)totalRtpPermyriad * sharePermyriad / 10000);
}
/**
 * @brief 基于窗口实测 RTP 与目标 RTP 的偏差，调整分项概率偏移。
 * @param activeProfile 当前生效 RTP 配置；为空时不执行调参。
 */
static void TableControl_TryAdaptiveAdjust(const RtpProfileConfig* activeProfile) 
{
	int32_t targetFreeRtp = 0; // Free 分项目标 RTP
	int32_t targetBonusRtp = 0; // Bonus 分项目标 RTP
	int32_t targetJackpotRtp = 0; // Jackpot 分项目标 RTP
	int32_t curFreeRtp = 0; // Free 分项当前 RTP
	int32_t curBonusRtp = 0; // Bonus 分项当前 RTP
	int32_t curJackpotRtp = 0; // Jackpot 分项当前 RTP
	int64_t windowBonusPaid = 0; // Bonus 口径实际派彩（含 Jackpot 局内 Bonus）

	if (activeProfile == NULL) return; // 无配置时不调参
	if (gTableControl.runtime.windowRounds < gTableControl.tuning.adjustWindowRounds) return; // 未到大小时不调参
	if (gTableControl.runtime.windowBet <= 0)
	{
		gTableControl.runtime.windowRounds = 0; // 窗口下注异常，重置局数计数
		return; // 直接返回
	}

	targetFreeRtp = TableControl_GetShareTargetPermyriad(activeProfile->rtpPermyriad, activeProfile->freeSharePermyriad);
	targetBonusRtp = TableControl_GetShareTargetPermyriad(activeProfile->rtpPermyriad, activeProfile->bonusSharePermyriad);
	targetJackpotRtp = TableControl_GetShareTargetPermyriad(activeProfile->rtpPermyriad, activeProfile->jackpotSharePermyriad);
	curFreeRtp = (int32_t)(gTableControl.runtime.windowPaidFree * 10000 / gTableControl.runtime.windowBet);
	windowBonusPaid = gTableControl.runtime.windowPaidBonus + gTableControl.runtime.windowPaidJackpotBonus;
	curBonusRtp = (int32_t)(windowBonusPaid * 10000 / gTableControl.runtime.windowBet);
	curJackpotRtp = (int32_t)(gTableControl.runtime.windowPaidJackpot * 10000 / gTableControl.runtime.windowBet);

	if (curFreeRtp + gTableControl.tuning.adjustDeadbandPermyriad < targetFreeRtp)
	{
		gTableControl.runtime.adaptiveFreePassDelta += gTableControl.tuning.adjustStepPermyriad;
	}
	else if (curFreeRtp > targetFreeRtp + gTableControl.tuning.adjustDeadbandPermyriad)
	{
		gTableControl.runtime.adaptiveFreePassDelta -= gTableControl.tuning.adjustStepPermyriad;
	}

	if (curBonusRtp + gTableControl.tuning.adjustDeadbandPermyriad < targetBonusRtp)
	{
		gTableControl.runtime.adaptiveBonusPassDelta += gTableControl.tuning.adjustStepPermyriad;
	}
	else if (curBonusRtp > targetBonusRtp + gTableControl.tuning.adjustDeadbandPermyriad)
	{
		gTableControl.runtime.adaptiveBonusPassDelta -= gTableControl.tuning.adjustStepPermyriad;
	}

	if (curJackpotRtp + gTableControl.tuning.adjustDeadbandPermyriad < targetJackpotRtp)
	{
		gTableControl.runtime.adaptiveJackpotPassDelta += gTableControl.tuning.adjustStepPermyriad;
	}
	else if (curJackpotRtp > targetJackpotRtp + gTableControl.tuning.adjustDeadbandPermyriad)
	{
		gTableControl.runtime.adaptiveJackpotPassDelta -= gTableControl.tuning.adjustStepPermyriad;
	}

	if (gTableControl.runtime.adaptiveFreePassDelta > gTableControl.tuning.adaptiveDeltaMax) gTableControl.runtime.adaptiveFreePassDelta = gTableControl.tuning.adaptiveDeltaMax;
	if (gTableControl.runtime.adaptiveFreePassDelta < -gTableControl.tuning.adaptiveDeltaMax) gTableControl.runtime.adaptiveFreePassDelta = -gTableControl.tuning.adaptiveDeltaMax;
	if (gTableControl.runtime.adaptiveBonusPassDelta > gTableControl.tuning.adaptiveDeltaMax) gTableControl.runtime.adaptiveBonusPassDelta = gTableControl.tuning.adaptiveDeltaMax;
	if (gTableControl.runtime.adaptiveBonusPassDelta < -gTableControl.tuning.adaptiveDeltaMax) gTableControl.runtime.adaptiveBonusPassDelta = -gTableControl.tuning.adaptiveDeltaMax;
	if (gTableControl.runtime.adaptiveJackpotPassDelta > gTableControl.tuning.adaptiveDeltaMax) gTableControl.runtime.adaptiveJackpotPassDelta = gTableControl.tuning.adaptiveDeltaMax;
	if (gTableControl.runtime.adaptiveJackpotPassDelta < -gTableControl.tuning.adaptiveDeltaMax) gTableControl.runtime.adaptiveJackpotPassDelta = -gTableControl.tuning.adaptiveDeltaMax;

	gTableControl.runtime.windowRounds = 0; // 重置窗口局数
	//gTableControl.runtime.windowBet = 0;
	//gTableControl.runtime.windowPaidBase = 0;
	//gTableControl.runtime.windowPaidFree = 0;
	//gTableControl.runtime.windowPaidBonus = 0;
	//gTableControl.runtime.windowPaidJackpotBonus = 0;
	//gTableControl.runtime.windowPaidJackpot = 0;
}
/**
 * @brief 按当前 RTP 档位把本局下注注入四个目标池。
 * @param totalBet 本局总下注金额（betVal * lineNum）。
 * @param profile 本局使用的 RTP 档位；为空时采用默认兜底参数。
 */
static void TableControl_InjectFourPools(int32_t totalBet, const RtpProfileConfig* profile) 
{
	int32_t totalRtpPermyriad = 10000; // 默认总 RTP（无配置兜底）
	int32_t baseSharePermyriad = 2500; // 默认 Base 占比
	int32_t freeSharePermyriad = 2000; // 默认 Free 占比
	int32_t bonusSharePermyriad = 5000; // 默认 Bonus 占比
	int32_t jackpotSharePermyriad = 500; // 默认 本地Jackpot 占比
	int32_t netJackpotSharePermyriad = 0; // 默认联网 Jackpot 占比
	int64_t numerator = 0; // 注入分子临时值（带余数）

	// 二层万分比：下注 * 总 RTP * 分项占比，分母是 10000 * 10000。
	static const int64_t kPoolDenominator = 100000000LL;

	// 有配置时覆盖默认占比与 RTP
	if (profile != NULL) 
	{
		totalRtpPermyriad = profile->rtpPermyriad;
		baseSharePermyriad = profile->baseSharePermyriad;
		freeSharePermyriad = profile->freeSharePermyriad;
		bonusSharePermyriad = profile->bonusSharePermyriad;
		jackpotSharePermyriad = profile->jackpotSharePermyriad;
		netJackpotSharePermyriad = profile->netJackpotSharePermyriad;
	}
	//数值校验
	baseSharePermyriad = TableControl_ClampShare(baseSharePermyriad); 
	freeSharePermyriad = TableControl_ClampShare(freeSharePermyriad);
	bonusSharePermyriad = TableControl_ClampShare(bonusSharePermyriad); 
	jackpotSharePermyriad = TableControl_ClampShare(jackpotSharePermyriad); 
	netJackpotSharePermyriad = TableControl_ClampShare(netJackpotSharePermyriad);

	//注入 = 总下注 * 总RTP * 分项占比 / 10000 / 10000（带余数累计，避免精度损失）
	numerator = (int64_t)totalBet * totalRtpPermyriad * baseSharePermyriad + gTableControl.runtime.basePoolRemainder;
	gTableControl.runtime.basePool += numerator / kPoolDenominator;
	gTableControl.runtime.basePoolRemainder = numerator % kPoolDenominator;

	numerator = (int64_t)totalBet * totalRtpPermyriad * freeSharePermyriad + gTableControl.runtime.freePoolRemainder;
	gTableControl.runtime.freePool += numerator / kPoolDenominator;
	gTableControl.runtime.freePoolRemainder = numerator % kPoolDenominator;

	numerator = (int64_t)totalBet * totalRtpPermyriad * bonusSharePermyriad + gTableControl.runtime.bonusPoolRemainder;
	gTableControl.runtime.bonusPool += numerator / kPoolDenominator;
	gTableControl.runtime.bonusPoolRemainder = numerator % kPoolDenominator;

	numerator = (int64_t)totalBet * totalRtpPermyriad * jackpotSharePermyriad + gTableControl.runtime.jackpotPoolRemainder;
	gTableControl.runtime.jackpotPool += numerator / kPoolDenominator;
	gTableControl.runtime.jackpotPoolRemainder = numerator % kPoolDenominator;

	numerator = (int64_t)totalBet * totalRtpPermyriad * netJackpotSharePermyriad + gTableControl.runtime.netJackpotPoolRemainder;
	gTableControl.runtime.netJackpotPool += numerator / kPoolDenominator;
	gTableControl.runtime.netJackpotPoolRemainder = numerator % kPoolDenominator;
}
// 获取当前生效 RTP 配置（指定档位 -> 区域默认 -> 国内默认兜底）。
const RtpProfileConfig* TableControl_GetActiveProfile(void) 
{
	const RtpProfileConfig* profile = NULL;
	// 若指定了 RTP 档位，优先按指定档位查找
	if (gTableControl.runtime.rtpPermyriad > 0) 
	{
		profile = RtpConfig_FindProfile(gTableControl.runtime.region, gTableControl.runtime.rtpPermyriad);
	}
	// 指定档位无效或未设置，则回退到区域默认档位
	if (profile == NULL) 
	{
		profile = RtpConfig_GetDefaultProfile(gTableControl.runtime.region); 
	}
	// 找到合法配置直接返回
	if (profile != NULL) return profile;
	// 回退到国内默认档位，避免空指针导致流程中断。
	return RtpConfig_GetDefaultProfile(RTP_REGION_DOMESTIC); // 双重兜底：国内默认档
}
// 记录开局下注统计
void TableControl_OnFireWeapon(player_data_item* pUserInfo, int32_t betVal, int32_t lineNum) // 开局阶段：记录下注统计
{
	(void)pUserInfo;
	gTableControl.stats.totalBet += (int64_t)betVal * lineNum; // 记录按线权重后的投注累计
	gTableControl.runtime.windowBet += (int64_t)betVal * lineNum;                  // 调参窗口同步累计下注
}
// 区间和概率判定
int32_t TableControl_GetShotResult(player_data_item* pUserInfo, int32_t betVal, int64_t fishValue, int32_t lineNum, const RoundInfo_t* ri, const RtpProfileConfig* profile, uint8_t* jpType, int32_t* jpBet) // 判定候选结果是否放行（含本地JP）
{
	int32_t TotalBet = betVal * lineNum; // 本局总下注
	int64_t paidAmount = 0; // 当前候选类型对应应付金额
	const RtpProfileConfig* activeProfile = profile; // 当前使用的 RTP 配置

	if (activeProfile == NULL) // 若调用方未显式传配置，内部取当前生效配置
	{
		activeProfile = TableControl_GetActiveProfile();
	}

	// 四池按每局下注持续注入，避免只有命中某类型时才“进水”造成抖动。
	TableControl_InjectFourPools(TotalBet, activeProfile);
	gTableControl.stats.totalFishValue += fishValue; // 记录累计

// 调试模式中候选结果一律放行。
#ifdef _DebugControlMode
	
	if (gDebugControlMode.mode == DCM_PointResData)
	{
		return 1;
	}
#endif
//展示模式中候选结果一律放行。
#ifdef _ExhibitionMode
	return 1;
#endif

	// 无候选结果信息时默认放行
	if (ri == NULL) 
	{
		return 1;
	}

	// 本地 JP 是 RT_Jackpot 专属支付，其他结果类型统一清空候选 JP，避免串到后续统计。
	if (ri->resType != RT_Jackpot)
	{
		TableControl_RejectJackpotAndRefund(jpType, jpBet);
	}

	// 对 RT_Win 使用 BasePool 判定放行：池子够付才放。
	if (ri != NULL && ri->resType == RT_Win)
	{
		paidAmount = (int64_t)betVal * ri->nMatrixBet;

		if (!TableControl_SoftPoolAllowAndConsume(&gTableControl.runtime.basePool, paidAmount, TotalBet, gTableControl.tuning.baseDebtBetFactor, gTableControl.tuning.softPoolScaleBase))
		{
			gTableControl.stats.winRejectByTargetPool++;
			return 0;
		}
	}

	// 免费,大奖,彩金区间校验
	if (activeProfile != NULL) 
	{
		// 免费倍数区间校验
		if (ri->resType == RT_FreeWin) 
		{
			int32_t freePassPermyriad = 10000;
			int64_t freePaidAmount = 0; 

			freePaidAmount = (int64_t)betVal * ri->nFreeBet; 
			if (freePaidAmount < (int64_t)activeProfile->freeMinBet * TotalBet || freePaidAmount > (int64_t)activeProfile->freeMaxBet * TotalBet) 
			{
				gTableControl.stats.freeRejectByRange++; // 记录免费区间拒绝
				return 0;                               
			}

			paidAmount = freePaidAmount;
			if (!TableControl_SoftPoolAllowAndConsume(&gTableControl.runtime.freePool, paidAmount, TotalBet, gTableControl.tuning.freeDebtBetFactor, gTableControl.tuning.softPoolScaleFree))
			{
				gTableControl.stats.freeRejectByTargetPool++; // 记录免费目标池拒绝
				return 0;                                  
			}

			// 三池通过后，再叠加概率微调（覆盖值优先）。
			freePassPermyriad = (gTableControl.runtime.overrideFreePass >= 0) ? gTableControl.runtime.overrideFreePass : activeProfile->freePassPermyriad;
			freePassPermyriad = TableControl_ClampPass(freePassPermyriad + gTableControl.runtime.adaptiveFreePassDelta);
			if (!TableControl_HitPassRate(freePassPermyriad))
			{
				// 概率拒绝时只部分回补，剩余部分作为难度消耗。
				if (paidAmount > 0)
				{
					gTableControl.runtime.freePool += TableControl_CalcRefundByPermyriad(paidAmount, gTableControl.tuning.freePassRejectRefundPermyriad);
				}
				gTableControl.stats.freeRejectByPassRate++;
				return 0;
			}
		}
		else if (ri->resType == RT_BonusWin) 
		{
			int32_t bonusPassPermyriad = 10000;
			int64_t bonusPaidAmount = 0; 

			// Bonus 倍数区间校验
			bonusPaidAmount = (int64_t)betVal * ri->nBonusBet; 
			if (bonusPaidAmount < (int64_t)activeProfile->bonusMinBet * TotalBet || bonusPaidAmount > (int64_t)activeProfile->bonusMaxBet * TotalBet) 
			{
				gTableControl.stats.bonusRejectByRange++; // 记录 Bonus 区间拒绝
				return 0;                              
			}

			paidAmount = bonusPaidAmount;
			if (!TableControl_SoftPoolAllowAndConsume(&gTableControl.runtime.bonusPool, paidAmount, TotalBet, gTableControl.tuning.bonusDebtBetFactor, gTableControl.tuning.softPoolScaleBonus))
			{
				gTableControl.stats.bonusRejectByTargetPool++; // 记录 Bonus 目标池拒绝
				return 0;                                   
			}

			//三池通过后，再叠加概率微调。
			bonusPassPermyriad = (gTableControl.runtime.overrideBonusPass >= 0) ? gTableControl.runtime.overrideBonusPass : activeProfile->bonusPassPermyriad;
			bonusPassPermyriad = TableControl_ClampPass(bonusPassPermyriad + gTableControl.runtime.adaptiveBonusPassDelta);
			if (!TableControl_HitPassRate(bonusPassPermyriad))
			{
				// 概率拒绝时只部分回补，剩余部分作为难度消耗。
				if (paidAmount > 0)
				{
					gTableControl.runtime.bonusPool += TableControl_CalcRefundByPermyriad(paidAmount, gTableControl.tuning.bonusPassRejectRefundPermyriad);
				}
				gTableControl.stats.bonusRejectByPassRate++;
				return 0;
			}
		}
		else if (ri->resType == RT_Jackpot)
		{
			int32_t jackpotPassPermyriad = 10000;
			int64_t jackpotBonusPaidAmount = 0;
			int64_t jackpotPaidAmount = 0;

			//对本地彩金中生成的bonusbet做四池判断
			jackpotBonusPaidAmount = (int64_t)betVal * ri->nBonusBet;
			if (jackpotBonusPaidAmount > 0)
			{
				//应该不用bonus最大最小倍数判断，暂时保留接口
				/*if (jackpotBonusPaidAmount < (int64_t)activeProfile->bonusMinBet * TotalBet || jackpotBonusPaidAmount > (int64_t)activeProfile->bonusMaxBet * TotalBet)
				{
					gTableControl.stats.bonusRejectByRange++;
					TableControl_RejectJackpotAndRefund(jpType, jpBet);
					return 0;
				}*/
				if (!TableControl_SoftPoolAllowAndConsume(&gTableControl.runtime.bonusPool, jackpotBonusPaidAmount, TotalBet, gTableControl.tuning.bonusDebtBetFactor, gTableControl.tuning.softPoolScaleBonus))
				{
					gTableControl.stats.bonusRejectByTargetPool++;
					TableControl_RejectJackpotAndRefund(jpType, jpBet);
					return 0;
				}
			}

			if (jpBet != NULL && *jpBet > 0)
			{
				jackpotPaidAmount = *jpBet;
				if (jackpotPaidAmount < (int64_t)activeProfile->jackpotMinBet * TotalBet || jackpotPaidAmount > (int64_t)activeProfile->jackpotMaxBet * TotalBet)
				{
					gTableControl.stats.jackpotRejectByRange++;
					TableControl_RejectJackpotAndRefund(jpType, jpBet);
					return 0;
				}

				if (!TableControl_SoftPoolAllowAndConsume(&gTableControl.runtime.jackpotPool, jackpotPaidAmount, TotalBet, gTableControl.tuning.jackpotDebtBetFactor, gTableControl.tuning.softPoolScaleJackpot))
				{
					gTableControl.stats.jackpotRejectByTargetPool++;
					TableControl_RejectJackpotAndRefund(jpType, jpBet);
					return 0;
				}

				jackpotPassPermyriad = (gTableControl.runtime.overrideJackpotPass >= 0) ? gTableControl.runtime.overrideJackpotPass : activeProfile->jackpotPassPermyriad;
				jackpotPassPermyriad = TableControl_ClampPass(jackpotPassPermyriad + gTableControl.runtime.adaptiveJackpotPassDelta);
				if (!TableControl_HitPassRate(jackpotPassPermyriad))
				{
					gTableControl.stats.jackpotRejectByPassRate++;
					TableControl_RejectJackpotAndRefund(jpType, jpBet);
					return 0;
				}
			}
		}
	}

	TableControl_AccumulatePaid(betVal, ri, (jpBet != NULL && *jpBet > 0) ? *jpBet : 0);
	return  1;
}
// 统计拒绝和放行次数。
void TableControl_OnEndShot(player_data_item* pUserInfo, int32_t bRes, int64_t fishValue)
{
	const RtpProfileConfig* activeProfile = NULL;
	(void)pUserInfo;
	(void)fishValue;

	if (bRes) gTableControl.stats.totalPass++;  // 放行计数 +1
	else gTableControl.stats.totalReject++;     // 拒绝计数 +1

	gTableControl.runtime.windowRounds++; // 每局推进一次窗口计数
	activeProfile = TableControl_GetActiveProfile();
	TableControl_TryAdaptiveAdjust(activeProfile);
	TableControl_FlushRuntimePersist();
}
// 联网彩金派彩统计：记录命中/金额，并消费联网预算池。
void TableControl_OnJackpotOnlineWin(int32_t jpOnlineBet)
{
	if (jpOnlineBet <= 0)
	{
		return;
	}

	gTableControl.stats.paidNetJackpot += jpOnlineBet;
	gTableControl.stats.netJackpotHitCount++;
	//预算池不足次数,仅作统计
	if (gTableControl.runtime.netJackpotPool < jpOnlineBet)
	{
		gTableControl.stats.netJackpotOverBudgetCount++;
	}
	gTableControl.runtime.netJackpotPool -= jpOnlineBet;
	TableControl_FlushRuntimePersist();
}

/** 将 rtpPermyriad<=0 解析为区域默认档位的实际 RTP 万分比（用于判断档位是否变化） */
static int32_t TableControl_GetEffectiveRtpPermyriad(uint8_t region, int32_t rtpPermyriad)
{
	const RtpProfileConfig* profile = NULL;

	if (rtpPermyriad > 0) return rtpPermyriad;

	profile = RtpConfig_GetDefaultProfile(region);
	return (profile != NULL) ? profile->rtpPermyriad : 0;
}

// 设置区域与 RTP 档位；rtpPermyriad<=0 表示使用区域默认档。
int32_t TableControl_SetRtpDifficulty(uint8_t region, int32_t rtpPermyriad) 
{
	const RtpProfileConfig* profile = NULL; // 目标 RTP 档位配置指针
	uint8_t oldRegion = gTableControl.runtime.region;
	int32_t oldRtpPermyriad = gTableControl.runtime.rtpPermyriad;
	int32_t oldEffectiveRtp = 0;
	int32_t newEffectiveRtp = 0;

	if (region != RTP_REGION_DOMESTIC && region != RTP_REGION_OVERSEAS) // 区域参数合法性校验
	{
		return 0; // 区域非法，设置失败
	}

	if (rtpPermyriad <= 0) // 小于等于 0 表示使用当前区域默认 RTP 档位
	{
		gTableControl.runtime.region = region;
		gTableControl.runtime.rtpPermyriad = 0;
	}
	else
	{
		profile = RtpConfig_FindProfile(region, rtpPermyriad); // 校验目标档位是否存在
		if (profile == NULL)
		{
			return 0; // 档位不存在，设置失败
		}

		gTableControl.runtime.region = region;
		gTableControl.runtime.rtpPermyriad = rtpPermyriad;
	}

	oldEffectiveRtp = TableControl_GetEffectiveRtpPermyriad(oldRegion, oldRtpPermyriad);
	newEffectiveRtp = TableControl_GetEffectiveRtpPermyriad(gTableControl.runtime.region, gTableControl.runtime.rtpPermyriad);
	/* 仅当区域或有效 RTP 档位真正变化时才清零池子，避免 20000 重复初始化冲掉 Flash 恢复值 */
	if (oldRegion != gTableControl.runtime.region || oldEffectiveRtp != newEffectiveRtp)
	{
		TableControl_ResetPoolsAdaptiveAndWindow();
	}

	TableControl_FlushRuntimePersist();
	return 1;
}
// 按等级设置当前区域 RTP 档位（1~5，对应配置数组从低到高 RTP 档）。
int32_t TableControl_SetDifficultyLevel(int32_t level) 
{
	const RtpProfileConfig* profiles = NULL; // 当前区域档位数组
	int32_t count = 0; // 当前区域档位数量
	int32_t defaultRtpPermyriad = 0; // 区域默认 RTP（仅作为接口出参）

	if (level < 1 || level > 5)
	{
		return 0;
	}

	if (!RtpConfig_GetProfiles(gTableControl.runtime.region, &profiles, &count, &defaultRtpPermyriad))
	{
		return 0;
	}

	if (profiles == NULL || level > count)
	{
		return 0;
	}

	return TableControl_SetRtpDifficulty(gTableControl.runtime.region, profiles[level - 1].rtpPermyriad);
}
// 调试用的:设置免费和Bonus 放行概率。
void TableControl_SetRtpPassOverride(int32_t freePassPermyriad, int32_t bonusPassPermyriad) 
{
	gTableControl.runtime.overrideFreePass = (freePassPermyriad < 0) ? -1 : freePassPermyriad;
	gTableControl.runtime.overrideBonusPass = (bonusPassPermyriad < 0) ? -1 : bonusPassPermyriad;
	TableControl_FlushRuntimePersist();
}
// 调试用的:设置 Jackpot 放行概率。
void TableControl_SetJackpotPassOverride(int32_t jackpotPassPermyriad)
{
	gTableControl.runtime.overrideJackpotPass = (jackpotPassPermyriad < 0) ? -1 : jackpotPassPermyriad;
	TableControl_FlushRuntimePersist();
}
// 读取当前生效难度配置（覆盖值优先于档位默认值）
void TableControl_GetRtpDifficulty(RtpProfileConfig* outProfile) 
{
	const RtpProfileConfig* profile = TableControl_GetActiveProfile(); // 获取当前生效档位
	if (outProfile == NULL) return;                                   

	memset(outProfile, 0, sizeof(*outProfile)); // 先清零输出结构体
	if (profile != NULL)
	{
		*outProfile = *profile; // 拷贝当前生效档位配置
	}

	// 若存在概率，返回“当前生效值”。
	if (gTableControl.runtime.overrideFreePass >= 0) outProfile->freePassPermyriad = gTableControl.runtime.overrideFreePass;
	if (gTableControl.runtime.overrideBonusPass >= 0) outProfile->bonusPassPermyriad = gTableControl.runtime.overrideBonusPass;
	if (gTableControl.runtime.overrideJackpotPass >= 0) outProfile->jackpotPassPermyriad = gTableControl.runtime.overrideJackpotPass;
}
// 导出统计信息。
void TableControl_GetStats(TableControlStats* outStats) // 导出闸门统计
{
	if (outStats == NULL) return;
	*outStats = gTableControl.stats;
}

int64_t TableControl_GetPool(int8_t id)
{
	int64_t pool = 0;
	switch (id)
	{
	case 0:
		pool = gTableControl.runtime.basePool;
		break;
	case 1:
		pool = gTableControl.runtime.freePool;
		break;
	case 2:
		pool = gTableControl.runtime.bonusPool;
		break;
	case 3:
		pool = gTableControl.runtime.jackpotPool;
		break;
	case 4:
		pool = gTableControl.runtime.netJackpotPool;
		break;
	}
	
	return pool;
}
