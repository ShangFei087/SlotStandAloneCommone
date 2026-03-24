#include <stddef.h>  
#include "RtpConfig.h" // 引入 RTP 配置头文件

// 获取区域配置数组与默认 RTP 档位；返回 1 表示成功，0 表示失败。
int32_t RtpConfig_GetProfiles(uint8_t region,const RtpProfileConfig** outProfiles,int32_t* outCount,int32_t* outDefaultRtpPermyriad) // 导出区域档位表
{
    if (outProfiles == NULL || outCount == NULL || outDefaultRtpPermyriad == NULL) // 校验输出参数
    {
        return 0; // 参数不合法直接失败
    }

    if (region == RTP_REGION_DOMESTIC) // 国内区域分支
    {
        *outProfiles = gDomesticProfiles; // 返回国内档位数组
        *outCount = RTP_PROFILE_COUNT; // 返回国内档位数量
        *outDefaultRtpPermyriad = RTP_DEFAULT_DOMESTIC; // 返回国内默认 RTP
        return 1; // 成功
    }

    if (region == RTP_REGION_OVERSEAS) // 海外区域分支
    {
        *outProfiles = gOverseasProfiles; // 返回海外档位数组
        *outCount = RTP_PROFILE_COUNT; // 返回海外档位数量
        *outDefaultRtpPermyriad = RTP_DEFAULT_OVERSEAS; // 返回海外默认 RTP
        return 1; // 成功
    }

    return 0; // 未知区域返回失败
} // RtpConfig_GetProfiles 结束
// 按 RTP 档位精确查找配置，找不到返回 NULL。
const RtpProfileConfig* RtpConfig_FindProfile(uint8_t region, int32_t rtpPermyriad) // 查找指定 RTP 对应的档位
{
    const RtpProfileConfig* profiles = NULL; // 区域档位数组指针
    int32_t count = 0; // 档位数量
    int32_t defaultRtpPermyriad = 0; // 默认 RTP（仅用于接口出参占位）

    if (!RtpConfig_GetProfiles(region, &profiles, &count, &defaultRtpPermyriad)) // 先获取区域档位表
    {
        return NULL; // 获取失败则返回空
    }

    for (int32_t i = 0; i < count; ++i) // 遍历区域全部档位
    {
        if (profiles[i].rtpPermyriad == rtpPermyriad) // 命中目标 RTP 值
        {
            return &profiles[i]; // 返回该档位配置指针
        }
    }

    return NULL; // 遍历结束仍未命中则返回空
} // RtpConfig_FindProfile 结束
// 获取区域默认档位配置，找不到返回 NULL。
const RtpProfileConfig* RtpConfig_GetDefaultProfile(uint8_t region) // 获取指定区域的默认 RTP 档位
{
    int32_t defaultRtpPermyriad = 0; // 用于接收默认 RTP 值
    const RtpProfileConfig* profiles = NULL; // 用于接收区域档位数组指针
    int32_t count = 0; // 用于接收区域档位数量

    if (!RtpConfig_GetProfiles(region, &profiles, &count, &defaultRtpPermyriad)) // 获取区域基础信息
    {
        return NULL; // 区域非法或参数异常时返回空
    }

    (void)count; // 当前函数不直接使用数量，避免编译器告警
    return RtpConfig_FindProfile(region, defaultRtpPermyriad); // 按默认 RTP 值返回默认档位
} // RtpConfig_GetDefaultProfile 结束
