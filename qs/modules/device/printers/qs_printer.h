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
#ifndef _QS_PRINTER_H_
#define _QS_PRINTER_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (QS_DRIVER_SUPPORT_PRINTER)

/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#define QS_PRINTER_STATE_IDLE 0
#define QS_PRINTER_STATE_NO_SELECT -1         // 未指定打印机
#define QS_PRINTER_STATE_NO_CONNECTED -2      // 打印机未连接
#define QS_PRINTER_STATE_FAILURE -3           // 打印失败
#define qs_printer_state int


/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
typedef struct _qs_printer_time {
	uint16_t seconds;
	uint16_t minutes;
	uint16_t hours;
	uint16_t week;
	uint16_t day;
	uint16_t month;
	uint16_t year;
	uint16_t millisecond;
} qs_printer_time;

typedef struct _qs_printer_context
{
	void (*pfnDataOutCallback)(uint8_t*, size_t);
	int IndexOfAcceptor;
	qs_printer_state state;
} qs_printer_context;

typedef struct _qs_printer
{
	const char* name;
	serial_conf conf;

	void(*pfnDataInCallback)(qs_printer_context*, uint8_t*, size_t);
	void(*pfnExec)(qs_printer_context*, uint32_t);

	BOOL(*pfnInitialize)(qs_printer_context*);
	BOOL(*pfnDispose)(qs_printer_context*);

	BOOL(*pfnReset)(qs_printer_context*); // 重置打印机参数
	BOOL(*pfnExecuteCut)(qs_printer_context*, BOOL); // 切纸，全切/部分切
	BOOL(*pfnFontSizeSet)(qs_printer_context*, uint32_t); // 设置文字大小
	BOOL(*pfnMessagePrint)(qs_printer_context*, char *, size_t); // 打印并切纸
	BOOL(*pfnTimeSet)(qs_printer_context*, qs_printer_time*); // 设置打印机内部时间
	BOOL(*pfnTimeGet)(qs_printer_context*, qs_printer_time*); // 读取打印机内部时间
} qs_printer;

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
  *  @brief          重启打印机参数
  *  @param          无
  *  @return         无
  *  @details
  */
BOOL qs_printer_reset(void);

/**
 *  @brief          切纸
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_printer_execute_cut(BOOL mode);

/**
 *  @brief          设定字体大小
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_printer_font_size_set(uint32_t select);

/**
 *  @brief          打印信息并切纸
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_printer_print_message(char *pMsg, size_t MsgLen);

/**
 *  @brief          设置打印机内部时间
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_printer_time_set(qs_printer_time *pTime);

/**
 *  @brief          读了打印机内部时间
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_printer_time_get(qs_printer_time* pTime);

 /**
  *  @brief          获取所有支持打印机名称列表-开始
  *  @param          无
  *  @return         无
  *  @details
  */
const char* qs_printer_name_get_start(void);

/**
 *  @brief          获取所有支持打印机名称列表-下一个
 *  @param          无
 *  @return         无
 *  @details
 */
const char* qs_printer_name_get_next(void);

/**
 *  @brief          获取指定打印机的通信参数
 *  @param          无
 *  @return         无
 *  @details
 */
serial_conf *qs_printer_serial_conf(uint32_t id);

/**
 *  @brief          获取打印机的状态
 *  @param          无
 *  @return         无
 *  @details
 */
qs_printer_state qs_printer_state_get(void);

/**
 *  @brief          初始化打印机
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_printer_initialize(uint32_t id);

/**
 *  @brief          处置打印机
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_printer_dispose(void);

/**
 *  @brief          设置发送数据回调
 *  @param          无
 *  @return         无
 *  @details
 */
void qs_printer_set_data_out_callback(void (*pfnDataOut)(uint8_t*, size_t));

/**
 *  @brief          数据输入
 *  @param          无
 *  @return         无
 *  @details
 */
void qs_printer_data_in(uint8_t *pData, size_t DataSize);

/**
 *  @brief          
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_printer_exec(uint32_t millisecond);

/**
 *  @brief          设备管理初始化操作
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_printer_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_PRINTER_H_
/* END OF FILE */
