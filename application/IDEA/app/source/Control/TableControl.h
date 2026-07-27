#ifndef _TABLE_CONTROL_H_ 
#define _TABLE_CONTROL_H_ 

#include "RtpConfig.h" 
#include <idea_player.h> 
#include "CommonStruct.h" 

typedef struct TableControlStats {
	int64_t totalBet;                // 累计下注额（金额口径）
	int64_t totalFishValue;          // 累计候选理论支付值
	int64_t paidBase;                // Base 实际派彩累计（金额口径）
	int64_t paidFree;                // Free 实际派彩累计（金额口径）
	int64_t paidBonus;               // Bonus 实际派彩累计（金额口径）
	int64_t paidJackpotBonus;        // Jackpot 局内 Bonus 实际派彩累计（金额口径）
	int64_t paidJackpot;             // Jackpot 实际派彩累计（金额口径）
	int64_t paidNetJackpot;          // 联网 Jackpot 实际派彩累计（金额口径）
	int64_t netJackpotHitCount;      // 联网 Jackpot 命中次数
	int64_t netJackpotOverBudgetCount; // 联网 Jackpot 超预算次数（命中时预算池不足）
	int64_t totalPass;               // 放行次数
	int64_t totalReject;             // 拒绝次数
	int64_t winRejectByTargetPool;   // Base 因目标池不足被拒绝次数
	int64_t freeRejectByTargetPool;  // Free 因目标池不足被拒绝次数
	int64_t bonusRejectByTargetPool; // Bonus 因目标池不足被拒绝次数
	int64_t jackpotRejectByTargetPool; // Jackpot 因目标池不足被拒绝次数
	int64_t freeRejectByRange;       // Free 因倍数区间不合法被拒绝次数（= Low + High）
	int64_t bonusRejectByRange;      // Bonus 因倍数区间不合法被拒绝次数（= Low + High）
	int64_t jackpotRejectByRange;    // Jackpot 因金额区间不合法被拒绝次数（= Low + High）
	int64_t freeRejectByRangeLow;    // Free 因倍数 < Min 被拒绝次数
	int64_t freeRejectByRangeHigh;   // Free 因倍数 > Max 被拒绝次数
	int64_t bonusRejectByRangeLow;   // Bonus 因倍数 < Min 被拒绝次数
	int64_t bonusRejectByRangeHigh;  // Bonus 因倍数 > Max 被拒绝次数
	int64_t jackpotRejectByRangeLow; // Jackpot 因金额 < Min 被拒绝次数
	int64_t jackpotRejectByRangeHigh;// Jackpot 因金额 > Max 被拒绝次数
	int64_t freeRejectByPassRate;    // Free 因概率门拒绝次数
	int64_t bonusRejectByPassRate;   // Bonus 因概率门拒绝次数
	int64_t jackpotRejectByPassRate; // Jackpot 因概率门拒绝次数
} TableControlStats; // 统计结构结束

/** 闸门运行时状态：区域/档位、目标池、闭环调参窗口、概率门覆盖与自适应偏移。 */
typedef struct TableControlRuntime {
	uint8_t region;                    // 当前区域（默认国内）
	int32_t rtpPermyriad;              // 当前 RTP 档位，0 表示使用区域默认档
	int32_t overrideFreePass;          // 免费放行概率覆盖，-1 表示使用档位默认
	int32_t overrideBonusPass;         // Bonus 放行概率覆盖
	int32_t overrideJackpotPass;       // Jackpot 放行概率覆盖

	int64_t basePool;                  // Base 目标池（RT_Win 放行判定）
	int64_t freePool;                  // Free 目标池（RT_FreeWin 放行判定）
	int64_t bonusPool;                 // Bonus 目标池（RT_BonusWin 放行判定）
	int64_t jackpotPool;               // 本地 Jackpot 目标池
	int64_t netJackpotPool;            // 联网 Jackpot 预算池
	int64_t basePoolRemainder;         // Base 目标池注入余量
	int64_t freePoolRemainder;         // Free 目标池注入余量
	int64_t bonusPoolRemainder;        // Bonus 目标池注入余量
	int64_t jackpotPoolRemainder;      // Jackpot 目标池注入余量
	int64_t netJackpotPoolRemainder;   // 联网 Jackpot 目标池注入余量

	int32_t adaptiveFreePassDelta;     // Free 自适应概率偏移（万分比）
	int32_t adaptiveBonusPassDelta;    // Bonus 自适应概率偏移（万分比）
	int32_t adaptiveJackpotPassDelta;  // Jackpot 自适应概率偏移（万分比）

	int64_t windowBet;                 // 调参窗口累计下注
	int64_t windowPaidBase;            // 调参窗口 Base 派彩
	int64_t windowPaidFree;            // 调参窗口 Free 派彩
	int64_t windowPaidBonus;           // 调参窗口 Bonus 派彩
	int64_t windowPaidJackpotBonus;    // 调参窗口 Jackpot 局内 Bonus 派彩
	int64_t windowPaidJackpot;         // 调参窗口 Jackpot 派彩
	int32_t windowRounds;              // 调参窗口累计局数
} TableControlRuntime;

