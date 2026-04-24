#include "TableControl.h" 

#include "../GameAlgo/common/JRand.h" 
#include <string.h> 
#include "Test.h"
#include "ComputerData.h"

static TableControlStats gTableControlStats = { 0 }; // 闸门统计全局实例
static uint8_t gRtpRegion = RTP_REGION_DOMESTIC;     // 当前区域（默认国内）
static int32_t gRtpPermyriad = 0;                    // 当前 RTP 档位，0 表示使用区域默认档
static int32_t gOverrideFreePass = -1;               // 免费放行概率
static int32_t gOverrideBonusPass = -1;              // Bonus 放行概率
static int32_t gOverrideJackpotPass = -1;            // Jackpot 放行概率
static int64_t gBasePool = 0;                        // Base 目标池（用于 RT_Win 放行判定）
static int64_t gFreePool = 0;                        // Free 目标池（用于 RT_FreeWin 放行判定）
static int64_t gBonusPool = 0;                       // Bonus 目标池（用于 RT_BonusWin 放行判定）
static int64_t gJackpotPool = 0;                     // Jackpot 目标池（用于本地彩金放行判定）
static int64_t gNetJackpotPool = 0;                  // 联网 Jackpot 预算池（用于联网彩金 RTP 预算留存）
static int64_t gBasePoolRemainder = 0;               // Base 目标池注入余量
static int64_t gFreePoolRemainder = 0;               // Free 目标池注入余量
static int64_t gBonusPoolRemainder = 0;              // Bonus 目标池注入余量
static int64_t gJackpotPoolRemainder = 0;            // Jackpot 目标池注入余量
static int64_t gNetJackpotPoolRemainder = 0;         // 联网 Jackpot 目标池注入余量
static int32_t gAdaptiveFreePassDelta = 0;           // Free 自适应概率偏移（万分比）
static int32_t gAdaptiveBonusPassDelta = 0;          // Bonus 自适应概率偏移（万分比）
static int32_t gAdaptiveJackpotPassDelta = 0;        // Jackpot 自适应概率偏移（万分比）
static int64_t gWindowBet = 0;                       // 调参窗口累计下注
static int64_t gWindowPaidBase = 0;                  // 调参窗口 Base 派彩
static int64_t gWindowPaidFree = 0;                  // 调参窗口 Free 派彩
static int64_t gWindowPaidBonus = 0;                 // 调参窗口 Bonus 派彩
static int64_t gWindowPaidJackpotBonus = 0;          // 调参窗口 Jackpot 局内 Bonus 派彩
static int64_t gWindowPaidJackpot = 0;               // 调参窗口 Jackpot 派彩
static int32_t gWindowRounds = 0;                    // 调参窗口累计局数

// 闭环参数：5 万局一次小步调参，避免大幅震荡。 //现使用概率门已经来微调已经足够了,所以目前没有启动
static const int32_t kAdjustWindowRounds = 1000;
static const int32_t kAdjustStepPermyriad = 0;      // 单次微调步长：0.2%
static const int32_t kAdjustDeadbandPermyriad = 30;  // 死区：0.3%
static const int32_t kAdaptiveDeltaMax = 2500;       // 最大偏移：25%

