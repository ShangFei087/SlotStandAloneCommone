#ifndef _RTP_BUCKET_REPORT_H_
#define _RTP_BUCKET_REPORT_H_

#include <stdint.h>
#include "CommonStruct.h"

/** 重置分桶统计（压测开始前可调用）。 */
void RtpBucketReport_Reset(void);

/**
 * @brief 累计一局付费结果到分桶表（仅 OT_Normal 应调用）。
 * @param betValue 单线下注（与 DLL_GetGameResultById 一致）
 * @param lineCount 线数
 * @param outRes 本局输出结果
 */
void RtpBucketReport_OnRound(int32_t betValue, int32_t lineCount, const OutResult_t* outRes);

/** 打印与 Excel 同结构的分桶 RTP 报表。 */
void RtpBucketReport_Print(void);

#endif /* _RTP_BUCKET_REPORT_H_ */