/** 闭环/软池/概率门等调参常量（运行时只读，切档时不重置）。 */
typedef struct TableControlTuning {
	int32_t adjustWindowRounds;          // 每满 N 局按窗口实测分项 RTP 微调概率门
	int32_t adjustStepPermyriad;         // 单次微调步长（万分比）
	int32_t adjustDeadbandPermyriad;     // 死区（万分比）
	int32_t adaptiveDeltaMax;            // 最大自适应偏移（万分比）

	int32_t baseDebtBetFactor;           // Base 最大负债 = -factor * TotalBet
	int32_t freeDebtBetFactor;           // Free 最大负债倍数
	int32_t bonusDebtBetFactor;          // Bonus 最大负债倍数
	int32_t jackpotDebtBetFactor;        // Jackpot 最大负债倍数
	int32_t softPoolScaleBase;           // 恢复随机软放行时使用（Base）
	int32_t softPoolScaleFree;           // 同上（Free）
	int32_t softPoolScaleBonus;          // 同上（Bonus）
	int32_t softPoolScaleJackpot;        // 同上（Jackpot）

	int32_t freePassRejectRefundPermyriad;   // Free 概率拒绝时池子回补比例（万分比）
	int32_t bonusPassRejectRefundPermyriad;  // Bonus 概率拒绝时池子回补比例（万分比）
} TableControlTuning;

/** 闸门模块全部状态：统计 + 运行时 + 调参常量。 */
typedef struct TableControlState {
	TableControlStats stats;
	TableControlRuntime runtime;
	TableControlTuning tuning;
} TableControlState;

/**
 * @brief 导出当前 RAM 中的闸门运行时快照。
 * @param out 输出缓冲区；为 NULL 时不写入。
 */
void TableControl_ExportRuntime(TableControlRuntime* out);
/**
 * @brief 导入闸门运行时快照到 RAM（含区域/档位/池子/窗口合法性校验）。
 * @param in 待导入快照；非法时保持 RAM 不变。
 */
void TableControl_ImportRuntime(const TableControlRuntime* in);
/**
 * @brief 恢复运行时默认值：国内区域、默认 RTP 档、清除概率覆盖，并重置池子与调参窗口。
 */
void TableControl_ResetRuntimeDefaults(void);
/**
 * @brief 重置调控状态：清五池/窗口/自适应/概率覆盖，保留区域与 RTP 档位，并写回 Flash。
 */
void TableControl_ResetRegulationKeepDifficulty(void);

// 获取当前生效 RTP 配置（优先指定档位，其次区域默认，最后国内默认兜底）。
const RtpProfileConfig* TableControl_GetActiveProfile(void);
/**
 * @brief 记录本局开局下注统计（用于累计统计和窗口调参）。
 * @param pUserInfo 当前玩家数据指针（当前实现未使用，预留给分玩家统计）。
 * @param betVal 单线下注金额（例如每条线下注 10）。
 * @param lineNum 本局生效线数（例如 20 线）。
 */
