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
#ifndef _QS_TIME_H_
#define _QS_TIME_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (QS_DRIVER_SUPPORT_TIME)

/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
typedef struct _qs_time {
	uint16_t seconds;
	uint16_t minutes;
	uint16_t hours;
	uint16_t week;
	uint16_t day;
	uint16_t month;
	uint16_t year;
	uint16_t millisecond;
} qs_time;

/**************************************************************************
 *                 E X T E R N A L   R E F E R E N C E S                  *
 **************************************************************************/


/**************************************************************************
 *                        G L O B A L   D A T A                           *
 **************************************************************************/


/**************************************************************************
 *               F U N C T I O N   D E C L A R A T I O N S                *
 **************************************************************************/

int qs_time_cmp(qs_time *pTimeOld, qs_time *pTimeNew);

/**
 *  @brief          
 *  @param[in]      
 *  @return        
 *  @details
 */
BOOL qs_time_set(qs_time *pTime);

/**
 *  @brief
 *  @param[in]
 *  @return
 *  @details
 */
BOOL qs_time_get(qs_time *pTime);

/**
 *  @brief          
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_time_exec(uint32_t millisecond);

/**
 *  @brief          蜂鸣器设备管理初始化操作
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_time_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_TIME_H_
/* END OF FILE */
