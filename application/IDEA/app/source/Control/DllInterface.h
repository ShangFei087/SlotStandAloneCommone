#ifndef DllInterface_h__
#define DllInterface_h__

#include "GameManager.h"
#include <idea_player.h>
#include "qs.h"

extern GameInstance_t* g_CurrentGameInstance;
typedef GameInstanceId_t GameId_t;
#define GAME_ID_INVALID GAME_INSTANCE_ID_INVALID

//设置调试信息
 void DLL_SetControlDebugMode(DebugControlMode_t* pDebugMode);
//切换游戏
 int8_t DLL_GameSwitch(GameId_t gameId);
 // 返回堆内存字符串，调用方必须 free
 int8_t* OutResToJsonnById(OutResult_t* outRes, GameId_t gameId);
 //产生一局
 void GenerateARound(RoundInfo_t* info, GameId_t gameId);
 //应用调试模式
 void ApplyDebugMode(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu ,int32_t *matrixBet, int32_t *idVec, GameId_t gameId);
 //生成一个免费局
 void GenerateARoundWithFreeWinById(RoundInfo_t* info, GameId_t gameId, Matrix_u* freeMxu);
 //生成一个奖池局
 void GenerateARoundWithBonusWinById(RoundInfo_t* info, GameId_t gameId, Matrix_u* bonusMxu);
 //应用RoundInfo到输出结果
 void ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, int32_t* idVec);
 //为免费游戏应用矩阵
 void ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx);
//获取游戏结果（指定游戏ID）
 void DLL_GetGameResultById(player_data_item* pUserInfo, int32_t betValue, OutResult_t* outRes, int32_t* ret, GameId_t gameId);

 void GetNormalResult(player_data_item* pUserInfo, int32_t betVal, OutResult_t* outRes, int32_t* ret, GameId_t gameId);

 void DLL_GetUserDebugInfo(DebugInfo* pDebugInfo, GameId_t gameId);

 void DLL_GameClose(GameId_t gameId);


#endif // DllInterface_h__
