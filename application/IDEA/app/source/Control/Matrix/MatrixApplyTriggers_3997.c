#include "Matrix_u_TriggersById.h"

void MatrixApplyTriggers_3997(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet)
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