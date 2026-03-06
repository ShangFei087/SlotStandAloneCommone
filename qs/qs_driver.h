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
#ifndef _QS_DRIVER_H_
#define _QS_DRIVER_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "qs_platform.h"
#include "qs_board.h"

/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/     
#define QS_DRIVER_IOCTL_USER_BASE                   0x0100

#define QS_DRIVER_IOCTL_SET_CALLBACK_0              0x0001
#define QS_DRIVER_IOCTL_GET_CALLBACK_0              0x0002
#define QS_DRIVER_IOCTL_SET_CALLBACK_1              0x0003
#define QS_DRIVER_IOCTL_GET_CALLBACK_1              0x0004
#define QS_DRIVER_IOCTL_SET_ARG_0                   0x0005
#define QS_DRIVER_IOCTL_GET_ARG_0                   0x0006
#define QS_DRIVER_IOCTL_SET_ARG_1                   0x0007
#define QS_DRIVER_IOCTL_GET_ARG_1                   0x0008
#define QS_DRIVER_IOCTL_SET_BPS                     0x0009
#define QS_DRIVER_IOCTL_GET_BPS                     0x000A
#define QS_DRIVER_IOCTL_SET_POS                     0x000B
#define QS_DRIVER_IOCTL_GET_POS                     0x000C
#define QS_DRIVER_IOCTL_SET_SIZE                    0x000D
#define QS_DRIVER_IOCTL_GET_SIZE                    0x000E
#define QS_DRIVER_IOCTL_ON                          0x000F
#define QS_DRIVER_IOCTL_OFF                         0x0010
#define QS_DRIVER_IOCTL_GET                         0x0011
#define QS_DRIVER_IOCTL_SET                         0x0012
#define QS_DRIVER_IOCTL_RESET                       0x0013
#define QS_DRIVER_IOCTL_ENABLE                      0x0014
#define QS_DRIVER_IOCTL_DISABLE                     0x0015
#define QS_DRIVER_IOCTL_ERASE                       0x0016
#define QS_DRIVER_IOCTL_UPDATE                      0x0017
#define QS_DRIVER_IOCTL_DETECT                      0x0018
#define QS_DRIVER_IOCTL_SET_SECTOR_SIZE             0x0019
#define QS_DRIVER_IOCTL_GET_SECTOR_SIZE             0x001a
#define QS_DRIVER_IOCTL_SET_BLOCK_SIZE              0x001b
#define QS_DRIVER_IOCTL_GET_BLOCK_SIZE              0x001c

#define QS_DRIVER_IOCTL_SET_LOGIC_MODE              0x001d
#define QS_DRIVER_IOCTL_SET_ABSOLUTE_MODE           0x001e

#define QS_DRIVER_IOCTL_SET_CALLBACK_2              0x0020
#define QS_DRIVER_IOCTL_GET_CALLBACK_2              0x0021
#define QS_DRIVER_IOCTL_SET_CALLBACK_3              0x0022
#define QS_DRIVER_IOCTL_GET_CALLBACK_3              0x0023

/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/
typedef struct _qs_driver
{
	const char *pName;
	void       *pDriver;
	BOOL      (*pfnOpen) (void *pDriver);
	void      (*pfnClose)(void *pDriver);
	size_t    (*pfnRead) (void *pDriver, void *pData, size_t DataSize);
	size_t    (*pfnWrite)(void *pDriver, void *pData, size_t DataSize);
	BOOL      (*pfnIoctl)(void *pDriver, uint32_t CmdTag, void *pArg);
	struct _qs_driver *next;
} qs_driver;

/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
typedef enum _serial_parity
{
	SERIAL_PARITY_NONE,
	SERIAL_PARITY_ODD,
	SERIAL_PARITY_EVEN,
	SERIAL_PARITY_MARK,
	SERIAL_PARITY_SPACE,
} serial_parity;

typedef struct _serial_conf {
//	char* pName;
	uint32_t BaudRate;
	uint32_t DataBits;
	uint32_t StopBits;
	serial_parity parity;
} serial_conf;

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
*  @brief          打开指定名称的设备驱动
*  @param[in]      pName 设备在驱动程序里指定的名称
*  @return         成功，返回设备的句柄#qs_driver，失败，返回#NULL
*  @details
*/
qs_driver *qs_driver_open(const char *pName);

/**
*  @brief          关闭指定句柄的设备驱动
*  @param[in]      pCtx 设备的句柄#sbo_driver
*  @return         无
*  @details
*/
void qs_driver_close(qs_driver *pCtx);

/**
*  @brief          从指定句柄的设备驱动里读取数据
*  @param[in]      pCtx 设备驱动的句柄#sbo_driver
*  @param[out]     pData 数据缓冲区地址
*  @param[in]      DataSize 数据的字节大小
*  @return         返回实际读取的数据的字节大小
*  @details
*/
size_t qs_driver_read(qs_driver *pCtx, void *pData, size_t DataSize);

/**
*  @brief          往指定句柄的设备驱动里写入数据
*  @param[in]      pCtx 设备驱动的句柄#sbo_driver
*  @param[in]      pData 数据缓冲区地址
*  @param[in]      DataSize 数据的字节大小
*  @return         返回实际写入的数据的字节大小
*  @details
*/
size_t qs_driver_write(qs_driver *pCtx, void *pData, size_t DataSize);

/**
*  @brief          设备驱动操作控制
*  @param[in]      pCtx 设备的句柄#sbo_driver
*  @param[in]      CmdTag 操作指令
*  @param[in/out]  pArg 操作指令数据
*  @return         操作成功返回#TRUE，否则返回#FALSE
*  @details
*/
BOOL qs_driver_ioctl(qs_driver *pCtx, uint32_t CmdTag, void *pArg);

/**
*  @brief          把指定设备安装到设备管理列表
*  @param[in]      pCtx 设备的句柄#sbo_driver
*  @return         无
*  @details        本函数由驱动程序调用
*/
void qs_driver_install(qs_driver *pCtx);

/**
*  @brief          把指定设备驱动从设备管理列表移除
*  @param[in]      pCtx 设备的句柄#sbo_driver
*  @return         无
*  @details
*/
void qs_driver_uninstall(qs_driver *pCtx);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void qs_driver_exec(uint32_t millisecond);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_driver_uninit(void);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_driver_init(void);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_DRIVER_H_
/* END OF FILE */
