#ifndef DllInterface_h__
#define DllInterface_h__

#include "GameManager.h"
#include "RtpConfig.h"
#include <idea_player.h>
#include "qs.h"

extern GameInstance_t* g_CurrentGameInstance;

//设置调试信息
 void DLL_SetControlDebugMode(DebugControlMode_t* pDebugMode);
//切换游戏
 int8_t DLL_GameSwitch(GameInstanceId_t gameId);
 // 返回堆内存字符串，调用方必须 free
 int8_t* OutResToJsonnById(OutResult_t* outRes, GameInstanceId_t gameId);
 //随机产生一局
 void GenerateARound(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal, int32_t* matrixBet, int32_t* idVec, GameInstanceId_t gameId);
 //应用调试模式
 void ApplyDebugMode(RoundInfo_t* info, GameInstance_t* inst, Matrix_u* mxu, int32_t betVal,int32_t *matrixBet, int32_t *idVec, GameInstanceId_t gameId);
 //生成一个免费局
 void GenerateARoundWithFreeWinById(RoundInfo_t* info, int32_t betVal, GameInstanceId_t gameId, Matrix_u* freeMxu);
 //生成一个大奖局
 void GenerateARoundWithBonusWinById(RoundInfo_t* info, int32_t betVal, GameInstanceId_t gameId, Matrix_u* bonusMxu);
 //生成一个输局
 void GenerateARoundWithLoseById(GameInstance_t* inst, Matrix_u* LoseMxu, int32_t* idVec, GameInstanceId_t gameId);
 //应用RoundInfo到输出结果
 void ApplyMatrixToOutResByRound(OutResult_t* pRes, int8_t resType, RoundInfo_t* info, Matrix_u* Mxu, int32_t* idVec, GameInstanceId_t gameId);
 //为免费游戏应用矩阵
 void ApplyMatrixToOutResForFree(OutResult_t* pRes, RoundInfo_t* info, int8_t freeIdx, GameInstanceId_t gameId);
 //请求结果
 void GetNormalResult(player_data_item* pUserInfo, int32_t betVal, OutResult_t* outRes, int32_t* ret, GameInstanceId_t gameId);
//获取游戏结果（指定游戏ID）
 void DLL_GetGameResultById(player_data_item* pUserInfo, int32_t betValue, OutResult_t* outRes, int32_t* ret, GameInstanceId_t gameId);
 // 设置难度配置：region=RTP_REGION_DOMESTIC / RTP_REGION_OVERSEAS，rtpPermyriad 例如 9920
 int32_t DLL_SetRtpDifficulty(uint8_t region, int32_t rtpPermyriad);
// 按等级设置当前区域 RTP 档位（1~5，对应低到高 RTP 档）。
int32_t DLL_SetDifficultyLevel(int32_t level);
 // 设置放行概率覆盖（万分比），传负数表示取消覆盖并恢复档位默认值
 void DLL_SetRtpPassOverride(int32_t freePassPermyriad, int32_t bonusPassPermyriad);
// 设置本地彩金放行概率覆盖（万分比），传负数表示取消覆盖并恢复档位默认值
void DLL_SetJackpotPassOverride(int32_t jackpotPassPermyriad);
// 上报联网 Jackpot 派彩（用于 TableControl 联网彩金统计）
void DLL_OnJackpotOnlineWin(int32_t jpOnlineBet);
 // 获取当前生效的难度配置快照
 void DLL_GetRtpDifficulty(RtpProfileConfig* outProfile);

 void DLL_GetUserDebugInfo(DebugInfo* pDebugInfo, GameInstanceId_t gameId);

 void DLL_GameClose(GameInstanceId_t gameId);


#endif // DllInterface_h__
