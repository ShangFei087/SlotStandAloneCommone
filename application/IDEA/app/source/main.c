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
#include <time.h>
#include "app.h"
#include "Control/NatureAlg.h"
#include "Control/DllInterface.h"
#include "Control/CommonStruct.h"
#include "Control/Test.h"
#include "Control/LotteryManager.h"
#include "Control/ComputerData.h"
#include "Control/TableControl.h"
#include "GameAlgo/common/JRand.h"
#include "Control/GameManager.h"
#include "Control/GameRegistry.h"
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
#ifdef _LocalDebug
     //初始化随机种子
    //测试用时间作为种子，正式的时候种子需要另外处理
    JSrand(time(NULL));
    U32 seed_array[4] = { 1 * time(NULL), 2 * time(NULL), 3 * time(NULL), 4 * time(NULL) };
    JSrandArray(seed_array, 4);

    //初始化调试
	DebugControlMode_t debugMode;
    DebugControlMode_Init(&debugMode);
	//debugMode.resType = RT_Win;
	//debugMode.resType = RT_Lose;
	debugMode.resType = RT_FreeWin;
	//debugMode.resType = RT_BonusWin;
    //debugMode.resType = RT_Jackpot;
   // debugMode.jpType = JT_Major;
    //debugMode.bonusType =0;
	debugMode.mode = DCM_PointResData;
    DLL_SetControlDebugMode(&debugMode);

    // 设置当前区域+RTP档位（这里使用国内 99.2 档）。
    DLL_SetRtpDifficulty(RTP_REGION_DOMESTIC, 3);   // 可选：先固定区域并使用该区域默认基准
    DLL_SetDifficultyLevel(3);                      // 难度
    // 设置免费/Bonus概率覆盖（-1 表示不覆盖，沿用档位默认）。
    //DLL_SetRtpPassOverride(9920, 9920);// 99.2
    //DLL_SetRtpPassOverride(9980, 9980);//99.8
   
    //初始化配置
    GameInstanceId_t gameId = GAME_ID_INVALID;
    if (!GameRegistry_InitAndRegisterDefaults())
    {
        return 0;
    }
    
    //初始化输出
    OutResult_t outres ;
    OutResult_Init(&outres);
    uint32_t totalTime = 0; // 每台机子的总玩次数
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
  
    // 本地压测机位：每个元素代表一台独立机子的账户状态（不依赖 PLAYER_NUM_MAX）。
    #define _TestMachineCount 1
    player_data_item testPlayers[_TestMachineCount];

    // 免费局状态按机位隔离，避免不同机子的免费次数互相污染。
    int32_t giveTime[_TestMachineCount];
    int32_t giveBetVal[_TestMachineCount];
    memset(testPlayers, 0, sizeof(testPlayers));
    memset(giveTime, 0, sizeof(giveTime));
    memset(giveBetVal, 0, sizeof(giveBetVal));
    for (int32_t i = 0; i < _TestMachineCount; ++i)
    {
        testPlayers[i].Bet = 20;
    }

    int32_t ret = 0;
    uint32_t TotalWin = 0;
