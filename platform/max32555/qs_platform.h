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
#ifndef _QS_PLATFORM_H_
#define _QS_PLATFORM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "qs_ae_algo.h"
#include "qs_ae_aes.h"
#include "qs_ae_des.h"
#include "qs_ae_rand.h"
#include "qs_ae_sha.h"
	

#if(QS_DRIVER_SUPPORT_NVRAM)
#include "driver_nvram.h"
#endif
#if(QS_DRIVER_SUPPORT_IFLASH)
#include "driver_iflash.h"
#endif
#if(QS_DRIVER_SUPPORT_TIMER)
#include "driver_timer.h"
#endif
#if(QS_DRIVER_SUPPORT_SERIAL)
#include "driver_serial.h"
#endif
#if(QS_DRIVER_SUPPORT_USB)
#include "driver_usb.h"
#endif
#if(QS_DRIVER_SUPPORT_TIME)
#include "driver_rtc.h"
#endif
#if(QS_DRIVER_SUPPORT_USB_HID)
#include "driver_usb_hid.h"
#endif
#if(QS_DRIVER_SUPPORT_I2C)
#include "driver_i2c.h"
#endif
#if(QS_DRIVER_SUPPORT_SPI)
#include "driver_spi.h"
#endif

/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/     
#define QS_PLATFORM_MILLISECOND_MAX   0xFFFFFFFF

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
*  @brief          
*  @param          
*  @return         
*  @details
*/
void qs_platform_interrupt_enable(void);

/**
*  @brief          
*  @param          
*  @return         
*  @details
*/
void qs_platform_interrupt_disable(void);

/**
*  @brief          
*  @param          
*  @return         
*  @details
*/
void qs_platform_system_reboot(void);

/**
*  @brief          
*  @param          
*  @return         
*  @details
*/
void qs_platform_run_code(uint32_t address);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
uint32_t qs_platform_millisecond(void);
/**
*  @brief        
*  @param[in]    
*  @param[in]     
*  @return         
*  @details
*/
size_t qs_platform_sn(uint8_t *pData, size_t DataSizeMax);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void qs_platform_exec(uint32_t millisecond);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_platform_uninit(void);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_platform_init(void);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_PLATFORM_H_
/* END OF FILE */
