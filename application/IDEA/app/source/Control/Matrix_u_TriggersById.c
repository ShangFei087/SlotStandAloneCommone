#include "Matrix_u_TriggersById.h"

typedef void (*MatrixApplyTriggersFn)
(
    Matrix_u* pMatrix,
    SlotGameConfig_t* gameConfig,
    uint32_t gameId,
    uint32_t* pLocalWinBet
);

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
    (void)gameId;

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

