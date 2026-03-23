#ifndef _Matrix_u_TriggersById_H_
#define _Matrix_u_TriggersById_H_

#include "CMD_Fish.h"

// gameId 相关触发判定：Scatter/Bonus/3998 Bonus-Free，并在需要时覆盖 pMatrix->resultType。
void Matrix_u_applyTriggersByGameId(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet);

#endif // _Matrix_u_TriggersById_H_

