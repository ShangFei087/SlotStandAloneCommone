#include "Control/CMD_Fish.h"

// 3999 触发判定：当前与默认 scatter+bonus 逻辑一致，但保留为独立实现点。
void MatrixApplyTriggers_3999(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet)
{
    (void)gameId;

    if (pMatrix == NULL || gameConfig == NULL || pLocalWinBet == NULL) return;

    uint8_t scatterCount = 0;
    uint8_t _bonusCount = 0;
    uint8_t bounsOdds = 0; // 原始实现中恒为 0，这里保持一致

    // Scatter -> Free
    for (uint8_t i = 0; i < gameConfig->header.lineCount * gameConfig->header.rowCount; ++i)
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

    // Bonus -> BonusWin
    for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
    {
        if (pMatrix->dataArray[i] == gameConfig->header.Bonus)
        {
            ++_bonusCount;
        }
    }
    if (_bonusCount >= 3)
    {
        pMatrix->resultType = RT_BonusWin;
        *pLocalWinBet += bounsOdds;
    }
}

