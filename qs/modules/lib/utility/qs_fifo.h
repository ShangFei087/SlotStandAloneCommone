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
#ifndef _QS_FIFO_H_
#define _QS_FIFO_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */


/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
/// FIFO 管理结构体
typedef struct _qs_fifo {
	size_t   length;      ///< FIFO 缓冲区的大小，即可容纳的对象数量
	void    *pData;       ///< 对象缓冲区指针
	uint32_t rindex;      ///< 当前 FIFO 读出位置
	uint32_t windex;      ///< 当前 FIFO 写入位置
	uint32_t pos;         ///< 当前 FIFO look的位置
} qs_fifo;


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
 *   \brief    申请FIFO管理对象句柄，对像以8位的数据宽度模式
 *   \param    length    对象缓冲区可容纳的对象数量
 *   \returns  #qs_fifo 管理对象的指针, #NULL 申请失败
 */
qs_fifo *qs_fifo_alloc8(size_t length);

/**
 *   \brief    申请FIFO管理对象句柄，对像以16位的数据宽度模式
 *   \param    length    对象缓冲区可容纳的对象数量
 *   \returns  #qs_fifo 管理对象的指针, #NULL 申请失败
 */
qs_fifo *qs_fifo_alloc16(size_t length);

/**
 *   \brief    申请FIFO管理对象句柄，对像以32位的数据宽度模式
 *   \param    length    对象缓冲区可容纳的对象数量
 *   \returns  #qs_fifo 管理对象的指针, #NULL 申请失败
 */
qs_fifo *qs_fifo_alloc32(size_t length);

/**
 *   \brief    申请FIFO管理对象句柄，对像以62位的数据宽度模式
 *   \param    length    对象缓冲区可容纳的对象数量
 *   \returns  #qs_fifo 管理对象的指针, #NULL 申请失败
 */
qs_fifo* qs_fifo_alloc64(size_t length);

/**
 *   \brief    释放FIFO管理对象句柄
 *   \param    pFifo     FIFO管理对象句柄
 *   \returns  #NULL
 */
qs_fifo *qs_fifo_free(qs_fifo *pFifo);

/**
 *   \brief    向FIFO对象缓冲区中写入一个8位数据宽度的数据
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    element   8位数据宽度的数据
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_put8(qs_fifo *pFifo, uint8_t element);

/**
 *   \brief    从FIFO对象缓冲区中读出一个8位数据宽度的数据
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  8位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_get8(qs_fifo *pFifo, uint8_t *pElement);

/**
 *   \brief    从FIFO对象缓冲区查看第一个可以读出的数据，但这个数据不会出列
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  8位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_look_first8(qs_fifo *pFifo, uint8_t *pElement);

/**
 *   \brief    从FIFO对象缓冲区查看下一个可以读出的数据，但这个数据不会出列
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  8位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_look_next8(qs_fifo *pFifo, uint8_t *pElement);

/**
 *   \brief    向FIFO对象缓冲区中写入一个16位数据宽度的数据
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    element   16位数据宽度的数据
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_put16(qs_fifo *pFifo, uint16_t element);

/**
 *   \brief    从FIFO对象缓冲区中读出一个16位数据宽度的数据
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  16位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_get16(qs_fifo *pFifo, uint16_t *pElement);

/**
 *   \brief    从FIFO对象缓冲区查看第一个可以读出的数据，但这个数据不会出列
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  16位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_look_first16(qs_fifo *pFifo, uint16_t *pElement);

/**
 *   \brief    从FIFO对象缓冲区查看下一个可以读出的数据，但这个数据不会出列
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  16位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_look_next16(qs_fifo *pFifo, uint16_t *pElement);

/**
 *   \brief    向FIFO对象缓冲区中写入一个32位数据宽度的数据
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    element   32位数据宽度的数据
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_put32(qs_fifo *pFifo, uint32_t element);

/**
 *   \brief    从FIFO对象缓冲区中读出一个32位数据宽度的数据
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  32位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_get32(qs_fifo *pFifo, uint32_t *pElement);

/**
 *   \brief    从FIFO对象缓冲区查看第一个可以读出的数据，但这个数据不会出列
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  32位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_look_first32(qs_fifo *pFifo, uint32_t *pElement);

/**
 *   \brief    从FIFO对象缓冲区查看下一个可以读出的数据，但这个数据不会出列
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  32位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_look_next32(qs_fifo *pFifo, uint32_t *pElement);

/**
 *   \brief    向FIFO对象缓冲区中写入一个64位数据宽度的数据
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    element   64位数据宽度的数据
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_put64(qs_fifo* pFifo, uint64_t element);

/**
 *   \brief    从FIFO对象缓冲区中读出一个64位数据宽度的数据
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  64位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_get64(qs_fifo* pFifo, uint64_t* pElement);

/**
 *   \brief    从FIFO对象缓冲区查看第一个可以读出的数据，但这个数据不会出列
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  64位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_look_first64(qs_fifo* pFifo, uint64_t* pElement);

/**
 *   \brief    从FIFO对象缓冲区查看下一个可以读出的数据，但这个数据不会出列
 *   \param    pFifo     FIFO管理对象句柄
 *   \param    pElement  64位数据宽度的数据存放地址
 *   \returns  #TRUE 成功, #FALSE 失败
 */
BOOL qs_fifo_look_next64(qs_fifo* pFifo, uint64_t* pElement);

/**
 *   \brief    清空对象缓冲区，(注意，不对缓冲区中的内容进行擦除，只复位成无对象状态)
 *   \param    pFifo     FIFO管理对象句柄
 */
void qs_fifo_clear(qs_fifo *pFifo);

/**
 *   \brief    检查缓对象冲区是否为空
 *   \param    pFifo     FIFO管理对象句柄
 *   \returns  #TRUE 对象缓冲区为空状态, #FALSE 对象缓冲区为非空状态
 */
BOOL qs_fifo_empty(qs_fifo *pFifo);

/**
 *   \brief    检查缓对象冲区是否为满
 *   \param    pFifo     FIFO管理对象句柄
 *   \returns  #TRUE 对象缓冲区为满状态, #FALSE 对象缓冲区为非满状态
 */
BOOL qs_fifo_full(qs_fifo *pFifo);

/**
 *   \brief    读取当然FIFOF对象缓冲区中对象的数量
 *   \param    pFifo     FIFO管理对象句柄
 *   \returns  当前FIFO对象缓冲区中对象的数量
 */
size_t qs_fifo_level(qs_fifo *pFifo);

/**
 *   \brief    读取当然FIFOF对象缓冲区中还可写入对象的数量
 *   \param    pFifo     FIFO管理对象句柄
 *   \returns  当前FIFO对象缓冲区中还可写入对象的数量
 */
size_t qs_fifo_remaining(qs_fifo *pFifo);

/// \sa qs_fifo_alloc8
#define qs_fifo_alloc                qs_fifo_alloc8

/// \sa qs_fifo_put8
#define qs_fifo_put                  qs_fifo_put8

/// \sa qs_fifo_get8
#define qs_fifo_get                  qs_fifo_get8

/// \sa qs_fifo_look_first
#define qs_fifo_look_first           qs_fifo_look_first8

/// \sa qs_fifo_look_next
#define qs_fifo_look_next            qs_fifo_look_next8

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _QS_FIFO_H_ */
/* END OF FILE */
