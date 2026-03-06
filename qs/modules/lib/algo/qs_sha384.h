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
#ifndef _QS_SHA384_H_
#define _QS_SHA384_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#if (QS_SUPPORT_SHA384)
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
typedef struct qs_sha512_ctx  qs_sha384_ctx;


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

/**
 *   \brief    sha384运算
 *   \param    pHash     sha384结果缓冲区
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  sha384结果的长度
 */
size_t qs_sha384(uint8_t *pHash, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha384运算初始化
 *   \param    pCtx      sha384运算管理结构体
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_sha384_init(qs_sha384_ctx *pCtx);

/**
 *   \brief    sha384运算
 *   \param    pCtx      sha384运算管理结构体
 *   \param    pBuffer   数据输入缓冲区
 *   \param    length    数据的数量
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_sha384_core(qs_sha384_ctx *pCtx, uint8_t *pBuffer, size_t length);

/**
 *   \brief    sha384运算
 *   \param    pCtx      sha384运算管理结构体
 *   \param    pHash     sha384结果缓冲区
 *   \returns  sha384结果的长度
 */
size_t qs_sha384_finish(qs_sha384_ctx *pCtx, uint8_t *pHash);


#endif

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _QS_SHA384_H_ */
/* END OF FILE */
