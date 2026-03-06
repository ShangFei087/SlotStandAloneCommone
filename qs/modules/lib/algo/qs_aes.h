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
#ifndef _QS_AES_H_
#define _QS_AES_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#if (QS_SUPPORT_AES)
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
/// aes key 管理结构体
typedef struct _qs_aes_key
{
    uint32_t rd_key[4 *(14 + 1)];  ///< Round key.
    int rounds;                    ///< Number of rounds.
} qs_aes_key;

/// aes 管理结构体
typedef struct _qs_aes_ctx
{
    int mode;                      ///< Ciphering Mode.
    uint8_t memory[16];            ///< Intermediate state.
    uint32_t index;                ///< index.
    qs_aes_key key;               ///< AES Sub-keys.
} qs_aes_ctx;

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
 *   \brief    AES加密一个数据块，数据块的长度为16字节
 *   \param    pKey         密钥
 *   \param    KeySize      密钥长度(AES128: 16, AES192: 24, AES256: 32)
 *   \param    pPlaintext   明文输入
 *   \param    pCiphertext  密文输出
 *   \returns  成功时返回16，否则返回0
 */
size_t qs_aes_block_encrypt(uint8_t *pKey, size_t KeySize, uint8_t *pPlaintext, uint8_t *pCiphertext);

/**
 *   \brief    AES解密一个数据块，数据块的长度为16字节
 *   \param    pKey         密钥
 *   \param    KeySize      密钥长度(AES128: 16, AES192: 24, AES256: 32)
 *   \param    pCiphertext  密文输入
 *   \param    pPlaintext   明文输出
 *   \returns  成功时返回16，否则返回0
 */
size_t qs_aes_block_decrypt(uint8_t *pKey, size_t KeySize, uint8_t *pCiphertext, uint8_t *pPlaintext);


#endif

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _QS_AES_H_ */
/* END OF FILE */
