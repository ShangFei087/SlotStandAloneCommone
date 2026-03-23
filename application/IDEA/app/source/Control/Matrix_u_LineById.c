#include "Matrix_u_LineById.h"
#include "ComputerData.h"

// CMD_Fish.c 内部版本不对外声明，这里做一个等价初始化，避免改动 CMD_Fish.h。
static void CheckOnLineResult_InitLocal(CheckOnLineResult_t* pResult)
{
    if (pResult == NULL) return;

    memset(pResult, 0, sizeof(CheckOnLineResult_t));
    pResult->bIsEliminate = 0;
    pResult->bHasWild = 0;
    pResult->nAvailChessType = 0;
    pResult->nEliminateNum = 0;
    pResult->nEliminateLineId = 0;
    pResult->nWildNum = 0;
}

uint32_t Matrix_u_computeLineWinsById(Matrix_u* pMatrix, int32_t* idVec, SlotGameConfig_t* gameConfig)
{
    if (pMatrix == NULL || idVec == NULL || gameConfig == NULL) return 0;

    // 清空外部传入的 idVec（保持与原实现一致：只清理 lineCount 范围）
    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        idVec[i] = 0;
    }

    uint32_t nLocalWinBet = 0;
    CheckOnLineResult_t clr;
    uint8_t idVecCount = 0;

    // 按线逐条计算中奖
    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        CheckOnLineResult_InitLocal(&clr);
        Matrix_u_checkOnLine(pMatrix, i, &clr, gameConfig);

        if (clr.bIsEliminate)
        {
            nLocalWinBet += GET_BET_VALUE(gameConfig->header.id, clr.nAvailChessType, clr.nEliminateNum - 2);

            // ID 编码：千位=线号，百位=连中数量，十位/个位=图标类型
            int32_t nID = (int32_t)i * 1000 + (int32_t)clr.nEliminateNum * 100 + (int32_t)clr.nAvailChessType;

            pMatrix->resultType = RT_Win; // RT_Win
            idVec[idVecCount++] = nID;
        }
    }

    pMatrix->idVecSize = idVecCount;
    return nLocalWinBet;
}

