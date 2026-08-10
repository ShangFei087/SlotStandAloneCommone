#ifndef _RTP_CONFIG_H_ 
#define _RTP_CONFIG_H_ 

#include <stdint.h> 

// RTP 区域定义
#define RTP_REGION_DOMESTIC_JPOnline 0      // 国内有联网彩金区域
#define RTP_REGION_DOMESTIC 1               // 国内无联网彩金区域
#define RTP_REGION_OVERSEAS 2               // 海外区域
#define RTP_PROFILE_COUNT 5                 // 每个区域固定 5 个难度档位
#define RTP_DEFAULT_DOMESTIC 9850           // 国内默认 RTP（98.5%）
#define RTP_DEFAULT_OVERSEAS 8900           // 海外默认 RTP（89.0%）

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

// 国内有联网彩金 RTP 档位：98.5 / 98.8 / 99.2 / 99.5 / 99.8（默认 99.2） 
static const RtpProfileConfig gDomesticJPOnlineProfiles[] = { // 国内档位表（从低到高）
    { 9750, 3000, 2000, 4500, 500, 0, 9750, 9750, 10000, 50, 150, 200, 600, 0, 4500 }, // Level1
    { 9800, 3000, 2000, 4500, 500, 0, 9800, 9800, 10000, 50, 150, 200, 600, 0, 4500 }, // Level2
    { 9850, 3000, 2000, 4500, 500, 0, 9850, 9850, 10000, 50, 150, 200, 600, 0, 4500 }, // Level3（默认档）
    { 9900, 3000, 2000, 4500, 500, 0, 9900, 9900, 10000, 50, 150, 200, 600, 0, 4500 }, // Level4
    { 9950, 3000, 2000, 4500, 500, 0, 9950, 9950, 10000, 50, 150, 200, 600, 0, 4500 }, // Level5
}; 

// 国内无联网彩金 RTP 档位：97.5 / 98 / 98.5 / 99 / 99.5（默认 98.5）
static const RtpProfileConfig gDomesticProfiles[] = {
    { 9750, 3000, 2000, 4500, 500, 0, 9750, 9750, 10000, 50, 150, 200, 600, 0, 4500 }, // Level1
    { 9800, 3000, 2000, 4500, 500, 0, 9800, 9800, 10000, 50, 150, 200, 600, 0, 4500 }, // Level2
    { 9850, 3000, 2000, 4500, 500, 0, 9850, 9850, 10000, 50, 150, 200, 600, 0, 4500 }, // Level3（默认档）
    { 9900, 3000, 2000, 4500, 500, 0, 9900, 9900, 10000, 50, 150, 200, 600, 0, 4500 }, // Level4
    { 9950, 3000, 2000, 4500, 500, 0, 9950, 9950, 10000, 50, 150, 200, 600, 0, 4500 }, // Level5
};

// 海外 RTP 档位：83 / 86 / 89 / 92 / 95
static const RtpProfileConfig gOverseasProfiles[] = {
    { 8300, 3500, 1500, 4500, 500, 0, 8300, 8300, 8300, 30, 100, 50, 200, 0, 3000 }, // L1
    { 8600, 3500, 1500, 4500, 500, 0, 8600, 8600, 8600, 30, 100, 50, 200, 0, 3000 }, // L2
    { 8900, 3500, 1500, 4500, 500, 0, 8900, 8900, 8900, 30, 100, 50, 200, 0, 3000 }, // L3
    { 9200, 3500, 1500, 4500, 500, 0, 9200, 9200, 9200, 30, 100, 50, 200, 0, 3000 }, // L4
    { 9500, 3500, 1500, 4500, 500, 0, 9500, 9500, 9500, 30, 100, 50, 200, 0, 3000 }, // L5
};

// 获取某区域的档位表指针、档位数量和默认 RTP；成功返回 1，失败返回 0。
int32_t RtpConfig_GetProfiles(uint8_t regio,const RtpProfileConfig** outProfiles,int32_t* outCount,int32_t* outDefaultRtpPermyriad); // 导出区域档位表
// 按 RTP 数值精确查找档位；找不到返回 NULL。
const RtpProfileConfig* RtpConfig_FindProfile(uint8_t region, int32_t rtpPermyriad); // 查找指定档位
// 获取某区域的默认档位配置；找不到返回 NULL。
const RtpProfileConfig* RtpConfig_GetDefaultProfile(uint8_t region); // 获取默认档
// 由 RTP 万分比反查难度档位（1~5）；rtpPermyriad<=0 用区域默认档；失败返回 0。
int32_t RtpConfig_GetLevelByRtp(uint8_t region, int32_t rtpPermyriad);

#endif // _RTP_CONFIG_H_ // 头文件结束
