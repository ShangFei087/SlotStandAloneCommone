/**
 * @file    
 * @author  saimas@163.com
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
#ifndef _QS_TIMER_H_
#define _QS_TIMER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#if (QS_SUPPORT_TIMER)
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
 *  @brief    100微秒为单位的定时，由硬件定时器中断处理，非必要时，不用使用，而且不应处理复杂耗时的工作
 *  @param
 *  @return
 *  @details
 */
BOOL qs_timer_set_by_microsecond(void (*pfnCallback)(uint32_t count, void *parg), uint32_t microsecond, uint32_t count, void *parg);

/**
 *  @brief    毫秒为单位的定时，用于实时要求不高，本定时是在程序主循环内进行
 *  @param
 *  @return
 *  @details
 */
BOOL qs_timer_set_by_millisecond(void (*pfnCallback)(uint32_t count, void *parg), uint32_t millisecond, uint32_t count, void *parg);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void qs_timer_exec(uint32_t millisecond);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_timer_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_TIMER_H_
/* END OF FILE */
