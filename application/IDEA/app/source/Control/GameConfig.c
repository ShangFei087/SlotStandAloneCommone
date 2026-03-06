// GameConfig.c
#include "GameConfig.h"
#include "ComputerData.h"

void GameConfig_Init(SlotGameConfig_t* slotConfig)
{
    if (slotConfig == NULL) return;
    memset(slotConfig, 0, sizeof(SlotGameConfig_t));
}

void GameConfig_Copy(SlotGameConfig_t* pDest, SlotGameConfig_t* pSrc)
{
    // 复制固定大小的结构部分
    memcpy(&pDest->header, &pSrc->header, sizeof(GameConfigHeader_t));
    memcpy(&pDest->paytable, &pSrc->paytable, sizeof(PaytableConfig_t));

}