#define _TestTime 300000

    while (totalTime < _TestTime)
    {
        totalTime++;
        //10台机子并行跑
        for (uint32_t machineIdx = 0; machineIdx < _TestMachineCount; ++machineIdx)
        {
            TotalWin = 0;
            player_data_item* pItem = &testPlayers[machineIdx];
            OutResult_Init(&outres);

            outres.openType = OT_Normal;
            if (giveTime[machineIdx] > 0)
            {
                giveTime[machineIdx]--;
                // 免费局沿用触发时的下注额（giveBetVal）。
                outres.openType = OT_Give;
            }

            uint32_t totalbet = pItem->Bet;//总押注
            uint32_t betmultiple = pItem->Bet / g_CurrentGameInstance->gameConfig.header.lineCount;//下注倍数
            if (outres.openType == OT_Give)
            {
                totalbet = giveBetVal[machineIdx];
            }
            else
            {
                pItem->Bets += totalbet;
            }
#ifdef _ExhibitionMode
            int32_t matrixData[GE_WheelChessMaxNum] = 
            { 
                9,0,9,1,10,
                9,1,2,3,7,
                0,1,0,7,0 
            };
            int32_t ArraySize = 15;
            int32_t pAppliedLen = 0;
            DLL_SaveExhibitionData(matrixData, ArraySize, &pAppliedLen);
#endif
            //获取一局数据
            DLL_GetGameResultById(pItem, betmultiple, &outres, &ret, gameId);

            if (outres.resType == RT_FreeWin)
            {
                // 触发免费游戏后，把次数和基准下注记录到当前机位。
                giveTime[machineIdx] += outres.nTotalFreeTime;
                giveBetVal[machineIdx] = totalbet;
            }

            if (outres.openType == OT_Normal)
            {
                // 本地账户统计只在普通付费局更新；免费局收益已在触发局的 nTotalFreeBet 中结算。
                TotalWin = betmultiple * (outres.nMatrixBet  + outres.nBonusBet)+ outres.nTotalJackpotBet;
                pItem->Wins += TotalWin;
            }
            else
            {
                //免费最后一局才加免费总得分
                if (g_CurrentGameInstance->freeGameInfo.nTotalFreeTime == g_CurrentGameInstance->freeGameInfo.nCurFreeIdx)
                {
                    TotalWin = betmultiple * g_CurrentGameInstance->freeGameInfo.nFreeTotalWin;
                    pItem->Wins += TotalWin;
                }
            }

            int32_t res[150] = { 0 }; //250个以下的int32_t
            //输出游戏结果到Senv
            DLL_OutResToSenvById(&outres, res, gameId);
            QS_LOG("\n", res);
        }

        // 每10000局输出一次10台机子的实时RTP
        if ((totalTime % _DebugInfoInterval) == 0)
        {
            float totalMachineRtp = 0.0f;
            QS_LOG("=== Round:%u RTP Snapshot ===\n", totalTime);
            for (int32_t i = 0; i < _TestMachineCount; ++i)
            {
                float machineRtp = 0.0f;
                if (testPlayers[i].Bets > 0)
                {
                    machineRtp = testPlayers[i].Wins * 1.0f / testPlayers[i].Bets;
                }
                totalMachineRtp += machineRtp;
                QS_LOG("Machine[%d] RTP:%f Bets:%u Wins:%u\n",
                    i,
                    machineRtp,
                    testPlayers[i].Bets,
                    testPlayers[i].Wins);
            }
            QS_LOG("Round:%u MachineAvgRTP:%f\n", totalTime, totalMachineRtp / _TestMachineCount);
            
        }
      
    }
   
    // 输出 10 台机子的独立 RTP（Wins / Bets），用于观察机位离散度。
    {
        QS_LOG("\n\n\n");
        float totalMachineRtp = 0.0f;
        for (int32_t i = 0; i < _TestMachineCount; ++i)
        {
            float machineRtp = 0.0f;
            if (testPlayers[i].Bets > 0)
            {
                machineRtp = testPlayers[i].Wins * 1.0f / testPlayers[i].Bets;
            }
            totalMachineRtp += machineRtp;
            QS_LOG("Machine[%d] Bets:%u Wins:%u RTP:%f\n",
                i,
                testPlayers[i].Bets,
                testPlayers[i].Wins,
                machineRtp);
        }
        QS_LOG("MachineAvgRTP:%f\n", totalMachineRtp / _TestMachineCount);
       
    }

    //汇总测试信息
    {
        QS_LOG("\n\n\n");
        DebugInfo userDebugInfo;
        DebugInfo_reset(&userDebugInfo);
        int8_t finalString[1024] = { 0 };
        DLL_GetUserDebugInfo(&userDebugInfo);

        sprintf(finalString, "TotalPlayTime:%d LoseProb:%f NormalWinProb:%f FreeGameProb:%f BonusGamesProb:%f JackpotProb:%f JackpotOnlineProb:%f TotalProb:%f",
            userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwLooseTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwNormalWinTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwFreeGameTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwBonusTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwJackpotTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            userDebugInfo.dwJackpotOnlineTime * 1.0f / userDebugInfo.dwTotalPlayTime,
            (userDebugInfo.dwLooseTime * 1.0f / userDebugInfo.dwTotalPlayTime + userDebugInfo.dwNormalWinTime * 1.0f / userDebugInfo.dwTotalPlayTime + userDebugInfo.dwFreeGameTime * 1.0f / userDebugInfo.dwTotalPlayTime + userDebugInfo.dwBonusTime * 1.0f / userDebugInfo.dwTotalPlayTime + userDebugInfo.dwJackpotTime * 1.0f / userDebugInfo.dwTotalPlayTime + userDebugInfo.dwJackpotOnlineTime * 1.0f / userDebugInfo.dwTotalPlayTime));
        QS_LOG("%s\n", finalString);
        QS_LOG("\r\n");
        int8_t RTPStr[512];
        //输赢概率
        float LoseProb = userDebugInfo.dwLooseTime * 1.0f / userDebugInfo.dwTotalPlayTime;
        float WinProb = userDebugInfo.dwNormalWinTime * 1.0f / userDebugInfo.dwTotalPlayTime;
        float FreeProb = userDebugInfo.dwFreeGameTime * 1.0f / userDebugInfo.dwTotalPlayTime;
        float BonusProb = userDebugInfo.dwBonusTime * 1.0f / userDebugInfo.dwTotalPlayTime;
        float JackPotProb = userDebugInfo.dwJackpotTime * 1.0f / userDebugInfo.dwTotalPlayTime;
        float JackPotOnlineProb = userDebugInfo.dwJackpotOnlineTime * 1.0f / userDebugInfo.dwTotalPlayTime;

        int64_t TotalBet = userDebugInfo.dwPlayScore;
        float BaseRTP = 0.0f;
        float FreeRTP = 0.0f;
        float BounsRTP = 0.0f;
        float JackpotRTP = 0.0f;
        float JackpotOnlineRTP = 0.0f;
        float TotalRTP = 0.0f;
        float FreePerHitRTP = 0.0f;
        float BonusPerHitRTP = 0.0f;
        float JackpotPerHitRTP = 0.0f;
        float JackpotOnlinePerHitRTP = 0.0f;
        if (TotalBet > 0)
        {
            BaseRTP = userDebugInfo.dwBaseWinScore * 1.0f / TotalBet;
            FreeRTP = userDebugInfo.dwFreeWinScore * 1.0f / TotalBet;
            BounsRTP = userDebugInfo.dwBonusWinScore * 1.0f / TotalBet;
            JackpotRTP = userDebugInfo.dwJackpotWinScore * 1.0f / TotalBet;
            JackpotOnlineRTP = userDebugInfo.dwJackpotOnlineWinScore * 1.0f / TotalBet;
            TotalRTP = userDebugInfo.dwWinScore * 1.0f / TotalBet;
        }
        if (FreeProb > 0.0f) FreePerHitRTP = FreeRTP / FreeProb;
        if (BonusProb > 0.0f) BonusPerHitRTP = BounsRTP / BonusProb;
        if (JackPotProb > 0.0f) JackpotPerHitRTP = JackpotRTP / JackPotProb;
        if (JackPotOnlineProb > 0.0f) JackpotOnlinePerHitRTP = JackpotOnlineRTP / JackPotOnlineProb;

        sprintf(RTPStr, "BaseRTP:%f FreeRTP:%f BounsRTP:%f JackpotRTP:%f JackpotOnlineRTP:%f TotalRTP:%f TotalRTPByParts:%f " ,
            BaseRTP,
            FreeRTP,
            BounsRTP,
            JackpotRTP,
            JackpotOnlineRTP,
            TotalRTP,
            BaseRTP + FreeRTP + BounsRTP + JackpotRTP + JackpotOnlineRTP
       );
        QS_LOG("%s\n", RTPStr); 
        QS_LOG("\r\n");
        {
            TableControlStats tableStats;
            memset(&tableStats, 0, sizeof(tableStats));
            
            TableControl_GetStats(&tableStats);
            QS_LOG("TableControlStats: totalBet:%lld totalFishValue:%lld paidBase:%lld paidFree:%lld paidBonus:%lld paidJackpotBonus:%lld paidJackpot:%lld paidNetJackpot:%lld\n",
                (long long)tableStats.totalBet,
                (long long)tableStats.totalFishValue,
                (long long)tableStats.paidBase,
                (long long)tableStats.paidFree,
                (long long)tableStats.paidBonus,
                (long long)tableStats.paidJackpotBonus,
                (long long)tableStats.paidJackpot,
                (long long)tableStats.paidNetJackpot);
            QS_LOG("\r\n");

            QS_LOG("TableControlStats: totalPass:%lld totalReject:%lld netJackpotHitCount:%lld netJackpotOverBudgetCount:%lld\n",
                (long long)tableStats.totalPass,
                (long long)tableStats.totalReject,
                (long long)tableStats.netJackpotHitCount,
                (long long)tableStats.netJackpotOverBudgetCount);
            QS_LOG("\r\n");

            QS_LOG("TableControlStatsReject: winPool:%lld freePool:%lld bonusPool:%lld jackpotPool:%lld freeRange:%lld bonusRange:%lld jackpotRange:%lld freePassRate:%lld bonusPassRate:%lld jackpotPassRate:%lld\n",
                (long long)tableStats.winRejectByTargetPool,
                (long long)tableStats.freeRejectByTargetPool,
                (long long)tableStats.bonusRejectByTargetPool,
                (long long)tableStats.jackpotRejectByTargetPool,
                (long long)tableStats.freeRejectByRange,
                (long long)tableStats.bonusRejectByRange,
                (long long)tableStats.jackpotRejectByRange,
                (long long)tableStats.freeRejectByPassRate,
                (long long)tableStats.bonusRejectByPassRate,
                (long long)tableStats.jackpotRejectByPassRate);
            QS_LOG("\r\n");

            QS_LOG("LotteryManager: totalPlayTime:%d totalPlay:%d totalDraw:%d frozenTime:%d scale:%d injectPermil:%d midBet:%d highBet:%d midInjectPermil:%d highInjectPermil:%d minPlayScoreToTrigger:%d minPlayGapAfterWin:%d playSinceLastWin:%d\n",
                gLotteryManager.mTotalPlayTime,
                gLotteryManager.mTotalPlay,
                gLotteryManager.mTotalDraw,
                gLotteryManager.mFrozenTime,
                gLotteryManager.mScale,
                gLotteryManager.mInjectRatePermil,
                gLotteryManager.mTierMidBet,
                gLotteryManager.mTierHighBet,
                gLotteryManager.mTierMidInjectPermil,
                gLotteryManager.mTierHighInjectPermil,
                gLotteryManager.mMinPlayScoreToTrigger,
                gLotteryManager.mMinPlayGapAfterWin,
                gLotteryManager.mPlaySinceLastWin);
            for (int32_t jpIdx = 0; jpIdx < GAME_Local_JP_MAX; ++jpIdx)
            {
                Lottery* lottery = &gLotteryManager.mLotterys[jpIdx];
                QS_LOG("\r\n");
                QS_LOG("LotteryPool[%d]: base:%d show:%d pool:%d nextThresh:%d totalAccum:%d totalGive:%d totalGiveTime:%u max:%u threshMin:%d threshMax:%d drawRate:%d accumDrawVal:%d accumScore:%d showSpeedPermil:%d\n",
                    jpIdx,
                    lottery->mBaseLottery,
                    lottery->mShowLottery,
                    lottery->mLotteryPool,
                    lottery->mNextGiveLotteryThresh,
                    lottery->mTotalAccumPool,
                    lottery->mTotalGivePool,
                    lottery->mTotalGiveTime,
                    lottery->mMaxLottery,
                    lottery->mThreshMin,
                    lottery->mThreshMax,
                    lottery->mDrawRate,
                    lottery->mAccumDrawVal,
                    lottery->mAccumScore,
                    lottery->mShowSpeedPermil);
            }
        }
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
