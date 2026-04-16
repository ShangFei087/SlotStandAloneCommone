#include "Matrix_u_TriggersById.h"

#include "Control/ComputerData.h"

//--------------------------------------------外部调用--------------------------------------------//
int32_t computerMatrixById(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, uint32_t gameId, RoundInfo_t* info)
{
    uint32_t nLocalWinBet = 0;
    MatrixTriggerOps_t* ops = MatrixTriggerOps_t_Get(gameId);
    if (ops == NULL || ops->applyTriggers == NULL|| ops->computeLineWin == NULL)
    {
        return 0;
    }

    nLocalWinBet= ops->computeLineWin(pMatrix, idVec, gameConfig, gameId);
    ops->applyTriggers(pMatrix, gameConfig, gameId, &nLocalWinBet, info);
    return nLocalWinBet;
}
//--------------------------------------------公共--------------------------------------------//
uint8_t checkOnLine_Defaults(Matrix_u* matrix, uint8_t lineIdx, CheckOnLineResult_t* clr, SlotGameConfig_t* gameConfig) {
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

    for (uint8_t idIndex = 0; idIndex < gameConfig->header.colCount; ++idIndex)
    {
        nIdPos = GET_LINE_VALUE(gameConfig->header.id, lineIdx, idIndex);
        //nIdPos = gameConfig->paytable.gLineCheckIDArray[lineIdx * gameConfig->header.colCount + idIndex];
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
uint32_t computeLineWins_Defaults(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, GameInstanceId_t gameId)
{
    if (pMatrix == NULL || idVec == NULL || gameConfig == NULL) return 0;
    MatrixTriggerOps_t* ops = MatrixTriggerOps_t_Get(gameId);
    if (ops == NULL || ops->checkOnLine== NULL)
    {
        return;
    }

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
        ops->checkOnLine(pMatrix, i, &clr, gameConfig);

        if (clr.bIsEliminate)
        {
            nLocalWinBet += GET_BET_VALUE(gameConfig->header.id, clr.nAvailChessType, clr.nEliminateNum - 2);

            // ID 编码：千位=线号，百位=连中数量，十位/个位=图标类型
            uint32_t nID = ((uint32_t)i * 1000u) + ((uint32_t)clr.nEliminateNum * 100u) + (uint32_t)clr.nAvailChessType;

            pMatrix->resultType = RT_Win; // RT_Win
            idVec[idVecCount++] = (uint16_t)nID;
        }
    }

    pMatrix->idVecSize = idVecCount;
    return nLocalWinBet;
}
void MatrixApplyTriggers_Default(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet, RoundInfo_t* info)
{
    (void)info;
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
    if (_bonusCount >= 3)
    {
        pMatrix->resultType = RT_BonusWin;
    }
}
//--------------------------------------------解耦--------------------------------------------//
void MatrixApplyTriggers_1700(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet, RoundInfo_t* info);
//--------------------------------------------财富类--------------------------------------------//
void MatrixApplyTriggers_3999(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet, RoundInfo_t* info);
void MatrixApplyTriggers_3998(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet, RoundInfo_t* info);
void MatrixApplyTriggers_3997(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet, RoundInfo_t* info);
void MatrixApplyTriggers_3996(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet, RoundInfo_t* info);
//--------------------------------------------动物类--------------------------------------------//
void MatrixApplyTriggers_3993(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet, RoundInfo_t* info);
void MatrixApplyTriggers_3995(Matrix_u* pMatrix, SlotGameConfig_t* gameConfig, uint32_t gameId, uint32_t* pLocalWinBet, RoundInfo_t* info);
typedef struct
{
    GameInstanceId_t gameId;
    MatrixTriggerOps_t ops;
} MatrixTriggerOps_tEntry_t;

static MatrixTriggerOps_tEntry_t gReg[GAME_INSTANCE_ID_MAX];
static int32_t gRegCount = 0;

int8_t MatrixTrigger_Register(GameInstanceId_t gameId, const MatrixTriggerOps_t* ops)
{
    if (ops == NULL) return 0;
    if (gameId == GAME_ID_INVALID) return 0;

    // 已存在则覆盖
    for (int32_t i = 0; i < gRegCount; ++i)
    {
        if (gReg[i].gameId == gameId)
        {
            gReg[i].ops = *ops;
            return 1;
        }
    }

    if (gRegCount > GAME_INSTANCE_ID_MAX) return 0;

    gReg[gRegCount].gameId = gameId;
    gReg[gRegCount].ops = *ops;
    gRegCount++;
    return 1;
}

const MatrixTriggerOps_t* MatrixTriggerOps_t_Get(GameInstanceId_t gameId)
{
    if (gameId == GAME_ID_INVALID) return NULL;
    for (int32_t i = 0; i < gRegCount; ++i)
    {
        if (gReg[i].gameId == gameId)
        {
            return &gReg[i].ops;
        }
    }
    return NULL;
}

void MatrixTriggerOps_t_InitDefaults(void)
{
    memset(gReg, 0, sizeof(gReg));
    gRegCount = 0;

    MatrixTriggerOps_t ops1700 = { 0 };
    ops1700.applyTriggers = MatrixApplyTriggers_Default;
    ops1700.checkOnLine = checkOnLine_Defaults;
    ops1700.computeLineWin = computeLineWins_Defaults;
    (void)MatrixTrigger_Register(1700, &ops1700);

    MatrixTriggerOps_t ops3999 = { 0 };
    ops3999.applyTriggers = MatrixApplyTriggers_Default;
    ops3999.checkOnLine = checkOnLine_Defaults;
    ops3999.computeLineWin = computeLineWins_Defaults;
    (void)MatrixTrigger_Register(3999, &ops3999);

    MatrixTriggerOps_t ops3998 = { 0 };
    ops3998.applyTriggers = MatrixApplyTriggers_3998;
    ops3998.checkOnLine = checkOnLine_Defaults;
    ops3998.computeLineWin = computeLineWins_Defaults;
    (void)MatrixTrigger_Register(3998, &ops3998);

    MatrixTriggerOps_t ops3993 = { 0 };
    ops3993.applyTriggers = MatrixApplyTriggers_3993;
    ops3993.checkOnLine = checkOnLine_Defaults;
    ops3993.computeLineWin = computeLineWins_Defaults;
    (void)MatrixTrigger_Register(3993, &ops3993);

    MatrixTriggerOps_t ops3995 = { 0 };
    ops3995.applyTriggers = MatrixApplyTriggers_Default;
    ops3995.checkOnLine = checkOnLine_Defaults;
    ops3995.computeLineWin = computeLineWins_Defaults;
    (void)MatrixTrigger_Register(3995, &ops3995);

    MatrixTriggerOps_t ops3997 = { 0 };
    ops3997.applyTriggers = MatrixApplyTriggers_3997;
    ops3997.checkOnLine = checkOnLine_Defaults;
    ops3997.computeLineWin = computeLineWins_Defaults;
    (void)MatrixTrigger_Register(3997, &ops3997);

    MatrixTriggerOps_t ops3996 = { 0 };
    ops3996.applyTriggers = MatrixApplyTriggers_3996;
    ops3996.checkOnLine = checkOnLine_Defaults;
    ops3996.computeLineWin = computeLineWins_Defaults;
    (void)MatrixTrigger_Register(3996, &ops3996);
}


