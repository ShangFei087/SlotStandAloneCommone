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
#ifndef _QS_COM_H_
#define _QS_COM_H_


#ifdef __cplusplus
extern "C" {
#endif

#if (QS_SUPPORT_COM)
/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#define QS_COM_FIFO_SIZE      (SENV_BUF_SIZE / 2)

/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/
typedef enum _qs_com_type {
	QS_COM_TYPE_NONE = 0,
	QS_COM_TYPE_DIRECT,
	QS_COM_TYPE_SENV
} qs_com_type;

typedef enum _qs_com_direct_mode {
	QS_COM_DIRECT_MODE_NONE = 0,
	QS_COM_DIRECT_MODE_LITTLEENDIAN,
	QS_COM_DIRECT_MODE_BIGENDIAN
} qs_com_direct_mode;

/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
typedef struct _qs_com_context {
	qs_driver *driver;
	uint32_t id;
	qs_com_type type;
	int req;
	qs_fifo *fifo;

	qs_senv *SenvCtx;
	int *CmdList;
	size_t CmdListSize;

	qs_json *DirectRoot;
	int DirectCmd;
	uint32_t DirectTimeout;
	uint32_t DirectUpdateTime;
	qs_com_direct_mode DirectMode;

	struct _qs_com_context *next;
} qs_com_context;

/**************************************************************************
 *                 E X T E R N A L   R E F E R E N C E S                  *
 **************************************************************************/


/**************************************************************************
 *                        G L O B A L   D A T A                           *
 **************************************************************************/
#include "qs_com_senv.h"
#include "qs_com_direct.h"

/**************************************************************************
 *               F U N C T I O N   D E C L A R A T I O N S                *
 **************************************************************************/

 /**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
qs_senv *qs_com_senv(void);

 /**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
char *qs_com_name(uint32_t id);

 /**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void qs_com_remove(uint32_t id);

/**
*  @brief
*  @param
*  @return
*  @details
*/
qs_com_context *qs_com_add(uint32_t id, qs_com_type type);

/**
*  @brief
*  @param
*  @return
*  @details
*/
qs_com_context *qs_com_get(uint32_t id);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void qs_com_callback(qs_senv *pSenv, qs_json *root);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_com_exec(uint32_t millisecond);

/**
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
void qs_com_init(qs_senv *pSenv);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_COM_H_
/* END OF FILE */
