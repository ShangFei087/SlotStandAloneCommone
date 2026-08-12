#include "Matrix_u_TriggersById.h"

// 当前默认逻辑：占位实现（按协议按需扩展）
void MatrixApplyTriggers_3995(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet, RoundInfo_t* info)
{
    (void)gameId;
    (void)pLocalWinBet;
    (void)info;
    if (pMatrix == NULL || gameConfig == NULL) return;

    uint8_t scatterCount = 0;
    uint8_t bonusCount = 0;
    for (uint8_t i = 0; i < gameConfig->header.wheelChessNum; ++i)
    {
        if (pMatrix->dataArray[i] == gameConfig->header.Scatter)
            ++scatterCount;
        if (pMatrix->dataArray[i] == gameConfig->header.Bonus)
            ++bonusCount;
    }
    // ≥3 Scatter → 转盘+免费
    if (scatterCount >= 3)
        pMatrix->resultType = RT_FreeWin;
    // ≥3 Bonus
    if (bonusCount >= 3)
    {
        if (info != NULL && info->nJPCount > 0)
        {
            pMatrix->resultType = RT_Jackpot;
        }
        else
        {
            pMatrix->resultType = RT_BonusWin;
        }
    }
      
}

uint32_t computeLineWildWins_3995(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, GameInstanceId_t gameId, int8_t* wildData)
{
    if (pMatrix == NULL || idVec == NULL || gameConfig == NULL || wildData == NULL) return 0;
    const MatrixTriggerOps_t* ops = MatrixTriggerOps_t_Get(gameId);
    if (ops == NULL || ops->checkOnLine == NULL) return 0;

    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
        idVec[i] = 0;

    uint32_t nLocalWinBet = 0;
    CheckOnLineResult_t clr;
    uint8_t idVecCount = 0;

    //  先算基础线奖（不乘 Wild）
    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        CheckOnLineResult_Init(&clr);
        ops->checkOnLine(pMatrix, i, &clr, gameConfig);
        if (!clr.bIsEliminate) continue;

        nLocalWinBet += GET_BET_VALUE(gameConfig->header.id, clr.nAvailChessType, clr.nEliminateNum - 2);

        uint32_t nID = ((uint32_t)i * 1000u) + ((uint32_t)clr.nEliminateNum * 100u) + (uint32_t)clr.nAvailChessType;
        pMatrix->resultType = RT_Win;
        idVec[idVecCount++] = (uint16_t)nID;
    }
    pMatrix->idVecSize = idVecCount;

    if (nLocalWinBet == 0) return 0;

    //  卷轴 2/3/4（列下标 1/2/3）上的每个 ×2/×3，对总奖金连乘
    uint32_t sumMul = 0;
    uint8_t n = gameConfig->header.wheelChessNum;
    for (uint8_t i = 0; i < n; ++i)
    {
        uint8_t col = (uint8_t)(i % COL_MAX);
        if (col < 1 || col > 3) continue;
        if (pMatrix->dataArray[i] != gameConfig->header.Wild) continue;
        if (wildData[i] <= 1) continue;

        sumMul += (uint32_t)wildData[i];   // ×2、×3 先相加
    }

    if (sumMul == 0)
        return nLocalWinBet;               // 没有倍数 Wild，不乘

    return nLocalWinBet * sumMul;
}

