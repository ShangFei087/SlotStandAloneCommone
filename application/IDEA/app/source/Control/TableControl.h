#ifndef _TABLE_CONTROL_H_ 
#define _TABLE_CONTROL_H_ 

#include "CMD_Fish.h" 
#include "RtpConfig.h" 
#include <idea_player.h> 
#include "CommonStruct.h" 

typedef struct TableControlStats {
	int64_t totalBet;                // 累计下注额（金额口径）
	int64_t totalFishValue;          // 累计候选理论支付值
	int64_t paidBase;                // Base 实际派彩累计（金额口径）
	int64_t paidFree;                // Free 实际派彩累计（金额口径）
	int64_t paidBonus;               // Bonus 实际派彩累计（金额口径）
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
	int64_t freeRejectByRange;       // Free 因倍数区间不合法被拒绝次数
	int64_t bonusRejectByRange;      // Bonus 因倍数区间不合法被拒绝次数
	int64_t jackpotRejectByRange;    // Jackpot 因金额区间不合法被拒绝次数
	int64_t freeRejectByPassRate;    // Free 因概率门拒绝次数
	int64_t bonusRejectByPassRate;   // Bonus 因概率门拒绝次数
	int64_t jackpotRejectByPassRate; // Jackpot 因概率门拒绝次数
} TableControlStats; // 统计结构结束

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

#endif // _TABLE_CONTROL_H_ 
