#include "GameRegistry.h"


#include "GameManager.h"
#include "LotteryManager.h"
#include "GameConfig.h"
#include "GenerationResult/GameResultRegistry.h"
#include "Matrix/Matrix_u_TriggersById.h"


int8_t GameRegistry_InitAndRegisterDefaults()
{
    int32_t baseValueArray[3] = { 3000, 1500, 500 };
    int32_t maxValueArray[3] = { JPWeight[1], JPWeight[2], JPWeight[3] };

    // 每次初始化都从干净状态开始，避免热重载后残留旧实例。
    //GameManager_Init();

    // 奖池基础参数集中在此处，统一主程序与协议初始化行为。
    LotteryManager_Init();
    LotteryManager_SetBaseValue(&gLotteryManager, baseValueArray, maxValueArray);

    //初始化内置游戏配置
    GameConfigRegistry_InitDefaults();
    GameResultRegistry_InitDefaults();
    MatrixTriggerOps_t_InitDefaults();
    return 1;
}