// 软池参数：池子不够时允许一定负债并按缺口降概率放行，降低硬拒绝抖动。
static const int32_t kBaseDebtBetFactor = 100;       // Base 最大负债 = -100 * TotalBet
static const int32_t kFreeDebtBetFactor = 300;       // Free 最大负债 = -300 * TotalBet
static const int32_t kBonusDebtBetFactor = 200;      // Bonus 最大负债 = -200 * TotalBet
static const int32_t kJackpotDebtBetFactor = 260;    // Jackpot 最大负债 = -260 * TotalBet
static const int32_t kSoftPoolScaleBase = 35;        // 缺口转概率缩放（Base）
static const int32_t kSoftPoolScaleFree = 45;        // 缺口转概率缩放（Free）
static const int32_t kSoftPoolScaleBonus = 40;       // 缺口转概率缩放（Bonus）
static const int32_t kSoftPoolScaleJackpot = 50;     // 缺口转概率缩放（Jackpot）
//Pass 拒绝时不全额回补，让 passRate 对 RTP 有实际调节杠杆。
static const int32_t kFreePassRejectRefundPermyriad = 5000;   // Free 拒绝回补 50%
static const int32_t kBonusPassRejectRefundPermyriad = 5000;  // Bonus 拒绝回补 50%
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
	gTableControlStats.paidBase += (int64_t)betVal * ri->nMatrixBet;

	if (ri->resType == RT_FreeWin)
	{
		gTableControlStats.paidFree += (int64_t)betVal * ri->nFreeBet;
	}
	else if (ri->resType == RT_BonusWin)
	{
		gTableControlStats.paidBonus += (int64_t)betVal * ri->nBonusBet;
	}
	else if (ri->resType == RT_Jackpot)
	{
		gTableControlStats.paidJackpotBonus += (int64_t)betVal * ri->nBonusBet;
		gTableControlStats.paidJackpot += jpBet;
	}

	// 同步窗口统计（用于闭环调参）。
	gWindowPaidBase += (int64_t)betVal * ri->nMatrixBet;
	if (ri->resType == RT_FreeWin)
	{
		gWindowPaidFree += (int64_t)betVal * ri->nFreeBet;
	}
	else if (ri->resType == RT_BonusWin)
	{
		gWindowPaidBonus += (int64_t)betVal * ri->nBonusBet;
	}
	else if (ri->resType == RT_Jackpot)
	{
		gWindowPaidJackpotBonus += (int64_t)betVal * ri->nBonusBet;
		gWindowPaidJackpot += jpBet;
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
 * @param pool [in/out] 当前分项池（Base/Free/Bonus/Jackpot 对应池）。
 * @param paidAmount 本次候选应支付金额（金额口径）。
 * @param totalBet 本局总下注金额（betVal * lineNum）。
 * @param debtBetFactor 允许负债倍数，用于计算最小负债下限：-debtBetFactor * totalBet。
 * @param scaleFactor 缺口转概率缩放因子，越大表示缺口导致的降概率越缓。
 * @return 1=放行（并真实扣池）；0=拒绝（池值保持不变）。
 */
static int32_t TableControl_SoftPoolAllowAndConsume(int64_t* pool, int64_t paidAmount, int32_t totalBet, int32_t debtBetFactor, int32_t scaleFactor) 
{
	int64_t afterPaid = 0; // 支付后的池子余额（可能为负）
	int64_t debtLimit = 0; // 允许的最小负债下限
	int64_t shortage = 0; // 当前缺口
	int64_t denom = 0; // 缺口转概率分母
	int32_t softPass = 0; // 软放行概率

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

	shortage = paidAmount - *pool;
	denom = (int64_t)totalBet * scaleFactor;
	if (denom <= 0)
	{
		return 0; // 分母异常，无法计算软概率，拒绝
	}

	// 缺口越大放行率越低：softPass = 1 - shortage/(totalBet*scale)
	softPass = 10000 - (int32_t)(shortage * 10000 / denom);
	softPass = TableControl_ClampPass(softPass);
	if (!TableControl_HitPassRate(softPass))
	{
		return 0; // 软概率未命中，拒绝
	}

	*pool -= paidAmount; // 命中软概率后再真实扣池
	return 1; // 放行
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
	if (gWindowRounds < kAdjustWindowRounds) return; // 未到大小时不调参
	if (gWindowBet <= 0)
	{
		gWindowRounds = 0; // 窗口下注异常，重置局数计数
		return; // 直接返回
	}

	targetFreeRtp = TableControl_GetShareTargetPermyriad(activeProfile->rtpPermyriad, activeProfile->freeSharePermyriad);
	targetBonusRtp = TableControl_GetShareTargetPermyriad(activeProfile->rtpPermyriad, activeProfile->bonusSharePermyriad);
	targetJackpotRtp = TableControl_GetShareTargetPermyriad(activeProfile->rtpPermyriad, activeProfile->jackpotSharePermyriad);
	curFreeRtp = (int32_t)(gWindowPaidFree * 10000 / gWindowBet);
	windowBonusPaid = gWindowPaidBonus + gWindowPaidJackpotBonus;
	curBonusRtp = (int32_t)(windowBonusPaid * 10000 / gWindowBet);
	curJackpotRtp = (int32_t)(gWindowPaidJackpot * 10000 / gWindowBet);

	//目前没有进行自适应偏移调整
	if (curFreeRtp + kAdjustDeadbandPermyriad < targetFreeRtp)
	{
		gAdaptiveFreePassDelta += kAdjustStepPermyriad;
	}
	else if (curFreeRtp > targetFreeRtp + kAdjustDeadbandPermyriad)
	{
		gAdaptiveFreePassDelta -= kAdjustStepPermyriad;
	}

	if (curBonusRtp + kAdjustDeadbandPermyriad < targetBonusRtp)
	{
		gAdaptiveBonusPassDelta += kAdjustStepPermyriad;
	}
	else if (curBonusRtp > targetBonusRtp + kAdjustDeadbandPermyriad)
	{
		gAdaptiveBonusPassDelta -= kAdjustStepPermyriad;
	}
	if (curJackpotRtp + kAdjustDeadbandPermyriad < targetJackpotRtp)
	{
		gAdaptiveJackpotPassDelta += kAdjustStepPermyriad;
	}
	else if (curJackpotRtp > targetJackpotRtp + kAdjustDeadbandPermyriad)
	{
		gAdaptiveJackpotPassDelta -= kAdjustStepPermyriad;
	}

	if (gAdaptiveFreePassDelta > kAdaptiveDeltaMax) gAdaptiveFreePassDelta = kAdaptiveDeltaMax; // Free 偏移上限保护
	if (gAdaptiveFreePassDelta < -kAdaptiveDeltaMax) gAdaptiveFreePassDelta = -kAdaptiveDeltaMax; // Free 偏移下限保护
	if (gAdaptiveBonusPassDelta > kAdaptiveDeltaMax) gAdaptiveBonusPassDelta = kAdaptiveDeltaMax; // Bonus 偏移上限保护
	if (gAdaptiveBonusPassDelta < -kAdaptiveDeltaMax) gAdaptiveBonusPassDelta = -kAdaptiveDeltaMax; // Bonus 偏移下限保护
	if (gAdaptiveJackpotPassDelta > kAdaptiveDeltaMax) gAdaptiveJackpotPassDelta = kAdaptiveDeltaMax; // Jackpot 偏移上限保护
	if (gAdaptiveJackpotPassDelta < -kAdaptiveDeltaMax) gAdaptiveJackpotPassDelta = -kAdaptiveDeltaMax; // Jackpot 偏移下限保护

	gWindowRounds = 0; // 重置窗口局数
	gWindowBet = 0; // 重置窗口下注
	gWindowPaidBase = 0; // 重置窗口 Base 派彩
	gWindowPaidFree = 0; // 重置窗口 Free 派彩
	gWindowPaidBonus = 0; // 重置窗口 Bonus 派彩
	gWindowPaidJackpot = 0; // 重置窗口 Jackpot 派彩
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
	numerator = (int64_t)totalBet * totalRtpPermyriad * baseSharePermyriad + gBasePoolRemainder;
	gBasePool += numerator / kPoolDenominator;
	gBasePoolRemainder = numerator % kPoolDenominator;

	numerator = (int64_t)totalBet * totalRtpPermyriad * freeSharePermyriad + gFreePoolRemainder;
	gFreePool += numerator / kPoolDenominator;
	gFreePoolRemainder = numerator % kPoolDenominator;

	numerator = (int64_t)totalBet * totalRtpPermyriad * bonusSharePermyriad + gBonusPoolRemainder;
	gBonusPool += numerator / kPoolDenominator;
	gBonusPoolRemainder = numerator % kPoolDenominator;

	numerator = (int64_t)totalBet * totalRtpPermyriad * jackpotSharePermyriad + gJackpotPoolRemainder;
	gJackpotPool += numerator / kPoolDenominator;
	gJackpotPoolRemainder = numerator % kPoolDenominator;

	numerator = (int64_t)totalBet * totalRtpPermyriad * netJackpotSharePermyriad + gNetJackpotPoolRemainder;
	gNetJackpotPool += numerator / kPoolDenominator;
	gNetJackpotPoolRemainder = numerator % kPoolDenominator;
}
// 获取当前生效 RTP 配置（指定档位 -> 区域默认 -> 国内默认兜底）。
const RtpProfileConfig* TableControl_GetActiveProfile(void) 
{
	const RtpProfileConfig* profile = NULL;
	// 若指定了 RTP 档位，优先按指定档位查找
	if (gRtpPermyriad > 0) 
	{
		profile = RtpConfig_FindProfile(gRtpRegion, gRtpPermyriad);
	}
	// 指定档位无效或未设置，则回退到区域默认档位
	if (profile == NULL) 
	{
		profile = RtpConfig_GetDefaultProfile(gRtpRegion); 
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
	gTableControlStats.totalBet += (int64_t)betVal * lineNum; // 记录按线权重后的投注累计
	gWindowBet += (int64_t)betVal * lineNum;                  // 调参窗口同步累计下注
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
	gTableControlStats.totalFishValue += fishValue; // 记录累计

#ifdef _DebugControlMode
	// 调试模式中候选结果一律放行。
	if (gDebugControlMode.mode == DCM_PointResData)
	{
		return 1;
	}
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

	if (ri != NULL && ri->resType == RT_Lose)
	{
	}
	else if (ri != NULL && ri->resType == RT_Jackpot)
	{
	}

	// 对 RT_Win 使用 BasePool 判定放行：池子够付才放。
	if (ri != NULL && ri->resType == RT_Win)
	{
		paidAmount = (int64_t)betVal * ri->nMatrixBet;

		if (TableControl_SoftPoolAllowAndConsume(&gBasePool, paidAmount, TotalBet, kBaseDebtBetFactor, kSoftPoolScaleBase))
		{

		}
		else
		{
			gTableControlStats.winRejectByTargetPool++;
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
				gTableControlStats.freeRejectByRange++; // 记录免费区间拒绝
				return 0;                               
			}

			paidAmount = freePaidAmount;
			if (!TableControl_SoftPoolAllowAndConsume(&gFreePool, paidAmount, TotalBet, kFreeDebtBetFactor, kSoftPoolScaleFree))
			{
				gTableControlStats.freeRejectByTargetPool++; // 记录免费目标池拒绝
				return 0;                                  
			}

			// 三池通过后，再叠加概率微调（覆盖值优先）。
			freePassPermyriad = (gOverrideFreePass >= 0) ? gOverrideFreePass : activeProfile->freePassPermyriad;
			freePassPermyriad = TableControl_ClampPass(freePassPermyriad + gAdaptiveFreePassDelta);
			if (!TableControl_HitPassRate(freePassPermyriad))
			{
				// 概率拒绝时只部分回补，剩余部分作为难度消耗。
				if (paidAmount > 0)
				{
					gFreePool += TableControl_CalcRefundByPermyriad(paidAmount, kFreePassRejectRefundPermyriad);
				}
				gTableControlStats.freeRejectByPassRate++;
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
				gTableControlStats.bonusRejectByRange++; // 记录 Bonus 区间拒绝
				return 0;                              
			}

			paidAmount = bonusPaidAmount;
			if (!TableControl_SoftPoolAllowAndConsume(&gBonusPool, paidAmount, TotalBet, kBonusDebtBetFactor, kSoftPoolScaleBonus))
			{
				gTableControlStats.bonusRejectByTargetPool++; // 记录 Bonus 目标池拒绝
				return 0;                                   
			}

			//三池通过后，再叠加概率微调。
			bonusPassPermyriad = (gOverrideBonusPass >= 0) ? gOverrideBonusPass : activeProfile->bonusPassPermyriad;
			bonusPassPermyriad = TableControl_ClampPass(bonusPassPermyriad + gAdaptiveBonusPassDelta);
			if (!TableControl_HitPassRate(bonusPassPermyriad))
			{
				// 概率拒绝时只部分回补，剩余部分作为难度消耗。
				if (paidAmount > 0)
				{
					gBonusPool += TableControl_CalcRefundByPermyriad(paidAmount, kBonusPassRejectRefundPermyriad);
				}
				gTableControlStats.bonusRejectByPassRate++;
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
					gTableControlStats.bonusRejectByRange++;
					TableControl_RejectJackpotAndRefund(jpType, jpBet);
					return 0;
				}*/
				if (!TableControl_SoftPoolAllowAndConsume(&gBonusPool, jackpotBonusPaidAmount, TotalBet, kBonusDebtBetFactor, kSoftPoolScaleBonus))
				{
					gTableControlStats.bonusRejectByTargetPool++;
					TableControl_RejectJackpotAndRefund(jpType, jpBet);
					return 0;
				}
			}

			if (jpBet != NULL && *jpBet > 0)
			{
				jackpotPaidAmount = *jpBet;
				if (jackpotPaidAmount < (int64_t)activeProfile->jackpotMinBet * TotalBet || jackpotPaidAmount > (int64_t)activeProfile->jackpotMaxBet * TotalBet)
				{
					gTableControlStats.jackpotRejectByRange++;
					TableControl_RejectJackpotAndRefund(jpType, jpBet);
					return 0;
				}

				if (!TableControl_SoftPoolAllowAndConsume(&gJackpotPool, jackpotPaidAmount, TotalBet, kJackpotDebtBetFactor, kSoftPoolScaleJackpot))
				{
					gTableControlStats.jackpotRejectByTargetPool++;
					TableControl_RejectJackpotAndRefund(jpType, jpBet);
					return 0;
				}

				jackpotPassPermyriad = (gOverrideJackpotPass >= 0) ? gOverrideJackpotPass : activeProfile->jackpotPassPermyriad;
				jackpotPassPermyriad = TableControl_ClampPass(jackpotPassPermyriad + gAdaptiveJackpotPassDelta);
				if (!TableControl_HitPassRate(jackpotPassPermyriad))
				{
					gTableControlStats.jackpotRejectByPassRate++;
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

	if (bRes) gTableControlStats.totalPass++;  // 放行计数 +1
	else gTableControlStats.totalReject++;     // 拒绝计数 +1

	gWindowRounds++; // 每局推进一次窗口计数
	activeProfile = TableControl_GetActiveProfile();
	TableControl_TryAdaptiveAdjust(activeProfile);
}
// 联网彩金派彩统计：记录命中/金额，并消费联网预算池。
void TableControl_OnJackpotOnlineWin(int32_t jpOnlineBet)
{
	if (jpOnlineBet <= 0)
	{
		return;
	}

	gTableControlStats.paidNetJackpot += jpOnlineBet;
	gTableControlStats.netJackpotHitCount++;
	//预算池不足次数,仅作统计
	if (gNetJackpotPool < jpOnlineBet)
	{
		gTableControlStats.netJackpotOverBudgetCount++;
	}
	gNetJackpotPool -= jpOnlineBet;
}
// 设置区域与 RTP 档位；rtpPermyriad<=0 表示使用区域默认档。
int32_t TableControl_SetRtpDifficulty(uint8_t region, int32_t rtpPermyriad) 
{
	const RtpProfileConfig* profile = NULL; // 目标 RTP 档位配置指针
	if (region != RTP_REGION_DOMESTIC && region != RTP_REGION_OVERSEAS) // 区域参数合法性校验
	{
		return 0; // 区域非法，设置失败
	}

	if (rtpPermyriad <= 0) // 小于等于 0 表示使用当前区域默认 RTP 档位
	{
		gRtpRegion = region;  // 更新区域
		gRtpPermyriad = 0;    // 清除指定档位，回到默认档
		gBasePool = 0;        // 切档后重置 Base 目标池
		gFreePool = 0;        // 切档后重置 Free 目标池
		gBonusPool = 0;       // 切档后重置 Bonus 目标池
		gJackpotPool = 0;     // 切档后重置 Jackpot 目标池
		gNetJackpotPool = 0;  // 切档后重置联网 Jackpot 预算池
		gBasePoolRemainder = 0;
		gFreePoolRemainder = 0;
		gBonusPoolRemainder = 0;
		gJackpotPoolRemainder = 0;
		gNetJackpotPoolRemainder = 0;
		gAdaptiveFreePassDelta = 0;
		gAdaptiveBonusPassDelta = 0;
		gAdaptiveJackpotPassDelta = 0;
		gWindowBet = 0;
		gWindowPaidBase = 0;
		gWindowPaidFree = 0;
		gWindowPaidBonus = 0;
		gWindowPaidJackpotBonus = 0;
		gWindowPaidJackpot = 0;
		gWindowRounds = 0;
		return 1;             // 设置成功
	}

	profile = RtpConfig_FindProfile(region, rtpPermyriad); // 校验目标档位是否存在
	if (profile == NULL)
	{
		return 0; // 档位不存在，设置失败
	}

	gRtpRegion = region;           // 更新区域
	gRtpPermyriad = rtpPermyriad;  // 固定到指定 RTP 档位
	gBasePool = 0;                 // 切档后重置 Base 目标池
	gFreePool = 0;                 // 切档后重置 Free 目标池
	gBonusPool = 0;                // 切档后重置 Bonus 目标池
	gJackpotPool = 0;              // 切档后重置 Jackpot 目标池
	gNetJackpotPool = 0;           // 切档后重置联网 Jackpot 预算池
	gBasePoolRemainder = 0;
	gFreePoolRemainder = 0;
	gBonusPoolRemainder = 0;
	gJackpotPoolRemainder = 0;
	gNetJackpotPoolRemainder = 0;
	gAdaptiveFreePassDelta = 0;
	gAdaptiveBonusPassDelta = 0;
	gAdaptiveJackpotPassDelta = 0;
	gWindowBet = 0;
	gWindowPaidBase = 0;
	gWindowPaidFree = 0;
	gWindowPaidBonus = 0;
	gWindowPaidJackpotBonus = 0;
	gWindowPaidJackpot = 0;
	gWindowRounds = 0;
	return 1;                      // 设置成功
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

	if (!RtpConfig_GetProfiles(gRtpRegion, &profiles, &count, &defaultRtpPermyriad))
	{
		return 0;
	}

	if (profiles == NULL || level > count)
	{
		return 0;
	}

	return TableControl_SetRtpDifficulty(gRtpRegion, profiles[level - 1].rtpPermyriad);
}
// 调试用的:设置免费和Bonus 放行概率。
void TableControl_SetRtpPassOverride(int32_t freePassPermyriad, int32_t bonusPassPermyriad) 
{
	gOverrideFreePass = (freePassPermyriad < 0) ? -1 : freePassPermyriad;      // 设置免费概率
	gOverrideBonusPass = (bonusPassPermyriad < 0) ? -1 : bonusPassPermyriad;   // 设置Bonus概率
}
// 调试用的:设置 Jackpot 放行概率。
void TableControl_SetJackpotPassOverride(int32_t jackpotPassPermyriad)
{
	gOverrideJackpotPass = (jackpotPassPermyriad < 0) ? -1 : jackpotPassPermyriad;
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
	if (gOverrideFreePass >= 0) outProfile->freePassPermyriad = gOverrideFreePass;     // 覆盖免费概率
	if (gOverrideBonusPass >= 0) outProfile->bonusPassPermyriad = gOverrideBonusPass;   // 覆盖Bonus概率
	if (gOverrideJackpotPass >= 0) outProfile->jackpotPassPermyriad = gOverrideJackpotPass; // 覆盖Jackpot概率
}
// 导出统计信息。
void TableControl_GetStats(TableControlStats* outStats) // 导出闸门统计
{
	if (outStats == NULL) return;
	*outStats = gTableControlStats;
}
