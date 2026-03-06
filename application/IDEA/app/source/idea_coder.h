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
#ifndef _IDEA_CODER_H_
#define _IDEA_CODER_H_


#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/


/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/

typedef struct _idea_coder_code_context {
	uint32_t uid;                // 机台号
	uint32_t bets;               // 总押分
	uint32_t wins;               // 总得分
	uint32_t times;              // 打码次数
	uint32_t token;              // 校验码
} idea_coder_code_context;

typedef struct _idea_coder_code_data {
	// 天数：0: 不修改，1：1天、2：3天、3：7天、4：15天、5：30天、6：90天、7：180天、8：360天、9：永久
	uint32_t days : 4;

	// 难度：0: 保持-?，1~9级，值越大越难
	uint32_t difficulty : 4;

	// 盈利当机：0: 不修改，1：10万，2：20万、3：50万、4：100万、5：200万、6：500万、7：1000万、8：2000万、9：无限
	uint32_t limit : 4;

	// 放水：0: 不修改，1：1万、2：2万、3：3万、4：5万、5：10万、6：20万、7：30万、8：50万、9：80万
	uint32_t discount : 4;

	// 自备款：0: 不修改，1：自备款百份比为0、2：自备款百份比为50、3：自备款百份比为100
	uint32_t funded : 2;

	// 归零：0：不清零、1：正常清零、2：炒场清零
	uint32_t clean : 2;

	// 波动大小：0: 保持-?，1：10000、2：20000、3：50000、4：100000、5：200000、6：500000
	uint32_t wave : 3;

	// 内部打码器
	uint32_t root : 1;

	// ---------------------------
	uint32_t rfu0 : 4;

	// 3位
	uint32_t rfu : 3;

	// CRC8-MSB
	uint32_t msb : 1;
} idea_coder_decode_data;


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
BOOL idea_coder_request(idea_coder_code_context* pCtx);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
BOOL idea_coder_code(idea_coder_code_context* pCtx, idea_coder_decode_data* pData, uint64_t* pCodeNumber);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
BOOL idea_coder_decode(idea_coder_code_context* pCtx, idea_coder_decode_data* pData, uint64_t CodeNumber);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
uint32_t idea_coder_state(void);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
void idea_coder_update(void);

/**
 *  @brief
 *  @param
 *  @return
 *  @details
 */
int32_t idea_coder_remain_minute(void);

void idea_coder_set_minute(int32_t minute);

/**
 *  @brief      循环调用函数，本函数会被系统循环，禁止在函数里使用死锁语句
 *  @param[in]  millisecond 每次被调用的时间差，毫秒为单位        
 *  @return         
 *  @details        
 */
void idea_coder_exec(uint32_t millisecond);

/**
 *  @brief      在这里初始化你的应用      
 *  @param          
 *  @return         
 *  @details        
 */
void idea_coder_init(void);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _IDEA_CODER_H_
/* END OF FILE */
