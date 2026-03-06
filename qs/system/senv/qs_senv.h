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
#ifndef _QS_SENV_H_
#define _QS_SENV_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (QS_SUPPORT_SENV)

/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#if(QS_SENV_SMALL)
#define SENV_BUF_SIZE                   1300	
#else
#define SENV_BUF_SIZE                   5140	
#endif

/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
typedef struct _qs_senv {

	// 通信数据流，内部注册，外部调用，把数据送进来
	void(*pfnIOStreamInput)(struct _qs_senv *pSenv, uint8_t *pData, size_t DataSize);

	// 通信数据流，外部注册，内部调用，把数据传出去
	void(*pfnIOStreamOutput)(struct _qs_senv *pSenv, uint8_t *pData, size_t DataSize);


	// 应用数据输入，由SenvManager调用，函数退出后，SenvManager负责对json释放
	void(*pfnAppDataInput)(struct _qs_senv *pSenv, uint8_t *pData, size_t DataSize);

	// 应用数据输出, 内部调用后，json的生命周期由SenvManager负责
	void(*pfnAppDataOut)(uint8_t *pData, size_t DataSize);


	// 其它定义
	void(*pfnExec)(struct _qs_senv *pSenv, uint32_t millisecond);
	uint32_t address;
	void *pPrivate;

	void *pExternal;
} qs_senv;

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
qs_senv *qs_senv_alloc(void);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_senv_free(qs_senv *pSenv);

/**
*  @brief
*  @param
*  @return
*  @details
*/
BOOL qs_senv_connected(qs_senv *pSenv);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_senv_exec(uint32_t millisecond);

/**
*  @brief
*  @param
*  @return
*  @details
*/
void qs_senv_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_SENV_H_
/* END OF FILE */
