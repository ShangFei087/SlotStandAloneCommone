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
#ifndef _QS_UTILITY_H_
#define _QS_UTILITY_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "qs_fifo.h"
#include "qs_list.h"
#include "qs_log.h"
#include "qs_mem.h"
#include "qs_noise.h"
#include "qs_io.h"

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

size_t qs_hex_string_to_hex(char *pHexString, uint8_t *pHexData, size_t MaxLen);

size_t qs_get_value_by_tag(char *tag, uint8_t *pHexData, size_t MaxLen);
/**
 *  @brief          对于7位数据位的数据，检查并去掉奇偶校验位
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_add_remove_even_to_7bits_data(uint8_t* pData, size_t DataSize, serial_parity parity);

/**
 *  @brief          对于7位数据位的数据，增加奇偶校验位
 *  @param          无
 *  @return         无
 *  @details
 */
void qs_add_odd_even_to_7bits_data(uint8_t* pData, size_t DataSize, serial_parity parity);

/**
 *   \brief    初始化通用独立函数模块
 */
void qs_utility_init(void);


#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_UTILITY_H_
/* END OF FILE */
