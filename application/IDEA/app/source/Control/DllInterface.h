#ifndef DllInterface_h__
#define DllInterface_h__

#include "GameManager.h"
#include <idea_player.h>
#include "qs.h"

extern GameInstance_t* g_CurrentGameInstance;
typedef GameInstanceId_t GameId_t;
#define GAME_ID_INVALID GAME_INSTANCE_ID_INVALID

//设置调试信息
 void DLL_SetUserControlDebugMode(player_data_item* pUserInfo, DebugControlMode_t* pDebugMode);
 void DLL_SetControlDebugMode(DebugControlMode_t* pDebugMode);

//debugStr格式：{"Mode":0,"ResultType":1}
//Mode：0:DCM_Normal    1：DCM_PointResData
//ResultType:  0:RT_Lose  1:RT_Win  2:RT_FreeWin 3:RT_BonusWin
 void DLL_SetUserControlDebugModeJson(player_data_item* pUserInfo, int8_t debugStr[1024]);

//通过字符串调用函数
 void DLL_CallFunc( int8_t* funcName,  int8_t* inData, int8_t outData[2048]);

//初始化dll，返回游戏ID
 GameId_t DLL_GameInit(int8_t* gameName,GameId_t gameId);
//切换游戏
 int8_t DLL_GameSwitch(GameId_t gameId);
//注销游戏
 BOOL DLL_GameUnregister(GameId_t gameId);
//获取当前游戏ID
 GameId_t DLL_GetCurrentGameId(void);

 int8_t* OutResToJsonnById(OutResult_t* outRes, GameId_t gameId);
 //生成一个免费局
 void GenerateARoundWithFreeWinById(RoundInfo_t* info, GameId_t gameId, Matrix_u* freeMxu);
 //生成一个奖池局
 void GenerateARoundWithBonusWinById(RoundInfo_t* info, GameId_t gameId, Matrix_u* bonusMxu);
 //应用RoundInfo到输出结果
 void ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu);
 //为免费游戏应用矩阵
 void ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx);

//玩家开始,第一次进入游戏时，需要重置一些数据和分数，需要这些参数
 void DLL_OnBegin(player_data_item* pUserInfo, GameId_t gameId);
 void DLL_OnBeginEx(player_data_item* pUserInfo, int8_t isReconnect, GameId_t gameId);  // bool改为int32_t

//获取游戏结果（指定游戏ID）
 void DLL_GetGameResultById(player_data_item* pUserInfo, int32_t betValue, OutResult_t* outRes, int32_t* ret, GameId_t gameId);

 void GetNormalResult(player_data_item* pUserInfo, int32_t betVal, OutResult_t* outRes, int32_t* ret, GameId_t gameId);
;
//获取调试信息
 void DLL_GetDebugInfo(DebugInfo* pDebugInfo, GameId_t gameId);
 void DLL_GetUserDebugInfo(DebugInfo* pDebugInfo, GameId_t gameId);

//获取Json调试信息
 void DLL_GetDebugInfoJson(int8_t debugInfo[2048], GameId_t gameId);
 void DLL_GetUserDebugInfoJson(int32_t chairID, int8_t debugInfo[2048], GameId_t gameId);

//清除调试数据
 void DLL_DebugClearData(GameId_t gameId);

 void DLL_GameClose(GameId_t gameId);
 void DLL_GameCloseAll(void);


#endif // DllInterface_h__
