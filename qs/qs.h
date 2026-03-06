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
#ifndef _QS_H_
#define _QS_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "qs_cfg.h"

#if (QS_PLATFORM == QS_PLATFORM_WIN32)
#include <windows.h>
#endif

#ifdef __CROSSSTUDIO
#define SBO_CONST                       __code const
#else
#define SBO_CONST                       const
#endif

#ifndef CONST
#define CONST                           const
#endif

#ifndef BOOL
#define BOOL                            int
#endif

#if 0
#ifndef int8_t
#define int8_t                          signed char
#endif
#ifndef int16_t
#define int16_t                         signed short
#endif
#ifndef int32_t
#define int32_t                         signed int
#endif
#ifndef int64_t
#define int64_t                         signed long long
#endif
#ifndef uint8_t
#define uint8_t                         unsigned char
#endif
#ifndef uint16_t
#define uint16_t                        unsigned short
#endif
#ifndef uint32_t
#define uint32_t                        unsigned int
#endif
#ifndef uint64_t
#define uint64_t                        unsigned long long
#endif
#else
typedef signed char                     int8_t; 
typedef signed short                    int16_t;
typedef signed int                      int32_t;
typedef signed long long                int64_t;
typedef unsigned char                   uint8_t;
typedef unsigned short                  uint16_t;
typedef unsigned int                    uint32_t;
typedef unsigned long long              uint64_t;
#endif


#if (QS_PLATFORM != QS_PLATFORM_WIN32)
	#ifndef size_t
		#define size_t                          unsigned int
	#endif
#else
	#define size_t                          unsigned int
#endif


#ifndef TRUE
#define TRUE                            1
#endif

#ifndef FALSE
#define FALSE                           0
#endif

#ifndef NULL
#define NULL                            0
#endif


/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
typedef enum _qs_id {
	QS_ID_0 = 0,
	QS_ID_1,
	QS_ID_2,
	QS_ID_3,
	QS_ID_4,
	QS_ID_5,
	QS_ID_6,
	QS_ID_7,
	QS_ID_8,
	QS_ID_9,
	QS_ID_10,
	QS_ID_11,
	QS_ID_12,
	QS_ID_13,
	QS_ID_14,
	QS_ID_15,
	QS_ID_16,
	QS_ID_17,
	QS_ID_18,
	QS_ID_19,
	QS_ID_20,
	QS_ID_21,
	QS_ID_22,
	QS_ID_23,
	QS_ID_24,
	QS_ID_25,
	QS_ID_26,
	QS_ID_27,
	QS_ID_28,
	QS_ID_29,
	QS_ID_30,
	QS_ID_31,
	QS_ID_MAX
} qs_id;

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
#include "../application/IDEA/app/source/qs_cfg.h"
#include "qs_driver.h"
#include "qs_task.h"

#include "modules/qs_modules.h"
#include "system/qs_system.h"

//#if ((QS_PLATFORM == QS_PLATFORM_WIN32) || (QS_PLATFORM == QS_PLATFORM_LINUX))
#define QS_CRITICAL_ENTER()          qs_platform_interrupt_disable()
#define QS_CRITICAL_EXIT()           qs_platform_interrupt_enable()
//#else
//#define QS_CRITICAL_ENTER()
//#define QS_CRITICAL_EXIT()
//#endif
#define QS_SYSTEM_REBOOT()           qs_platform_system_reboot()
#define QS_SYSTEM_RUN_CODE(p)        qs_platform_system_run_code(p)
#define QS_MAX                       ((uint32_t)-1)

/**
*  @brief          系统固化数据
*  @param[in]      无
*  @return         #qs_json
*  @details
*/
const char *qs_const_data(void);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_reboot(void);

 /**
 *  @brief          读取唯一序列号
 *  @param[in]      pData 存在序列号的缓冲区
 *  @param[in]      DataSizeMax 缓冲区的大小
 *  @return         实际的序列号字节数
 *  @details
 */
size_t qs_sn(uint8_t *pData, size_t DataSizeMax);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_exec(void);

/**
 *  @brief          
 *  @param           
 *  @return         
 *  @details        
 */
void qs_delay(uint32_t millisecond);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_start(void);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_init(void);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_H_
/* END OF FILE */