void TableControl_OnFireWeapon(player_data_item* pUserInfo, int32_t betVal, int32_t lineNum);
/**
 * @brief 对候选结果执行闸门判定（池子/区间/概率）并返回是否放行。
 * @param pUserInfo 当前玩家数据指针（用于后续扩展按玩家维度控制）。
 * @param betVal 单线下注金额。
 * @param fishValue 本局候选理论总支付（金额口径，含线奖/免费/Bonus/JP）。
 * @param lineNum 本局生效线数。
 * @param ri 候选回合结果（结果类型、线奖、免费/Bonus信息）；为 NULL 时默认放行。
 * @param profile 本次判定使用的 RTP 档位；传 NULL 时内部自动取当前生效档位。
 * @param jpType [in/out] 本地彩金类型输入/输出（可能在拒绝时被清空为 JT_None）。
 * @param jpBet [in/out] 本地彩金金额输入/输出（可能在拒绝时被清零）。
 * @return 1 表示放行候选结果；0 表示拒绝并由上层改为输局。
 */
int32_t TableControl_GetShotResult(player_data_item* pUserInfo, int32_t betVal, int64_t fishValue, int32_t lineNum, const RoundInfo_t* ri, const RtpProfileConfig* profile, uint8_t* jpType, int32_t* jpBet);
/**
 * @brief 记录本局最终放行/拒绝结果，并推进窗口化自适应调参。
 * @param pUserInfo 当前玩家数据指针（当前实现未使用，预留）。
 * @param bRes 本局判定结果：1=放行，0=拒绝。
 * @param fishValue 本局候选理论总支付（当前实现未使用，预留）。
 */
void TableControl_OnEndShot(player_data_item* pUserInfo, int32_t bRes, int64_t fishValue); 
/**
 * @brief 记录联网 Jackpot 派彩统计，并消耗联网预算池。
 * @param jpOnlineBet 本次联网 Jackpot 实际派彩金额（金额口径）。
 */
void TableControl_OnJackpotOnlineWin(int32_t jpOnlineBet);
/**
 * @brief 设置区域与 RTP 档位；rtpPermyriad<=0 表示使用区域默认档位。
 * @param region 区域标识：RTP_REGION_DOMESTIC 或 RTP_REGION_OVERSEAS。
 * @param rtpPermyriad RTP 万分比档位（例如 9920）；<=0 表示切回该区域默认档。
 * @return 1=设置成功；0=参数非法或档位不存在。
 */
int32_t TableControl_SetRtpDifficulty(uint8_t region, int32_t rtpPermyriad); 
/**
 * @brief 按等级设置当前区域 RTP 档位（1~5 从低到高）。
 * @param level 难度等级（1~5）。
 * @return 1=设置成功；0=等级越界或当前区域档位不可用。
 */
int32_t TableControl_SetDifficultyLevel(int32_t level); 
/**
 * @brief 设置 Free/Bonus 放行概率覆盖；传负数表示清除覆盖。
 * @param freePassPermyriad Free 放行率覆盖（万分比，0~10000，负数=清除覆盖）。
 * @param bonusPassPermyriad Bonus 放行率覆盖（万分比，0~10000，负数=清除覆盖）。
 */
void TableControl_SetRtpPassOverride(int32_t freePassPermyriad, int32_t bonusPassPermyriad);
/**
 * @brief 设置 Jackpot 放行概率覆盖；传负数表示清除覆盖。
 * @param jackpotPassPermyriad Jackpot 放行率覆盖（万分比，0~10000，负数=清除覆盖）。
 */
void TableControl_SetJackpotPassOverride(int32_t jackpotPassPermyriad);
/**
 * @brief 读取当前生效难度快照（覆盖值优先于档位默认值）。
 * @param outProfile 输出参数，返回当前最终生效的 RTP 档位配置快照。
 */
void TableControl_GetRtpDifficulty(RtpProfileConfig* outProfile);
/**
 * @brief 导出当前闸门统计信息。
 * @param outStats 输出参数，返回累计放行/拒绝及各类拒绝原因计数。
 */
void TableControl_GetStats(TableControlStats* outStats);
/**
 * @brief 清零闸门累计统计（仅 RAM，不参与 Flash 持久化）。
 */
void TableControl_ClearStats(void);

/**
 * @brief 导出当前闸门五池信息。
 * @param id参数。
 */
int64_t TableControl_GetPool(int8_t id);

#endif // _TABLE_CONTROL_H_ 
