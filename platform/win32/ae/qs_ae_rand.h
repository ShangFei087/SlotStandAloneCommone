/**
 * @file    qs_ae_rand.h
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
#ifndef _QS_AE_RAND_H_
#define _QS_AE_RAND_H_

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus  */

#if (QS_SUPPORT_RAND > 1)
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
 *   \brief    以8位的数据宽度模式申请随机数
 *   \param    pBuffer   随机数输出缓冲区
 *   \param    length    需要申请的随机数的数量
 *   \returns  随机数的数量
 */
size_t qs_ae_rand(uint8_t *pBuffer, size_t length);

/**
 *   \brief    初始化随机数引擎
 */
void qs_ae_rand_init(void);

#endif

#ifdef __cplusplus
}
#endif /* __cplusplus  */

#endif /* _QS_AE_RAND_H_ */
/* END OF FILE */
