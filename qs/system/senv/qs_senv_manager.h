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
#ifndef _QS_SENV_MANAGER_H_
#define _QS_SENV_MANAGER_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (QS_SUPPORT_SENV)
#include "qs_senv.h"

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
qs_json *qs_senv_manager_read(void);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_senv_manager_write(qs_senv *pSenv, qs_json *json);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_senv_manager_write_message(qs_senv *pSenv, uint8_t *pData, size_t DataSize);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_senv_manager_clear(void);

/**
*  @brief
*  @param
*  @return
*  @details
*/
qs_senv *qs_senv_manager_get(uint32_t address);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
qs_senv *qs_senv_manager_register(uint32_t address);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_senv_manager_unregister(qs_senv *pSenv);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_senv_manager_exec(uint32_t millisecond);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_senv_manager_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_SENV_MANAGER_H_
/* END OF FILE */
