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
#ifndef _QS_IO_H_
#define _QS_IO_H_

#ifdef __cplusplus
extern "C" {
#endif



/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
typedef enum _qs_input_type {
	QS_INPUT_TYPE_SIG,
	QS_INPUT_TYPE_KEY
} qs_input_type;

/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/
typedef struct _qs_input {
	uint16_t RepeatPressCounter;
	uint16_t StateTimeout;
	uint16_t DelayTimeout;
	uint8_t Oned;
	uint8_t type;
	uint8_t Delaycounter;
	uint8_t DelayState0;
	uint8_t DelayState1;
	uint8_t State0;
} qs_input;

typedef struct _qs_output {
	BOOL Oned;
	BOOL EndState;
	uint32_t OnTime;
	uint32_t OffTime;
	uint32_t timeout;
	uint32_t counter;
} qs_output;

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
 *   \brief 输入信号是否有效，数值每次读取后会保持，连续有效计数器的值会保持
 */
BOOL qs_input_oned(qs_input *pCtx);

/**
 *   \brief 输入信号是否有效，数值每次读取后会清除，但连续有效计数器的值会保持
 */
BOOL qs_input_oned_reset(qs_input *pCtx);

/**
 *   \brief 输入信号连续有效计数
 */
uint32_t qs_input_oned_counter(qs_input *pCtx);

/**
 *   \brief 默认为QS_INPUT_TYPE_SIG
 */
void qs_input_set_type(qs_input *pCtx, qs_input_type type);

/**
 *   \brief 
 */
void qs_input_exec(qs_input *pCtx, uint32_t millisecond, BOOL OffOrOn);

/**
 *   \brief 
 */
void qs_input_init(qs_input *pCtx);




/**
 *   \brief
 */
void qs_output_on(qs_output *pCtx);

/**
 *   \brief
 */
void qs_output_off(qs_output *pCtx);

/**
 *   \brief
 */
void qs_output_flicker(qs_output *pCtx, uint32_t OnTime, uint32_t OffTime, uint32_t count);

/**
 *   \brief
 */
void qs_output_flicker_ex(qs_output *pCtx, uint32_t OnTime, uint32_t OffTime, uint32_t count, BOOL state);

/**
 *   \brief
 */
BOOL qs_output_oned(qs_output *pCtx);

/**
 *   \brief
 */
void qs_output_exec(qs_output *pCtx, uint32_t millisecond);

/**
 *   \brief
 */
void qs_output_init(qs_output *pCtx);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_IO_H_
/* END OF FILE */
