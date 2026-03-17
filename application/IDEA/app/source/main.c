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
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "app.h"
#include"Control/NatureAlg.h"
#include"Control/DllInterface.h"
#include "Control/CMD_Fish.h"
#include "Control/Test.h"
#include "Control/LotteryManager.h"
#include "Control/ComputerData.h"
#include "GameAlgo/common/JRand.h"
#include "Control/GameManager.h"
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
 *  @brief          
 *  @param          
 *  @return         
 *  @details        
 */
int32_t main(int32_t argc, char *argv[])
{
#ifdef _WIN32
	SetConsoleOutputCP(CP_UTF8);
#endif//
	qs_init();
	app_init();
	qs_start();

 //测试
#ifdef LocalDebug
    comm_data CommData;
    uint32_t result = 0;
	result = comm_data_set(&CommData);
	if (!result)
	{
		QS_LOG("init Conf failed\n");
	}

	DebugControlMode_t debugMode;
    DebugControlMode_Init(&debugMode);
	debugMode.resType = RT_Win;
	//debugMode.resType = RT_Lose;
	//debugMode.resType = RT_FreeWin;
	//debugMode.resType = RT_BonusWin;
    //debugMode.bonusType =1;
	debugMode.mode = DCM_PointResData;

	//debugMode.mode = DCM_Normal;
    DLL_SetControlDebugMode(&debugMode);

    GameManager_Init();
    //初始化随机种子
    JSrand(12345);
    U32 seed_array[4] = { 0x123, 0x456, 0x789, 0xabc };
    JSrandArray(seed_array, 4);
    //初始化输出
    OutResult_t outres ;
    OutResult_Init(&outres);
    //初始化彩金
    LotteryManager_Init();
    int32_t baseValueArray[3] = { 3000,1500,500 };
    int32_t maxValueArray[3] = { JPWeight[1], JPWeight[2] , JPWeight[3] };
    LotteryManager_SetBaseValue(&gLotteryManager, baseValueArray, maxValueArray);

    int32_t giveTime = 0;//免费次数
    int32_t giveBetVal = 0;//免费押注
    int32_t totalTime = 0; //总玩次数
    GameId_t gameId = GAME_ID_INVALID;
    SlotGameConfig_t tempConfig;
    GameConfig_Init(&tempConfig);
    //初始化ZhuZaiJinBi_1700
    {
        gameId = GameManager_RegisterGame("ZhuZaiJinBi_1700", 1700);//注册1700
        //初始化配置
        //GameConfigHeader_t
        tempConfig.header.id = 0;               // 内部id
        tempConfig.header.enabled = 0;          // 是否启用
        tempConfig.header.gameMode = 0;         // 游戏模式：0=普通，1=技巧，2=专家
        tempConfig.header.difficulty = 0;       // 难度：0-8
        tempConfig.header.minBet = 0;            // 最小押注
        tempConfig.header.maxBet = 0;           // 最大押注
        tempConfig.header.ChessTypeNum = 12;    //总计图标个数
        tempConfig.header.ChessNorTypeNum = 9;  //普通图标个数
        tempConfig.header.lineCount = 15;       // 线数
        tempConfig.header.reelCount = 5;        // 列数
        tempConfig.header.rowCount = 3;         // 行数
        tempConfig.header.MaxIDNyn = 15;        //最大可以消除线数
        tempConfig.header.freeGameMax = 20;     //最大免费游戏次数
        tempConfig.header.Wild = 9;              // Wild图标
        tempConfig.header.Scatter = 10;          // 免费图标
        tempConfig.header.Bonus =11;             // 大奖图标 
        tempConfig.header.normalRollTableId = 0;             // 普通滚轮表id
        tempConfig.header.freeRollTableId = 0;             // 免费滚轮表id

        GameConfig_Copy(&GameManager_GetInstance(gameId)->gameConfig, &tempConfig);
       
    }

    GameConfig_Init(&tempConfig);
    //初始化CaiFuZhiMen_3999
    {
        gameId = GameManager_RegisterGame("CaiFuZhiMen_3999", 3999);//注册3999
        //初始化配置
       //GameConfigHeader_t
        tempConfig.header.id = 1;               // 内部id
        tempConfig.header.enabled = 0;          // 是否启用
        tempConfig.header.gameMode = 0;         // 游戏模式：0=普通，1=技巧，2=专家
        tempConfig.header.difficulty = 0;       // 难度：0-8
        tempConfig.header.minBet = 0;            // 最小押注
        tempConfig.header.maxBet = 0;           // 最大押注
        tempConfig.header.ChessTypeNum = 12;    //总计图标个数
        tempConfig.header.ChessNorTypeNum = 9;  //普通图标个数
        tempConfig.header.lineCount = 20;       // 线数
        tempConfig.header.reelCount = 5;        // 列数
        tempConfig.header.rowCount = 3;         // 行数
        tempConfig.header.MaxIDNyn = 20;        //最大可以消除线数
        tempConfig.header.freeGameMax = 12;     //最大免费游戏次数
        tempConfig.header.Wild = 9;              // Wild图标
        tempConfig.header.Scatter = 10;          // 免费图标
        tempConfig.header.Bonus = 11;             // 大奖图标 
        tempConfig.header.normalRollTableId = 1;             // 普通滚轮表id
        tempConfig.header.freeRollTableId = 1;             // 免费滚轮表id

        GameConfig_Copy(&GameManager_GetInstance(gameId)->gameConfig, &tempConfig);
    }

    GameConfig_Init(&tempConfig);
    //初始化XingYunZhiLun_3998
    {
        gameId = GameManager_RegisterGame("XingYunZhiLun_3998", 3998);//注册3998
        //初始化配置
       //GameConfigHeader_t
        tempConfig.header.id = 2;               // 内部id
        tempConfig.header.enabled = 0;          // 是否启用
        tempConfig.header.gameMode = 0;         // 游戏模式：0=普通，1=技巧，2=专家
        tempConfig.header.difficulty = 0;       // 难度：0-8
        tempConfig.header.minBet = 0;            // 最小押注
        tempConfig.header.maxBet = 0;           // 最大押注
        tempConfig.header.ChessTypeNum = 10;    //总计图标个数
        tempConfig.header.ChessNorTypeNum = 8;  //普通图标个数
        tempConfig.header.lineCount = 20;       // 线数
        tempConfig.header.reelCount = 5;        // 列数
        tempConfig.header.rowCount = 3;         // 行数
        tempConfig.header.MaxIDNyn = 20;        //最大可以消除线数
        tempConfig.header.freeGameMax = 6;     //最大免费游戏次数
        tempConfig.header.Wild = 8;              // Wild图标
        tempConfig.header.Bonus = 9;             // 大奖图标 
        tempConfig.header.Scatter = 10;          // 免费图标 
        tempConfig.header.normalRollTableId = 2;             // 普通滚轮表id
        tempConfig.header.freeRollTableId = 3;             // 免费滚轮表id

        GameConfig_Copy(&GameManager_GetInstance(gameId)->gameConfig, &tempConfig);
    }

	//切换游戏
	if (DLL_GameSwitch(3998))
	{
		gameId = 3998;
	
	}

    if (gameId == GAME_ID_INVALID) 
    {
        QS_LOG("init game failed\n");
        return -1;
    }
  
    uint32_t player_id = 0;
    player_data_item* pItem = player_statistics(player_id);
    uint32_t CoinIn = 2;

    player_coin_info_update(player_id, CoinIn, 0, 0, 0, 0);
    pItem->Credit = CoinIn * Score_Scale;
    pItem->Wins = 0;
    pItem->Bet = 100 ;
   
    int32_t ret = 0;
    int32_t jpvaule = 0;
    int32_t jpType = 0;
    uint32_t TotalWin = 0;
#define _TestTime 10000

    while (totalTime < _TestTime)
    {
        int32_t jpvaule = 0;
        totalTime++;
    
        outres.openType = OT_Normal;
        if (giveTime > 0)
        {
            giveTime--;
            outres.openType = OT_Give;
        }

        int32_t betValue = pItem->Bet;
        if (outres.openType == OT_Give)
        {
            betValue = giveBetVal;
        }
        else
        {
            pItem->Credit -= betValue;
            pItem->Bets += betValue;
            //检测是否可以获得彩金
            LotteryManager_OnPlay(&gLotteryManager, betValue);
            LotteryManager_TryGetLottery(&gLotteryManager, betValue, &jpType, &jpvaule);
            if (jpvaule > 0)
            {
                outres.JPType = jpType;
                outres.nJPBet = jpvaule;
            }
        }

        DLL_GetGameResultById(pItem, betValue, &outres, &ret, gameId);

        if (outres.resType == RT_FreeWin)
        {
            giveTime += outres.nTotalFreeTime;
            giveBetVal = betValue;
        }
       
       
        if (outres.openType == OT_Normal)
        {
            TotalWin = pItem->Bet*(outres.nMatrixBet + outres.nTotalFreeBet + outres.nBonusBet);
            pItem->Credit += TotalWin;
            pItem->Wins += TotalWin;
        }
      
      
       
    }


    QS_LOG("\n\n\n");
    DebugInfo userDebugInfo;
   
    //汇总测试信息
    {
        DebugInfo_reset(&userDebugInfo);
        int8_t finalString[1024] = { 0 };
        DLL_GetUserDebugInfo(&userDebugInfo, gameId);

        sprintf(finalString, "TotalPlayTime:%d LoseProb:%f NormalWinProb:%f FreeGameProb:%f BonusGamesProb:%f  TotalProb:%f",
            userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwLooseTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwNormalWinTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwFreeGameTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwBonusTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            (userDebugInfo.dwLooseTime * 1.0f / userDebugInfo.dwTotalPlayTime + userDebugInfo.dwNormalWinTime * 1.0f / userDebugInfo.dwTotalPlayTime + userDebugInfo.dwFreeGameTime * 1.0f / userDebugInfo.dwTotalPlayTime + userDebugInfo.dwBonusTime * 1.0f / userDebugInfo.dwTotalPlayTime));
        QS_LOG("%s\n", finalString);

        int8_t RTPStr[200];
        //输赢概率
        float LoseProb = userDebugInfo.dwLooseTime * 1.0f / userDebugInfo.dwTotalPlayTime;
        float WinProb = userDebugInfo.dwNormalWinTime * 1.0f / userDebugInfo.dwTotalPlayTime;
        float FreeProb = userDebugInfo.dwFreeGameTime * 1.0f / userDebugInfo.dwTotalPlayTime;
        float BonusProb = userDebugInfo.dwBonusTime * 1.0f / userDebugInfo.dwTotalPlayTime;

        int32_t TotalBet = userDebugInfo.dwPlayScore;
        float BaseRTP = userDebugInfo.dwNormalWinTotalBet * 1.0f / TotalBet;
        float FreeRTP = userDebugInfo.dwFreeGameTotalBet * 1.0f / TotalBet;
        float BounsRTP = userDebugInfo.dwBonusGameTotalBet * 1.0f / TotalBet;
        sprintf(RTPStr, "BaseRTP:%f FreeRTP:%f-%f BounsRTP:%f-%f TotalRTP:%f",
            BaseRTP,
            FreeRTP / FreeProb,
            FreeRTP,
            BounsRTP / BonusProb,
            BounsRTP,
            BaseRTP + FreeRTP + BounsRTP);
        QS_LOG("%s\n", RTPStr); \
    }

   
   
#endif

	while(1) //主循环
	{
		qs_exec();
	}

    idea_app_close();
	return 0;
}

/* END OF FILE */
