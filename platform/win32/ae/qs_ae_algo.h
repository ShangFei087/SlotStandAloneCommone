/**
 * @file    qs_ae_algo.h
 * @author  Huang Wen <huangw@likingopto.com>
 * @version 1.0
 *
 * @section LICENSE
 *
 * Copyright (C) 2014 Guangzhou Liking Optoelectronic Technology Co.,Ltd, All 
 * Rights Reserved.
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
 * http://www.likingopto.com
 *
 * @section DESCRIPTION
 *
 * This file is ...
 */
#ifndef _QS_AE_ALGO_H_
#define _QS_AE_ALGO_H_

#ifdef __cplusplus
extern "C" {
#endif


#if (QS_SUPPORT_ALG > 1)

#if (QS_SUPPORT_RAND > 1)
#include "qs_ae_rand.h"
#endif

#if (QS_SUPPORT_SHA > 1)
#include "qs_ae_sha.h"
#endif

#if (QS_SUPPORT_AES > 1)
#include "qs_ae_aes.h"
#endif

#if (QS_SUPPORT_DES > 1)
#include "qs_ae_des.h"
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

#ifdef _WIN32
/**
 *   \brief    获取函数指针
 *   \param    lpProcName 函数名称
 *   \returns  成功时返回函数指针, #NULL 失败
 */
FARPROC qs_ae_get_proc_address(LPCSTR lpProcName);
#endif

/**
 *   \brief    初始化硬件算法引擎
 */
void qs_ae_algo_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_AE_ALGO_H_
/* END OF FILE */
