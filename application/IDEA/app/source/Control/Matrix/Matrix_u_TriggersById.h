#ifndef _Matrix_u_TriggersById_H_
#define _Matrix_u_TriggersById_H_

#include "Control/CommonStruct.h"
#include "Control/GameManager.h"

// 按线方式计算
int32_t computerMatrixById(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, uint32_t gameId);

uint8_t  checkOnLine_Defaults(Matrix_u* matrix, uint8_t lineIdx, CheckOnLineResult_t* clr, SlotGameConfig_t* gameConfig);
uint32_t computeLineWins_Defaults(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, GameInstanceId_t gameId);
void MatrixApplyTriggers_Default(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet);


typedef void (*MatrixApplyTriggersFn)(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet);
typedef void (*MatrixcheckOnLineWinsFn)(Matrix_u* matrix, uint8_t lineIdx, CheckOnLineResult_t* clr, SlotGameConfig_t* gameConfig);
typedef uint32_t(*MatrixcomputeLineWinsFn)(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, GameInstanceId_t gameId);
//3993免费奖特殊玩法
uint32_t computeLineFreeWins_3993(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, GameInstanceId_t gameId, int8_t* wildData);
//3995普通游戏wild玩法
uint32_t computeLineWildWins_3995(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, GameInstanceId_t gameId, int8_t* wildData);
//3996免费奖特殊玩法
uint32_t computeLineFreeWins_3996(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, GameInstanceId_t gameId, int8_t* wildData);
typedef struct
{
    MatrixApplyTriggersFn applyTriggers;
    MatrixcheckOnLineWinsFn checkOnLine;
    MatrixcomputeLineWinsFn  computeLineWin;
} MatrixTriggerOps_t;

int8_t MatrixTrigger_Register(GameInstanceId_t gameId, const MatrixTriggerOps_t* ops);
const MatrixTriggerOps_t* MatrixTriggerOps_t_Get(GameInstanceId_t gameId);
void MatrixTriggerOps_t_InitDefaults(void);

#endif // _Matrix_u_TriggersById_H_

