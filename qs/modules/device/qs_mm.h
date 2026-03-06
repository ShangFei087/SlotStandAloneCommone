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
#ifndef _QS_MM_H_
#define _QS_MM_H_


#ifdef __cplusplus
extern "C" {
#endif

#if(QS_SUPPORT_MM)
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/
typedef struct _qs_mm {
	qs_driver *pDriver;
	size_t SizeOfMemory;
	size_t SizeOfBlock;
	uint8_t *pMemKey;
	size_t MemKeySize;
	uint32_t BlockAddress;
	uint8_t *pBlock;
	BOOL Update;
} qs_mm;

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
 *  @brief          读出存储数据
 *  @param[in]      pCtx 存储设备结构体对象
 *  @param[in]      address 开始写入地址
 *  @param[in]      pData 数据缓冲区地址
 *  @param[in]      NumberOfByteToRead 需要读的数据字节数
 *  @return         返回实际读入的字节数
 *  @details
 */
size_t qs_mm_read(qs_mm *pCtx, uint32_t address, uint8_t *pData, size_t NumberOfByteToRead);

/**
*  @brief          写入存储数据
*  @param[in]      pCtx 存储设备结构体对象
*  @param[in]      address 开始写入地址
*  @param[in]      pData 数据缓冲区地址
*  @param[in]      NumberOfByteToWrite 需要写的数据字节数
*  @return         返回实际写入的字节数
*  @details
*/
size_t qs_mm_write(qs_mm *pCtx, uint32_t address, uint8_t *pData, size_t NumberOfByteToWrite);

/**
*  @brief          打开存储设备
*  @param[in]      pName 存储设备名称
*  @return         存储设备结构体
*  @details
*/
qs_mm *qs_mm_open(char *pName);

/**
*  @brief          关闭存储设备
*  @param[in]      pCtx 存储设备结构体对象
*  @return         
*  @details
*/
void qs_mm_close(qs_mm *pCtx);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_mm_exec(uint32_t millisecond);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_mm_init(void);
#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_MM_H_
/* END OF FILE */
