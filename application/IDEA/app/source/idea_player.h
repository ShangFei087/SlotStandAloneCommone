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
#ifndef _IDEA_PLAYER_H_
#define _IDEA_PLAYER_H_


#ifdef __cplusplus
extern "C" {
#endif


/**************************************************************************
 *                   G E N E R A L    C O N S T A N T S                   *
 **************************************************************************/
#define PLAYER_NUM_MAX                8      // 玩家数量
#define PLAYER_ROLE_NUM               15     // 角色数量

#define MAGIC_NUMBER				  0xd1a65b2e //3517340462
/**************************************************************************
 *                         D A T A   T Y P E S                            *
 **************************************************************************/


/**************************************************************************
 *                             M A C R O S                                *
 **************************************************************************/
    typedef struct _player_data_item {
        // 现金相关（物理硬币/钞票）
        uint32_t CoinIn;      // 投币次数 - 玩家投入的硬币/钞票数量
        uint32_t CoinOut;     // 退币次数 - 系统退出的硬币/钞票数量

        // 积分相关（虚拟分数）
        uint32_t ScoreUp;     // 上分 - 玩家充值/增加的游戏积分
        uint32_t ScoreDown;   // 下分 - 玩家兑换/减少的游戏积分

        // 票据系统
        int32_t  Ticket;      // 票据数量 - 可能用于彩票或兑换凭证（可为负？）

        // 账户余额
        uint32_t Credit;      // 当前积分/余额 - 玩家当前可用的游戏分数
        uint32_t CreditWin;   // 赢得的积分 - 本次游戏赢得的分数（可能临时存储）

        // 游戏投注统计
        uint32_t Bets;        // 总押注 - 玩家累计下注的总分数
        uint32_t Wins;        // 总赢分 - 玩家累计赢取的总分数
        int32_t  Profit;      // 盈利 - 净收益（赢分-押注），可为正负

        // 其他
        uint32_t lp;          // 保留字段或特定标识（可能是"loyalty points"忠诚度积分）

        // 当前游戏状态
        uint32_t Bet;         // 玩家押分 - 当前局的下注分数
        int32_t  GameStatus;  // 游戏的状态 - 当前游戏状态（如：0=等待,1=进行中,2=免费游戏等）
    } player_data_item;

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
 *  @brief      玩家本局押分和中奖信息
 *  @param[in]  PlayerId 玩家ID
 *  @param[in]  bets 玩家本局押分
 *  @param[in]  wins 玩家本局中奖分
 *  @return     成功：TRUE，失败：FALSE
 *  @details
 */
BOOL player_bets_info_update(uint32_t PlayerId, uint32_t bets, uint32_t wins);

void player_total_bets_wins(uint64_t* pBets, int64_t* pWins);

void player_wins_to_credit(uint32_t PlayerId);

void player_reset(void);

// 打码周期内
void player_total_reset_current_wins(void);

// 打码周期内
int64_t player_total_get_current_wins(void);

// 指定周期内
void player_total_reset_custom_wins(void);

// 指定周期内
int64_t player_total_get_custom_wins(void);

void player_update_credit(uint32_t PlayerId, uint32_t credit);

BOOL player_win_lock(uint32_t PlayerId);

/**
 *  @brief      玩家币信息更新
 *  @param[in]  PlayerId 玩家ID
 *  @param[in]  CoinIn 玩家进币分
 *  @param[in]  CoinOut 玩家退币分
 *  @param[in]  CoinOutWin 玩家退币分（赢分部分）
 *  @param[in]  ScoreUp 玩家上分数
 *  @param[in]  ScoreDown 玩家下分数
 *  @return     成功：TRUE，失败：FALSE
 *  @details
 */
BOOL player_coin_info_update(uint32_t PlayerId, uint32_t CoinIn, uint32_t CoinOut, uint32_t CoinOutWin, uint32_t ScoreUp, uint32_t ScoreDown);

/**
 *  @brief      获取玩家账目
 *  @param[in]  PlayerId 玩家ID
 *  @return
 *  @details
 */
player_data_item* player_statistics(uint32_t PlayerId);

BOOL player_data_null(void);

/**
 *  @brief      循环调用函数，本函数会被系统循环，禁止在函数里使用死锁语句
 *  @param[in]  millisecond 每次被调用的时间差，毫秒为单位        
 *  @return     
 *  @details        
 */
void idea_player_exec(uint32_t millisecond);

/**
 *  @brief      在这里初始化你的应用      
 *  @param          
 *  @return         
 *  @details        
 */
void idea_player_init(void);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _IDEA_PLAYER_H_
/* END OF FILE */
