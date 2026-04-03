#include "Matrix_u_TriggersById.h"

void MatrixApplyTriggers_3996(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet)
{
    uint8_t scatterCount = 0;
    uint8_t _bonusCount = 0;

    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        if (pMatrix->dataArray[i] == gameConfig->header.Scatter)
        {
            ++scatterCount;
        }
    }
    if (scatterCount >= 3)
    {
        pMatrix->resultType = RT_FreeWin;
    }

    for (uint8_t i = 0; i < gameConfig->header.wheelChessNum; ++i)
    {
        if (pMatrix->dataArray[i] == gameConfig->header.Bonus)
        {
            ++_bonusCount;
        }
    }
    if (_bonusCount >= 6)
    {
        pMatrix->resultType = RT_BonusWin;
    }
}

uint32_t computeLineFreeWins_3996(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, GameInstanceId_t gameId, int8_t* wildData)
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
    int8_t wildMultiplied = 0; // Wild 倍数
    // 统计连线并计算本地赢注
    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        wildMultiplied = 0;
        CheckOnLineResult_Init(&clr);
        ops->checkOnLine(pMatrix, i, &clr, gameConfig);

        if (clr.bIsEliminate)
        {
            if (clr.bHasWild)
            {
                for (uint8_t k = 0; k < 5; ++k)
                {
                    int Pos = clr.posVec[k];
                    if (pMatrix->dataArray[Pos] == gameConfig->header.Wild)
                    {
                        wildMultiplied += wildData[Pos];
                    }
                }
                nLocalWinBet += GET_BET_VALUE(gameConfig->header.id, clr.nAvailChessType, clr.nEliminateNum - 2) * wildMultiplied;
            }
            else
            {
                nLocalWinBet += GET_BET_VALUE(gameConfig->header.id, clr.nAvailChessType, clr.nEliminateNum - 2);
            }


            // ID 编码：千位=线编号(i)，百位=消除数量(clr.nEliminateNum)，个位=可用棋子类型(clr.nAvailChessType)
            uint32_t nID = ((uint32_t)i * 1000u) + ((uint32_t)clr.nEliminateNum * 100u) + (uint32_t)clr.nAvailChessType;

            pMatrix->resultType = RT_Win; // RT_Win
            idVec[idVecCount++] = (uint16_t)nID;
        }
    }

    pMatrix->idVecSize = idVecCount;
    return nLocalWinBet;
}
