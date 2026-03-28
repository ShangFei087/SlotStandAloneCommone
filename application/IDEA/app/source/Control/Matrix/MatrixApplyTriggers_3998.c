#include "Matrix_u_TriggersById.h"

void MatrixApplyTriggers_3998(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet)
{
    if (pMatrix == NULL || gameConfig == NULL || pLocalWinBet == NULL) return;

    uint8_t _bonusCount = 0;
    uint8_t bounsOdds = 0; // 原始实现中恒为 0，这里保持一致

    // 判定 Bonus
    for (uint8_t i = 0; i < GE_WheelChessMaxNum; ++i)
    {
        if (pMatrix->dataArray[i] == gameConfig->header.Bonus)
        {
            ++_bonusCount;
        }
    }

    if (_bonusCount >= 3)
    {
        if (JRandFrom(0, 10000) < 2000)
        {
            pMatrix->resultType = RT_BonusWin;
        }
        else
        {
            pMatrix->resultType = RT_FreeWin;
        }

        *pLocalWinBet += bounsOdds;
    }
}

