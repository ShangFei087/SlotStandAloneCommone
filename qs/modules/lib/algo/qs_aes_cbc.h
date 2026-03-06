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
#ifndef _QS_AES_CBC_H_
#define _QS_AES_CBC_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#if (QS_SUPPORT_AES_CBC)
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

/**
 *   \brief    AES CBC加密数据
 *   \param    pKey           密钥
 *   \param    KeySize        密钥长度(AES128: 16, AES192: 24, AES256: 32)
 *   \param    pPlaintext     明文输入
 *   \param    PlaintextSize  明文字节长度
 *   \param    pCiphertext    密文输出
 *   \param    pStartValue    初始向量值，16字节长度
 *   \returns  成功时返回密文字节长度，否则返回0
 */
size_t qs_aes_cbc_encrypt(uint8_t *pKey, size_t KeySize, uint8_t *pPlaintext, size_t PlaintextSize, uint8_t *pCiphertext, uint8_t *pStartValue);

/**
 *   \brief    AES CBC解密数据
 *   \param    pKey           密钥
 *   \param    KeySize        密钥长度(AES128: 16, AES192: 24, AES256: 32)
 *   \param    pCiphertext    密文输入
 *   \param    CiphertextSize 密文字节长度
 *   \param    pPlaintext     明文输出
 *   \param    pStartValue    初始向量值，16字节长度
 *   \returns  成功时返回明文字节长度，否则返回0
 */
size_t qs_aes_cbc_decrypt(uint8_t *pKey, size_t KeySize, uint8_t *pCiphertext, size_t CiphertextSize, uint8_t *pPlaintext, uint8_t *pStartValue);

/**
 *   \brief    AES CBC运算初始化
 *   \param    pCtx           运算管理结构体
 *   \param    pKey           密钥
 *   \param    KeySize        密钥长度(AES128: 16, AES192: 24, AES256: 32)
 *   \param    mode           #QS_ENCRYPT 加密, #QS_DECRYPT 解密
 *   \param    pStartValue    初始向量值，16字节长度
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_aes_cbc_init(qs_aes_ctx *pCtx, uint8_t *pKey, size_t KeySize, int mode, uint8_t *pStartValue);

/**
 *   \brief    AES CBC运算
 *   \param    pCtx           运算管理结构体
 *   \param    pDst           运算结果输出缓冲区
 *   \param    pSrc           运算输入缓冲区
 *   \param    SrcSize        输入字节长度, 必须为16的倍数
 *   \returns  成功时返回输出字节长度，否则返回0
 */
size_t qs_aes_cbc_core(qs_aes_ctx *pCtx, uint8_t *pDst, uint8_t *pSrc, size_t SrcSize);

/**
 *   \brief    AES CBC运算完成
 *   \param    pCtx           运算管理结构体
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_aes_cbc_finish(qs_aes_ctx *pCtx);


#endif

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _QS_AES_CBC_H_ */
/* END OF FILE */
