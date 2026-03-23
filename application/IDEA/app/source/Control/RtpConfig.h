#ifndef _RTP_CONFIG_H_ // 头文件防重复包含：RTP 配置模块
#define _RTP_CONFIG_H_ // 定义防重复包含宏

#include <stdint.h> // 标准整型定义

// RTP 区域定义
#define RTP_REGION_DOMESTIC 0 // 国内区域
#define RTP_REGION_OVERSEAS 1 // 海外区域
#define RTP_PROFILE_COUNT 5 // 每个区域固定 5 个难度档位
#define RTP_DEFAULT_DOMESTIC 9920  // 国内默认 RTP（99.2%）
#define RTP_DEFAULT_OVERSEAS 9200  // 海外默认 RTP（92.0%）

// RTP 档位配置（万分比口径）
typedef struct RtpProfileConfig { 
    int32_t rtpPermyriad;         // 总 RTP 目标，例如 9920 = 99.2%
    int32_t baseSharePermyriad;   // Base 分项占比（万分比）
    int32_t freeSharePermyriad;   // Free 分项占比（万分比）
    int32_t bonusSharePermyriad;  // Bonus 分项占比（万分比）
    int32_t jackpotSharePermyriad; // 本地 Jackpot 分项占比（万分比）
    int32_t netJackpotSharePermyriad; // 联网 Jackpot 分项占比（万分比）
    int32_t freePassPermyriad;    // Free 概率门放行率（万分比）
    int32_t bonusPassPermyriad;   // Bonus 概率门放行率（万分比）
    int32_t jackpotPassPermyriad; // Jackpot 概率门放行率（万分比）
    int32_t freeMinBet;           // Free 总倍数下限
    int32_t freeMaxBet;           // Free 总倍数上限
    int32_t bonusMinBet;          // Bonus 总倍数下限
    int32_t bonusMaxBet;          // Bonus 总倍数上限
    int32_t jackpotMinBet;        // Jackpot 金额倍数下限（相对 TotalBet）
    int32_t jackpotMaxBet;        // Jackpot 金额倍数上限（相对 TotalBet）
} RtpProfileConfig; // RTP 档位结构结束

// 国内 RTP 档位：98.5 / 98.8 / 99.2 / 99.5 / 99.8（默认 99.2）
static const RtpProfileConfig gDomesticProfiles[] = { // 国内档位表（从低到高）
	{ 9850, 2500, 2000, 4500, 500, 500, 9000, 9000, 10000, 60, 150, 200, 600, 0, 4500 }, // Level1
	{ 9880, 2500, 2000, 4500, 500, 500, 9300, 9300, 10000, 60, 150, 200, 600, 0, 4500 }, // Level2
	{ 9920, 2500, 2000, 4500, 500, 500, 9920, 9920, 10000, 60, 150, 200, 600, 0, 4500 }, // Level3（默认档）
	{ 9950, 2500, 2000, 4500, 500, 500, 9700, 9700, 10000, 60, 150, 200, 600, 0, 4500 }, // Level4
	{ 9980, 2500, 2000, 4500, 500, 500, 9980, 9980, 10000, 60, 150, 200, 600, 0, 4500 }, // Level5
}; // 国内档位表结束

// 海外 RTP 档位：85 / 88 / 92 / 95 / 98（默认 92）
static const RtpProfileConfig gOverseasProfiles[] = { // 海外档位表（从低到高）
    { 8500, 3000, 2000, 4000, 500, 500, 7600, 7600, 7600, 30, 100, 50, 200, 0, 3000 }, // Level1
    { 8800, 3000, 2000, 4000, 500, 500, 8200, 8200, 8200, 30, 100, 50, 200, 0, 3000 }, // Level2
    { 9200, 3000, 2000, 4000, 500, 500, 9000, 9000, 9000, 30, 100, 50, 200, 0, 3000 }, // Level3（默认档）
    { 9500, 3000, 2000, 4000, 500, 500, 9500, 9500, 9500, 30, 100, 50, 200, 0, 3000 }, // Level4
    { 9800, 3000, 2000, 4000, 500, 500, 9800, 9800, 9800, 30, 100, 50, 200, 0, 3000 }, // Level5
}; // 海外档位表结束

// 获取某区域的档位表指针、档位数量和默认 RTP；成功返回 1，失败返回 0。
int32_t RtpConfig_GetProfiles(uint8_t region,const RtpProfileConfig** outProfiles,int32_t* outCount,int32_t* outDefaultRtpPermyriad); // 导出区域档位表
// 按 RTP 数值精确查找档位；找不到返回 NULL。
const RtpProfileConfig* RtpConfig_FindProfile(uint8_t region, int32_t rtpPermyriad); // 查找指定档位
// 获取某区域的默认档位配置；找不到返回 NULL。
const RtpProfileConfig* RtpConfig_GetDefaultProfile(uint8_t region); // 获取默认档

#endif // _RTP_CONFIG_H_ // 头文件结束
