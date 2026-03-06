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
#ifndef _QS_ALGO_H_
#define _QS_ALGO_H_

#ifdef __cplusplus
extern "C" {
#endif
#if (QS_SUPPORT_ALG)

#if (QS_SUPPORT_AES)
#include "qs_aes.h"
#endif
#if (QS_SUPPORT_AES_CBC)
#include "qs_aes_cbc.h"
#endif
#if (QS_SUPPORT_AES_ECB)
#include "qs_aes_ecb.h"
#endif
#if (QS_SUPPORT_CRC16)
#include "qs_crc16.h"
#endif
#if (QS_SUPPORT_DES)
#include "qs_des.h"
#endif
#if (QS_SUPPORT_DES_CBC)
#include "qs_des_cbc.h"
#endif
#if (QS_SUPPORT_DES_ECB)
#include "qs_des_ecb.h"
#endif
#if(QS_SUPPORT_RAND)
#include "qs_rand.h"
#endif
#if (QS_SUPPORT_SHA1)
#include "qs_sha1.h"
#endif
#if (QS_SUPPORT_SHA224)
#include "qs_sha224.h"
#endif
#if (QS_SUPPORT_SHA256)
#include "qs_sha256.h"
#endif
#if (QS_SUPPORT_SHA384)
#include "qs_sha384.h"
#endif
#if (QS_SUPPORT_SHA512)
#include "qs_sha512.h"
#endif
#if (QS_SUPPORT_RSA)
#include "qs_nn.h"
#include "qs_prime.h"
#include "qs_rsa.h"
#endif

#if (QS_SUPPORT_ALG > 1)
#include "qs_ae_algo.h"
#endif
#endif
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#define QS_ENCRYPT                    0
#define QS_DECRYPT                    1

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
 *  @param[in]      millisecond 每次调用的时间差，毫秒为单位
 *  @return         无
 *  @details
 */
void qs_algo_exec(uint32_t millisecond);

/**
 *  @brief          
 *  @param[in]      无
 *  @return         无
 *  @details
 */
void qs_algo_init(void);


#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_ALGO_H_
/* END OF FILE */
