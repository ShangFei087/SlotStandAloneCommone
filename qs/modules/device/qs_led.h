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
#ifndef _QS_LED_H_
#define _QS_LED_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (QS_DRIVER_SUPPORT_LED)
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
 // define the led offset code in map
#define QS_LED_MASK_RED                                  (1 << 0)
#define QS_LED_MASK_GREEN                                (1 << 1)
#define QS_LED_MASK_BLUE                                 (1 << 2)

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
#if(QS_LED_RED)
/**
 *  @brief          红灯点亮操作
 *  @param[in]      millisecond 点亮时间，参数为#QS_MAX，一直亮
 *  @return         无
 *  @details
 */
void qs_led_red_on(uint32_t millisecond);

/**
 *  @brief          红灯息灭操作
 *  @param[in]      millisecond 息灭时间，参数为#QS_MAX，一直灭
 *  @return         无
 *  @details
 */
void qs_led_red_off(uint32_t millisecond);

/**
 *  @brief          红灯闪烁操作
 *  @param[in]      OnTime 点亮时间，毫秒为单位
 *  @param[in]      OffTime 息灭时间，毫秒为单位
 *  @return         无
 *  @details
 */
void qs_led_red_blink(uint32_t OnTime, uint32_t OffTime, uint32_t count);
#endif

#if(QS_LED_GREEN)
/**
 *  @brief          绿灯点亮操作
 *  @param[in]      millisecond 点亮时间，参数为#QS_MAX，一直亮
 *  @return         无
 *  @details
 */
void qs_led_green_on(uint32_t millisecond);

/**
 *  @brief          绿灯息灭操作
 *  @param[in]      millisecond 息灭时间，参数为#QS_MAX，一直灭
 *  @return         无
 *  @details
 */
void qs_led_green_off(uint32_t millisecond);

/**
 *  @brief          绿灯闪烁操作
 *  @param[in]      OnTime 点亮时间，毫秒为单位
 *  @param[in]      OffTime 息灭时间，毫秒为单位
 *  @return         无
 *  @details
 */
void qs_led_green_blink(uint32_t OnTime, uint32_t OffTime, uint32_t count);
#endif

#if(QS_LED_BLUE)
/**
 *  @brief          蓝灯点亮操作
 *  @param[in]      millisecond 点亮时间，参数为#QS_MAX，一直亮
 *  @return         无
 *  @details
 */
void qs_led_blue_on(uint32_t millisecond);

/**
 *  @brief          蓝灯息灭操作
 *  @param[in]      millisecond 息灭时间，参数为#QS_MAX，一直灭
 *  @return         无
 *  @details
 */
void qs_led_blue_off(uint32_t millisecond);

/**
 *  @brief          蓝灯闪烁操作
 *  @param[in]      OnTime 点亮时间，毫秒为单位
 *  @param[in]      OffTime 息灭时间，毫秒为单位
 *  @return         无
 *  @details
 */
void qs_led_blue_blink(uint32_t OnTime, uint32_t OffTime, uint32_t count);
#endif

/**
 *  @brief          
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_led_exec(uint32_t millisecond);

/**
 *  @brief          LED设备管理初始化操作
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_led_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_LED_H_
/* END OF FILE */
