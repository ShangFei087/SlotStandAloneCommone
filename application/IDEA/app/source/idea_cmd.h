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
#ifndef _IDEA_CMD_H_
#define _IDEA_CMD_H_


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
 *  @brief      循环调用函数，本函数会被系统循环，禁止在函数里使用死锁语句
 *  @param[in]  millisecond 每次被调用的时间差，毫秒为单位        
 *  @return         
 *  @details        
 */
void idea_cmd_exec(uint32_t millisecond);

/**
 *  @brief      在这里初始化你的应用      
 *  @param          
 *  @return         
 *  @details        
 */
void idea_cmd_init(void);

void idea_cmd_close(void);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _IDEA_CMD_H_
/* END OF FILE */
