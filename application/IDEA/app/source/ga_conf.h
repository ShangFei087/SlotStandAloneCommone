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
#ifndef _GA_CONF_H_
#define _GA_CONF_H_


#ifdef __cplusplus
extern "C" {
#endif

#include "qs.h"

/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/

#if(QS_PLATFORM == QS_PLATFORM_WIN32)
#define GA_TEST                       1
#else
#define GA_TEST                       0
#endif
#define GA_TEST_TIMEOUT               200

#define GA_PLAYER_NUM                 10         // 支持的最大玩家数
#define GA_BASE_ROLE_NUM              4          // 基本游戏角色数量
#define GA_ADD_ROLE_NUM               1          // 附加游戏角色数量
#define GA_ROLE_NUM                  (GA_BASE_ROLE_NUM + GA_ADD_ROLE_NUM)

#define GA_BASE_ROLL_SIZE             100        // 每轮有多少局游戏

/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/
typedef struct _ga_conf_context {
	uint32_t TypeOfPlace;                     // 场地类型，0：普通，1：技巧，2：专家
	uint32_t difficulty;                      // 难度，0~8
	uint32_t odds;                            // 倍率，0：低倍率，1：高倍率，2：随机
	uint32_t discount;                        // 放水
	uint32_t funded;                          // 自备款
	uint32_t wave;                            // 波动分数
	uint32_t BetsMinOfJackpot;                // 中彩金最小押分值
	uint32_t LimitBetsWins;                   // 限红
	uint32_t JackpotLevel;                    // 彩金等级（0~6）
} ga_conf_context;

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

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _GA_CONF_H_
/* END OF FILE */
