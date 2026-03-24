#include "Matrix_u_TriggersById.h"

#include "Control/ComputerData.h"

uint8_t Matrix_u_checkOnLine(Matrix_u* matrix, uint8_t lineIdx, CheckOnLineResult_t* clr, SlotGameConfig_t* gameConfig) {
    uint8_t nCheckIndex = 0;            // 当前检查下标（保留字段）
    int8_t nFirstAvailChessIndex = -1;  // 第一枚有效普通图标的位置（非 Wild/Bonus/Scatter）
    uint8_t nFirstChessType = 0;        // 第一枚图标类型（保留字段）
    uint8_t nFirstAvailChessType = 0;   // 第一枚有效普通图标类型
    uint8_t nIdPos = 0;

    clr->bIsEliminate = 0;
    clr->bHasWild = 0;

    uint8_t nConWildNum = 0;  // 从左开始连续 Wild 数量
    uint8_t nWildIdx = 0;
    uint8_t BetValue = 0;

    for (uint8_t idIndex = 0; idIndex < gameConfig->header.reelCount; ++idIndex)
    {
        nIdPos = GET_LINE_VALUE(gameConfig->header.id, lineIdx, idIndex);
        //nIdPos = gameConfig->paytable.gLineCheckIDArray[lineIdx * gameConfig->header.reelCount + idIndex];
        clr->posVec[idIndex] = nIdPos;
        clr->chessTypeVec[idIndex] = matrix->dataArray[nIdPos];

        if (matrix->dataArray[nIdPos] == gameConfig->header.Wild)
        {
            if (idIndex == nWildIdx)
            {
                nConWildNum++;
                nWildIdx++;
            }
        }
        else {
            // 找到第一枚普通图标
            if (nFirstAvailChessIndex == -1)
            {
                nFirstAvailChessIndex = nIdPos;
                nFirstAvailChessType = matrix->dataArray[nFirstAvailChessIndex];
            }
        }
    }


    // 5 连 Wild 的特殊处理
    if (nConWildNum == 5)
    {
        clr->nAvailChessType = gameConfig->header.Wild;
        clr->nEliminateLineId = lineIdx;
        BetValue = GET_BET_VALUE(gameConfig->header.id, gameConfig->header.Wild, 3 - 2);
        if (BetValue > 0)
        {
            clr->nEliminateNum = 5;
            clr->bHasWild = 1;
            clr->bIsEliminate = 1;
            return 1;
        }
        else {
            return 0;
        }
    }

    // 未找到普通图标，说明该线仅由 Wild/Bonus/Scatter 组成
    if (nFirstAvailChessIndex == -1)
    {
        return 0;
    }

    // 第一枚有效图标是 Bonus/Scatter，不参与线奖判定
    if (nFirstAvailChessType == gameConfig->header.Scatter || nFirstAvailChessType == gameConfig->header.Bonus)
    {
        // 仅处理前缀 Wild 的情况
        if (nConWildNum >= 3)
        {
            clr->nAvailChessType = gameConfig->header.Wild;
            clr->nEliminateLineId = lineIdx;
            clr->nEliminateNum = nConWildNum;
            BetValue = GET_BET_VALUE(gameConfig->header.id, gameConfig->header.Wild, nConWildNum - 2);
            if (BetValue > 0)
            {
                clr->bHasWild = 1;
                clr->bIsEliminate = 1;
                return 1;
            }
            else
            {
                return 0;
            }
        }
        return 0;
    }

    // 统计从左开始与首个有效图标（或 Wild）连续匹配的数量
    int8_t j = 1;
    for (j = 1; j < 5; j++)
    {
        if (clr->chessTypeVec[j] == gameConfig->header.Scatter || clr->chessTypeVec[j] == gameConfig->header.Bonus)  break;
        if (clr->chessTypeVec[j] == nFirstAvailChessType || clr->chessTypeVec[j] == gameConfig->header.Wild)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    // 至少连中 2 个才有机会构成线奖
    if (j >= 3)
    {
        BetValue = GET_BET_VALUE(gameConfig->header.id, clr->nAvailChessType, j - 2);
        if (BetValue > 0)
        {
            clr->bIsEliminate = 1;
        }
    }
    else if (j == 2)
    {
        BetValue = GET_BET_VALUE(gameConfig->header.id, nFirstAvailChessType, 0);
        if (BetValue != 0)
        {
            clr->bIsEliminate = 1;
        }
        else
        {
            clr->bIsEliminate = 0;
            return 0;
        }
    }
    else
    {
        clr->bIsEliminate = 0;
        return 0;
    }

    clr->nAvailChessType = nFirstAvailChessType;
    clr->nEliminateLineId = lineIdx;
    clr->nEliminateNum = j;

    // Wild 及其右侧特殊图标不单独作为普通线奖
    if (nFirstAvailChessType >= gameConfig->header.Wild && nConWildNum < 3)
    {
        clr->bIsEliminate = 0;
    }

    clr->nWildNum = 0;
    if (clr->bIsEliminate)
    {
        if (nConWildNum < 3)
        {
            for (j = 0; j < clr->nEliminateNum; ++j)
            {
                if (clr->chessTypeVec[j] == gameConfig->header.Wild)
                {
                    clr->bHasWild = 1;
                    clr->nWildNum++;
                }
            }
        }
        else
        {
            // 3 连及以上 Wild 时，比较“按普通图标赔付”和“按 Wild 赔付”，取更优
            int8_t nNormalBet = GET_BET_VALUE(gameConfig->header.id, clr->nAvailChessType, clr->nEliminateNum - 2);
            int8_t nWildBet = GET_BET_VALUE(gameConfig->header.id, gameConfig->header.Wild, nConWildNum - 2);

            if (nNormalBet >= nWildBet)
            {
                clr->bHasWild = 1;
            }
            else
            {
                clr->nAvailChessType = gameConfig->header.Wild;
                clr->nEliminateLineId = lineIdx;
                clr->nEliminateNum = nConWildNum;
                clr->bHasWild = 1;
                clr->bIsEliminate = 1;
                return 1;
            }
        }
    }

    return clr->bIsEliminate;
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
        CheckOnLineResult_Init(&clr);
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

typedef void (*MatrixApplyTriggersFn)(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig,uint32_t gameId, uint32_t* pLocalWinBet);

typedef struct
{
    MatrixApplyTriggersFn applyTriggers;
} MatrixTriggerOps_t;

// Per-game apply triggers implementations (one file per game).
void MatrixApplyTriggers_1700(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet);
void MatrixApplyTriggers_3999(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet);
void MatrixApplyTriggers_3998(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet);

static MatrixTriggerOps_t gOps_1700;
static MatrixTriggerOps_t gOps_3999;
static MatrixTriggerOps_t gOps_3998;
static MatrixTriggerOps_t gOps_Default;
static uint8_t gTriggerRegistryInited = 0;

static void MatrixApplyTriggers_Default(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet)
{
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

static void MatrixTriggerRegistry_InitDefaults(void)
{
    gOps_1700.applyTriggers = MatrixApplyTriggers_Default;
    gOps_3999.applyTriggers = MatrixApplyTriggers_Default;
    gOps_3998.applyTriggers = MatrixApplyTriggers_3998;
    gOps_Default.applyTriggers = MatrixApplyTriggers_Default;
    gTriggerRegistryInited = 1;
}

static const MatrixTriggerOps_t* MatrixTriggerRegistry_Get(uint32_t gameId)
{
    if (gameId == 1700) return &gOps_1700;
    if (gameId == 3999) return &gOps_3999;
    if (gameId == 3998) return &gOps_3998;
    return &gOps_Default;
}

void Matrix_u_applyTriggersByGameId(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet)
{
    if (pMatrix == NULL || gameConfig == NULL || pLocalWinBet == NULL) return;

    if (!gTriggerRegistryInited)
    {
        MatrixTriggerRegistry_InitDefaults();
    }

    const MatrixTriggerOps_t* ops = MatrixTriggerRegistry_Get(gameId);
    if (ops == NULL || ops->applyTriggers == NULL) return;
    ops->applyTriggers(pMatrix, gameConfig, gameId, pLocalWinBet);
}

