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
#ifndef _QS_BILL_H_
#define _QS_BILL_H_

#ifdef __cplusplus
extern "C" {
#endif

#if (QS_DRIVER_SUPPORT_BILL)

/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#define QS_BILL_ACCEPTOR_STATE_IDLING  0                  // 空闲状态
#define QS_BILL_ACCEPTOR_STATE_UNSELECTED  -1             // 未指定纸钞机
#define QS_BILL_ACCEPTOR_STATE_UNCONNECTED  -2            // 纸钞机未连接
#define QS_BILL_ACCEPTOR_STATE_JAMMED  -3                 // 卡钞
#define QS_BILL_ACCEPTOR_STATE_STACKER_FULL  -4           // 钱箱满
#define QS_BILL_ACCEPTOR_STATE_FAILURE  -5                // 未知错误
#define qs_bill_state int

typedef enum _qs_bill_event
{
	QS_BILL_EVENT_CREDIT = 0,
} qs_bill_event;

/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/

typedef struct _qs_bill_credit
{
	uint32_t credit;
	uint32_t unit;
} qs_bill_credit;

typedef struct _qs_bill_acceptor_context
{
	void (*pfnEvent)(qs_bill_event, void*);
	void (*pfnDataOutCallback)(uint8_t*, size_t);
	int IndexOfAcceptor;
	qs_bill_credit credit;
	qs_bill_state state;
	BOOL stacked;
} qs_bill_acceptor_context;

typedef struct _qs_bill_acceptor
{
	const char* name;
	serial_conf conf;

	void(*pfnDataInCallback)(qs_bill_acceptor_context*, uint8_t*, size_t);
	void(*pfnExec)(qs_bill_acceptor_context*, uint32_t);

	BOOL(*pfnInitialize)(qs_bill_acceptor_context*);
	BOOL(*pfnDispose)(qs_bill_acceptor_context*);

	BOOL(*pfnEscrowApprove)(qs_bill_acceptor_context*);
	BOOL(*pfnEscrowReject)(qs_bill_acceptor_context*);

} qs_bill_acceptor;

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
  *  @brief          获取所有支持纸钞机名称列表-开始
  *  @param          无
  *  @return         无
  *  @details
  */
const char* qs_bill_acceptor_name_get_start(void);

/**
 *  @brief          获取所有支持纸钞机名称列表-下一个
 *  @param          无
 *  @return         无
 *  @details
 */
const char* qs_bill_acceptor_name_get_next(void);

/**
 *  @brief          获取指定纸钞机的通信参数
 *  @param          无
 *  @return         无
 *  @details
 */
serial_conf *qs_bill_serial_conf(uint32_t id);

/**
 *  @brief          获取纸钞机的状态
 *  @param          无
 *  @return         无
 *  @details
 */
qs_bill_state qs_bill_state_get(void);

/**
 *  @brief          获取纸钞是否放进钱箱
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_bill_is_stacked(void);

/**
 *  @brief          获取纸钞接收器当前币值
 *  @param          无
 *  @return         无
 *  @details
 */
void qs_bill_credit_get(qs_bill_credit* pCredit);

/**
 *  @brief          初始化纸钞机
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_bill_initialize(uint32_t id);

/**
 *  @brief          处置纸钞机
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_bill_dispose(void);

/**
 *  @brief          核准
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_bill_escrow_approve(void);

/**
 *  @brief          拒收
 *  @param          无
 *  @return         无
 *  @details
 */
BOOL qs_bill_escrow_reject(void);

/**
 *  @brief          设置发送数据回调
 *  @param          无
 *  @return         无
 *  @details
 */
void qs_bill_set_data_out_callback(void (*pfnDataOut)(uint8_t*, size_t));

/**
 *  @brief          数据输入
 *  @param          无
 *  @return         无
 *  @details
 */
void qs_bill_data_in(uint8_t *pData, size_t DataSize);

/**
 *  @brief          
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_bill_exec(uint32_t millisecond);

/**
 *  @brief          设备管理初始化操作
 *  @param          无
 *  @return         无
 *  @details        
 */
void qs_bill_init(void);

#endif

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_BILL_H_
/* END OF FILE */
