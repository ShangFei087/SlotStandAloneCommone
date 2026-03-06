/**
 * @file    
 * @author  Huang Wen <Email:ww1383@163.com, QQ:214890094, WeChat:w18926268887>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * @section DESCRIPTION
 *
 * This file is ...
 */
#ifndef _QS_BUZZER_H_
#define _QS_BUZZER_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (QS_DRIVER_SUPPORT_BUZZER)
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/


/**************************************************************************
 *                 E X T E R N A L   R E F E R E N C E S                  *
 **************************************************************************/


/**************************************************************************
 *                        G L O B A L   D A T A                           *
 **************************************************************************/


/**************************************************************************
 *               F U N C T I O N   D E C L A R A T I O N S                *
 **************************************************************************/

/**
 *  @brief          蜂鸣器开操作
 *  @param[in]      millisecond 蜂鸣器开时间，参数为#QS_MAX，一直蜂鸣器开
 *  @return         无
 *  @details
 */
void qs_buzzer_on(uint32_t millisecond);

/**
 *  @brief          蜂鸣器关操作
 *  @param[in]      millisecond 蜂鸣器关时间，参数为#QS_MAX，一直蜂鸣器关
 *  @return         无
 *  @details
 */
void qs_buzzer_off(uint32_t millisecond);

/**
 *  @brief          蜂鸣器开关操作
 *  @param[in]      OnTime 蜂鸣器开时间，毫秒为单位
 *  @param[in]      OffTime 蜂鸣器关时间，毫秒为单位
 *  @return         无
 *  @details
 */
void qs_buzzer_blink(uint32_t OnTime, uint32_t OffTime, uint32_t count);

/**
 *  @brief          
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_buzzer_exec(uint32_t millisecond);

/**
 *  @brief          蜂鸣器设备管理初始化操作
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_buzzer_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_BUZZER_H_
/* END OF FILE */
