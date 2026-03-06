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
#ifndef _IDEA_COMM_H_
#define _IDEA_COMM_H_

#include "idea_coder.h"
#include "ga_conf.h"
#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#define COMM_BET_SWITCH_NUM               4

#define COMM_STIRFRY_SCORE                5000000

#define COMMON_MAGIC_NUMBER				  0xeb8a3c1e //0xab893c2e  0xeb8a3c1e=3951705118

/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/



/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/

typedef struct _comm_data {
	uint32_t flag;
	uint32_t PlaceType;                       // 场地类型，0：普通，1：技巧，2：专家
	uint32_t difficulty;                      // 难度，0~8
	uint32_t odds;                            // 倍率，0：低倍率，1：高倍率，2：随机

	int32_t WinLock;                          // 盈利宕机
	uint32_t MachineId;                       // 机台编号，8位有效十进制数
	uint32_t LineId;                          // 线号，4位有效十进制数

	uint32_t TicketMode;                      // 退票模式，0：即中即退，1：退票
	uint32_t TicketValue;                     // 1票对应几分（彩票比例）
	uint32_t scoreTicket;                     // 1分对应几票
	uint32_t CoinValue;                       // 投币比例
	uint32_t MaxBet;                          // 最大押注
	uint32_t MinBet;                          // 最小押注
	uint32_t CountDown;                       // 例计时
//	uint32_t BetSwitch[COMM_BET_SWITCH_NUM];  // 押注切换值，1，10，50，100

	BOOL MachineIdLock;                       // 1：机台号已锁定，除超级管理员外，无法更改，0：机台号未锁定

	uint32_t discount;                        // 放水
	uint32_t funded;                          // 自备款
	uint32_t wave;                            // 波动分数

	uint32_t BetsMinOfJackpot;                // 中彩金最小押分值
	uint32_t JackpotStartValue;               // 彩金初始值
	uint32_t StirFryValue;                    // 炒场目标押分量
	uint32_t StirFryminute;                   // 炒场目标天数（7天）
	uint32_t LimitBetsWins;                   // 限红值

	uint32_t ReturnScore;                     // 返分值
	uint32_t ReturnScoreRTP;                  // 返分率，千分几

	uint32_t SwitchBetsUnitMin;               // 切换单位小
	uint32_t SwitchBetsUnitMid;               // 切换单位中
	uint32_t SwitchBetsUnitMax;               // 切换单位大

	uint32_t ScoreUpUnit;                     // 上分单位
	uint32_t PrintMode;                       // 打单模式，0：不打单，1：正常打单，2：伸缩打单
	uint32_t ShowMode;                        // 显示模式
	uint32_t CheckTime;                       // 对单时间
	uint32_t OpenBoxTime;                     // 开箱时间
	uint32_t PrintLevel;                      // 打印深度
	uint32_t PlayerWinLock;                   // 分机爆机分数
	uint32_t LostLock;                        // 全台爆机分数
	uint32_t CustomLostLock;                  // 自定义爆机分数
	uint32_t PulseValue;                      // 脉冲比例
	uint32_t NewGameMode;                     // 开始新一轮游戏模式，0：自动开始，1：手动开始
	uint32_t NetJackpot;               // 彩金初始值
} comm_data;

/**************************************************************************
 *                 E X T E R N A L   R E F E R E N C E S                  *
 **************************************************************************/


/**************************************************************************
 *                        G L O B A L   D A T A                           *
 **************************************************************************/


/**************************************************************************
 *               F U N C T I O N   D E C L A R A T I O N S                *
 **************************************************************************/

typedef BOOL(*fptrAfterConf)(ga_conf_context*);
/**
 *  @brief      得到一个uint32_t数值的有效十进制位数
 *  @param[in]  value 值        
 *  @return         
 *  @details        
 */
uint32_t comm_value_length(uint32_t value);

/**
 *  @brief      投币比例
 *  @param[in]
 *  @return		投币比例
 *  @details
 */
uint32_t comm_coin_value(void);

/**
 *  @brief      1票对应几分（彩票比例）
 *  @param[in]
 *  @return		1票对应几分（彩票比例）
 *  @details
 */
uint32_t comm_ticket_value(void);

/**
 *  @brief      1分对应几票
 *  @param[in]
 *  @return		1分对应几票
 *  @details
 */
uint32_t comm_score_ticket(void);

/**
 *  @brief      上分单位，每次上分的最小值
 *  @param[in]
 *  @return
 *  @details
 */
uint32_t comm_score_up_unit(void);

/**
 *  @brief      脉冲比例
 *  @param[in]
 *  @return
 *  @details
 */
uint32_t comm_pulse_value(void);

/**
 *  @brief      最大押注
 *  @param[in]
 *  @return
 *  @details
 */
uint32_t comm_bets_max(void);

/**
 *  @brief      最小押注
 *  @param[in]
 *  @return
 *  @details
 */
uint32_t comm_bets_min(void);

/**
 *  @brief      打单模式
 *  @param[in]
 *  @return
 *  @details
 */
uint32_t comm_print_mode(void);

/**
 *  @brief      打单模式
 *  @param[in]
 *  @return
 *  @details
 */
uint32_t comm_print_level(void);

/**
 *  @brief      玩家分数最大限额
 *  @param[in]
 *  @return
 *  @details
 */
uint32_t comm_player_credit_max(void);

/**
 *  @brief      押注单位表
 *  @param[in]
 *  @return
 *  @details
 */
size_t comm_bets_switch_table(uint32_t* pTable, size_t TableSize);

/**
 *  @brief      读取参数
 *  @param[out] pData 数据结构体
 *  @return		成功：TRUE，失败：FALSE
 *  @details
 */
BOOL comm_data_get(comm_data *pData);

/**
 *  @brief      设定参数
 *  @param[in]	pData 数据结构体
 *  @return		成功：TRUE，失败：FALSE
 *  @details
 */
uint32_t comm_data_set(comm_data* pData);

BOOL comm_machine_id_lock(void);

int32_t comm_win_lock_Balance(void);

uint32_t comm_get_device_type_by_device_id(void);

uint32_t comm_data_set_coder(idea_coder_decode_data* pData);


/**
 *  @brief      循环调用函数，本函数会被系统循环，禁止在函数里使用死锁语句
 *  @param[in]  millisecond 每次被调用的时间差，毫秒为单位        
 *  @return         
 *  @details        
 */
void idea_comm_exec(uint32_t millisecond);

/**
 *  @brief      在这里初始化你的应用      
 *  @param          
 *  @return         
 *  @details        
 */
void idea_comm_init(void);

/**
 *  @brief      在这里设置后调用算法的处理
 *  @param
 *  @return
 *  @details
 */
void idea_comm_after_set(fptrAfterConf pFunc);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _IDEA_COMM_H_
/* END OF FILE */
