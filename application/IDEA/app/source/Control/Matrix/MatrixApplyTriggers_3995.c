#include "Matrix_u_TriggersById.h"

// 当前与默认逻辑一致，但保留为独立实现点。
void MatrixApplyTriggers_3995(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet)
{
}

uint32_t computeLineWildWins_3995(Matrix_u* pMatrix, int32_t* idVec, SlotGameConfig_t* gameConfig, GameInstanceId_t gameId, int8_t* wildData)
{
    if (pMatrix == NULL || idVec == NULL || gameConfig == NULL) return 0;
    const MatrixTriggerOps_t* ops = MatrixTriggerOps_t_Get(gameId);
    if (ops == NULL || ops->checkOnLine == NULL)
    {
        return 0;
    }

    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        idVec[i] = 0;
    }

    uint32_t nLocalWinBet = 0;
    CheckOnLineResult_t clr;
    uint8_t idVecCount = 0;
    int8_t wildMultiplied = 0;//wild倍数
    // 按线逐条计算中奖
    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        wildMultiplied = 0;
        CheckOnLineResult_Init(&clr);
        ops->checkOnLine(pMatrix, i, &clr, gameConfig);

        if (clr.bIsEliminate)
        {
            if (clr.bHasWild)
            {
                for (uint8_t k = 0; k < gameConfig->header.colCount; ++k)
                {
                    int Pos = clr.posVec[k];
                    if (pMatrix->dataArray[Pos] == gameConfig->header.Wild)
                    {
                        wildMultiplied += wildData[Pos];
                    }
                }
            }

            if (wildMultiplied > 0)
            {
                nLocalWinBet += GET_BET_VALUE(gameConfig->header.id, clr.nAvailChessType, clr.nEliminateNum - 2) * wildMultiplied;
            }
            else
            {
                nLocalWinBet += GET_BET_VALUE(gameConfig->header.id, clr.nAvailChessType, clr.nEliminateNum - 2);
            }


            // ID 编码：千位=线号，百位=连中数量，十位/个位=图标类型
            int32_t nID = (int32_t)i * 1000 + (int32_t)clr.nEliminateNum * 100 + (int32_t)clr.nAvailChessType;

            pMatrix->resultType = RT_Win; // RT_Win
            idVec[idVecCount++] = nID;
        }
    }

    pMatrix->idVecSize = idVecCount;
    return nLocalWinBet;
}

