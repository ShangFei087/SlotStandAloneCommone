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
#ifndef _QS_MODULES_H_
#define _QS_MODULES_H_

#ifdef __cplusplus
extern "C" {
#endif


#include "modules/lib/base64/qs_base64.h"
#include "modules/lib/algo/qs_algo.h"
#include "modules/lib/json/qs_json.h"
#include "modules/lib/utility/qs_utility.h"

#if (QS_SUPPORT_DATABASE)
#include "device/qs_database.h"
#endif

#if(QS_DRIVER_SUPPORT_KEYBOARD)
#include "device/qs_keyboard.h"
#endif
#if(QS_DRIVER_SUPPORT_LED)
#include "device/qs_led.h"
#endif
#if(QS_DRIVER_SUPPORT_BUZZER)
#include "device/qs_buzzer.h"
#endif
#if(QS_SUPPORT_MM)
#include "device/qs_mm.h"
#endif
#if(QS_DRIVER_SUPPORT_TIME)
#include "device/qs_time.h"
#endif
#if(QS_DRIVER_SUPPORT_BILL)
#include "device/bills/qs_bill.h"
#endif
#if(QS_DRIVER_SUPPORT_PRINTER)
#include "device/printers/qs_printer.h"
#endif

#if (QS_SUPPORT_MODBUS)
#include "device/modbus/include/mb.h"
#endif

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
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_modules_exec(uint32_t millisecond);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_modules_init(void);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_MODULES_H_
/* END OF FILE */
