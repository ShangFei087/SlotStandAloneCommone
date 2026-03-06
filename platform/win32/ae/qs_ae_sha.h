/**
 * @file    qs_ae_sha.h
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
#ifndef _QS_AE_SHA_H_
#define _QS_AE_SHA_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#if (QS_SUPPORT_SHA > 1)
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/


/**************************************************************************
 *                         D A T A   T Y P E S                            *
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

#if (QS_SUPPORT_SHA1 > 1)
/**
 *   \brief    sha1运算
 *   \param    pHash     sha1结果缓冲区
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  sha1结果的长度
 */
size_t qs_ae_sha1(uint8_t *pHash, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha1运算初始化
 *   \param    pCtx      sha1运算管理结构体
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha1_init(qs_sha1_ctx *pCtx);

/**
 *   \brief    sha1运算
 *   \param    pCtx      sha1运算管理结构体
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha1_core(qs_sha1_ctx *pCtx, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha1运算
 *   \param    pCtx      sha1运算管理结构体
 *   \param    pHash     sha1结果缓冲区
 *   \returns  sha1结果的长度
 */
size_t qs_ae_sha1_finish(qs_sha1_ctx *pCtx, uint8_t *pHash);
#endif

#if (QS_SUPPORT_SHA224 > 1)
/**
 *   \brief    sha224运算
 *   \param    pHash     sha224结果缓冲区
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  sha224结果的长度
 */
size_t qs_ae_sha224(uint8_t *pHash, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha224运算初始化
 *   \param    pCtx      sha224运算管理结构体
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha224_init(qs_sha224_ctx *pCtx);

/**
 *   \brief    sha224运算
 *   \param    pCtx      sha224运算管理结构体
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha224_core(qs_sha224_ctx *pCtx, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha224运算
 *   \param    pCtx      sha224运算管理结构体
 *   \param    pHash     sha224结果缓冲区
 *   \returns  sha224结果的长度
 */
size_t qs_ae_sha224_finish(qs_sha224_ctx *pCtx, uint8_t *pHash);
#endif

#if (QS_SUPPORT_SHA256 > 1)
/**
 *   \brief    sha256运算
 *   \param    pHash     sha256结果缓冲区
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  sha256结果的长度
 */
size_t qs_ae_sha256(uint8_t *pHash, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha256运算初始化
 *   \param    pCtx      sha256运算管理结构体
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha256_init(qs_sha256_ctx *pCtx);

/**
 *   \brief    sha256运算
 *   \param    pCtx      sha256运算管理结构体
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha256_core(qs_sha256_ctx *pCtx, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha256运算
 *   \param    pCtx      sha256运算管理结构体
 *   \param    pHash     sha256结果缓冲区
 *   \returns  sha256结果的长度
 */
size_t qs_ae_sha256_finish(qs_sha256_ctx *pCtx, uint8_t *pHash);
#endif

#if (QS_SUPPORT_SHA384 > 1)
/**
 *   \brief    sha384运算
 *   \param    pHash     sha384结果缓冲区
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  sha384结果的长度
 */
size_t qs_ae_sha384(uint8_t *pHash, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha384运算初始化
 *   \param    pCtx      sha384运算管理结构体
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha384_init(qs_sha384_ctx *pCtx);

/**
 *   \brief    sha384运算
 *   \param    pCtx      sha384运算管理结构体
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha384_core(qs_sha384_ctx *pCtx, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha384运算
 *   \param    pCtx      sha384运算管理结构体
 *   \param    pHash     sha384结果缓冲区
 *   \returns  sha384结果的长度
 */
size_t qs_ae_sha384_finish(qs_sha384_ctx *pCtx, uint8_t *pHash);
#endif

#if (QS_SUPPORT_SHA512 > 1)
/**
 *   \brief    sha512运算
 *   \param    pHash     sha512结果缓冲区
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  sha512结果的长度
 */
size_t qs_ae_sha512(uint8_t *pHash, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha512运算初始化
 *   \param    pCtx      sha512运算管理结构体
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha512_init(qs_sha512_ctx *pCtx);

/**
 *   \brief    sha512运算
 *   \param    pCtx      sha512运算管理结构体
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_ae_sha512_core(qs_sha512_ctx *pCtx, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha512运算
 *   \param    pCtx      sha512运算管理结构体
 *   \param    pHash     sha512结果缓冲区
 *   \returns  sha512结果的长度
 */
size_t qs_ae_sha512_finish(qs_sha512_ctx *pCtx, uint8_t *pHash);
#endif

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _QS_AE_SHA_H_ */
/* END OF FILE */
