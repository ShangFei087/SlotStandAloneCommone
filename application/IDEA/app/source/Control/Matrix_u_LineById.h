#ifndef _Matrix_u_LineById_H_
#define _Matrix_u_LineById_H_

#include "CMD_Fish.h"


// 按线计算：填充 idVec、设置 pMatrix->idVecSize，并返回线奖倍数。
uint32_t Matrix_u_computeLineWinsById(Matrix_u* pMatrix, int32_t* idVec, SlotGameConfig_t* gameConfig);

#endif // _Matrix_u_LineById_H_

