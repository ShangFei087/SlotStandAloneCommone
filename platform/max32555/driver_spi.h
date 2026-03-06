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
#ifndef _QS_DRIVER_SPI_H_
#define _QS_DRIVER_SPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#if(QS_DRIVER_SUPPORT_SPI)

#include <mml_spi.h>

/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
typedef struct _driver_spi_handle
{
	const char *name;
	mml_spi_dev_e dev;
	uint32_t number;
	uint32_t BaudRate;
} driver_spi_handle;

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
 *  @param          
 *  @return         
 *  @details        
 */
void driver_spi_transmit(driver_spi_handle *dev, uint8_t *pBuf, size_t size);

/**
 *  @brief          关闭设备
 *  @param[in]      pDriver 设备句柄
 *  @return         成功，#TRUE, 失败，#FALSE
 *  @details        
 */
void driver_spi_close(driver_spi_handle *dev);

/**
 *  @brief          打开设备
 *  @param[in]      pDriver 设备句柄
 *  @return         成功，#TRUE, 失败，#FALSE
 *  @details        
 */
driver_spi_handle *driver_spi_open(const char *name);

/**
*  @brief
*  @param          无
*  @return         无
*  @details
*/
void driver_spi_exec(uint32_t millisecond);

/**
 *  @brief          向系统设备管理器安装serial设备
 *  @param          无
 *  @return         无
 *  @details        
 */
void driver_spi_install(void);

/**
 *  @brief          从系统设备管理器移除serial设备
 *  @param          无
 *  @return         无
 *  @details        
 */
void driver_spi_uninstall(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_DRIVER_SPI_H_
/* END OF FILE */
