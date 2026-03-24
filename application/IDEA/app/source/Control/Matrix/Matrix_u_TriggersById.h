#ifndef _Matrix_u_TriggersById_H_
#define _Matrix_u_TriggersById_H_

#include "Control/CMD_Fish.h"

// 按线方式计算
uint32_t Matrix_u_computeLineWinsById(Matrix_u* pMatrix, int32_t* idVec, SlotGameConfig_t* gameConfig);
// gameId 相关触发判定：Scatter/Bonus/
void Matrix_u_applyTriggersByGameId(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet);

#endif // _Matrix_u_TriggersById_H_

