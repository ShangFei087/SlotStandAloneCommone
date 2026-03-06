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
#ifndef _QS_NOISE_H_
#define _QS_NOISE_H_

#ifdef __cplusplus
extern "C" {
#endif



/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#define QS_NOISE_KEY                  0
#define QS_NOISE_SIG                  1

/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/
typedef struct _qs_noise {
	uint16_t time;
	uint8_t MaxTime;	
	uint8_t MaxCounter;
	uint8_t counter;
	uint8_t t0;
	uint8_t t1;

	uint8_t state;
	uint8_t s0;
	uint8_t stype;
	uint16_t stime;
	uint8_t update;
} qs_noise;


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
  *   \brief
  */
BOOL qs_noise_state_spec_high(uint32_t value);

/**
 *   \brief
 */
BOOL qs_noise_state_spec_low(uint32_t value);

/**
 *   \brief
 */
BOOL qs_noise_state_spec_rise(uint32_t value);

/**
 *   \brief
 */
BOOL qs_noise_state_spec_fall(uint32_t value);

/**
 *   \brief
 */
uint32_t qs_noise_state_spec(qs_noise *pCtx);

/**
 *   \brief
 */
BOOL qs_noise_state_reverse(qs_noise *pCtx);
/**
 *   \brief
 */
BOOL qs_noise_state_update(qs_noise *pCtx);

/**
 *   \brief
 */
BOOL qs_noise_state(qs_noise *pCtx);

/**
*   \brief
*/
BOOL qs_noise_state_ex(qs_noise *pCtx);

/**
 *   \brief 
 */
void qs_noise_exec(qs_noise *pCtx, uint32_t millisecond, uint32_t value);


/**
 *   \brief 
 */
void qs_noise_init(qs_noise *pCtx, uint32_t MaxTime, uint32_t MaxCounter);


#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_NOISE_H_
/* END OF FILE */
