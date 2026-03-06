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
#ifndef _QS_DES_H_
#define _QS_DES_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#if (QS_SUPPORT_DES)
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
/// des 管理结构体
typedef struct _qs_des_ctx
{
    int mode;                      ///< Ciphering Mode.
    uint8_t memory[8];             ///< Intermediate state.
    uint32_t index;                ///< index.
    uint32_t subKeys[3 * 32 * 2];  ///< DES Sub-keys.
	size_t   KeySize;              ///< DES key size.
} qs_des_ctx;

typedef struct {
	uint32_t which;
	uint32_t esk[96];     /* DES encryption subkeys */
	uint32_t dsk[96];     /* DES decryption subkeys */
} des_context;

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

size_t
des_key_set(
	des_context *ctx,
	uint8_t *pKey,
	size_t   KeySize
);

size_t
des_encrypt(
	des_context *ctx,
	uint8_t *pPlaintext,
	size_t   PlaintextSize,
	uint8_t *pCiphertext
);

size_t
des_decrypt(
	des_context *ctx,
	uint8_t *pCiphertext,
	size_t   CiphertextSize,
	uint8_t *pPlaintext
);

/**
 *   \brief    DES加密一个数据块，数据块的长度为8字节
 *   \param    pKey         密钥
 *   \param    KeySize      密钥长度(DES64: 8, DES128: 16, DES192: 24)
 *   \param    pPlaintext   明文输入
 *   \param    pCiphertext  密文输出
 *   \returns  成功时返回8，否则返回0
 */
size_t qs_des_block_encrypt(uint8_t *pKey, size_t KeySize, uint8_t *pPlaintext, uint8_t *pCiphertext);

/**
 *   \brief    DES解密一个数据块，数据块的长度为8字节
 *   \param    pKey         密钥
 *   \param    KeySize      密钥长度(DES64: 8, DES128: 16, DES192: 24)
 *   \param    pCiphertext  密文输入
 *   \param    pPlaintext   明文输出
 *   \returns  成功时返回8，否则返回0
 */
size_t qs_des_block_decrypt(uint8_t *pKey, size_t KeySize, uint8_t *pCiphertext, uint8_t *pPlaintext);


#endif

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _QS_DES_H_ */
/* END OF FILE */
