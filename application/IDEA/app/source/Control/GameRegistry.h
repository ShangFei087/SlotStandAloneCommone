#ifndef _GAME_REGISTRY_H_
#define _GAME_REGISTRY_H_

#include "GameManager.h"
#include "DllInterface.h"

// 统一初始化：重置管理器、初始化奖池、注册默认游戏配置
// 返回 1 表示成功；返回 0 表示任一环节失败。
int8_t GameRegistry_InitAndRegisterDefaults();

#endif // _GAME_REGISTRY_H_

