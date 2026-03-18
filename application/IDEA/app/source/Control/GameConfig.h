// GameConfig.h
#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_

#include "qs.h"

#define PAY_WAY_LINE 0  //9 10 15 30 50   payway line
#define PAY_WAY_WAY 1 //243 1024 2048	  payway way

// 配置头结构，固定大小 64 字节
typedef struct {
	uint32_t	id;                 // 内部游戏 ID
	uint8_t		enabled;            // 是否启用
	uint8_t		gameMode;           // 游戏模式：0=普通，1=技巧，2=专家
	uint8_t		difficulty;         // 难度：0-8
	uint32_t	minBet;             // 最小押注
	uint32_t	maxBet;             // 最大押注
	uint8_t		ChessTypeNum;       // 总图标种类数
	uint8_t		ChessNorTypeNum;    // 普通图标种类数
	uint32_t	lineCount;          // 线数（如 15/20）或 ways（如 243）
	uint8_t		reelCount;          // 轴数（通常为 5）
	uint8_t		rowCount;           // 行数（通常为 3）
	uint32_t	MaxIDNyn;           // 中奖 ID 最大数量
	uint8_t		freeGameMax;        // 免费游戏最大次数
	uint8_t		Scatter;            // Scatter 图标
	uint8_t		Wild;               // Wild 图标
	uint8_t		Bonus;				// Bonus 图标
	int8_t		normalRollTableId;	// 普通卷轴表 ID
	int8_t		freeRollTableId;	// 免费卷轴表 ID
	uint8_t reserved[1];			// 预留字段
} GameConfigHeader_t;

typedef struct 
{
	//uint8_t* wildSymbol;                         // Wild 对应赔率
	//uint8_t*scatterSymbol;                       // Scatter 对应赔率（2~7 连）
	//uint8_t* bonusSymbol;                        // Bonus 对应赔率
	//uint8_t* freeGameTrigger;                    // 3~8 个 Scatter 对应免费次数
	//uint8_t *gLineCheckIDArray;				  // 每条线的检查位置索引

	//uint8_t  normalRollTableId;				  // 普通卷轴表
	//uint8_t  freeRollTableId;					  // 免费卷轴表
	//uint8_t  normalRollTableSizeId;			  // 普通卷轴长度表
	//uint8_t  freeRollTableSizeId;				  // 免费卷轴长度表
	uint8_t reserved[1];						  // 预留

} PaytableConfig_t;


// 老虎机完整配置结构
typedef struct {
    GameConfigHeader_t header;           // 配置头
	PaytableConfig_t   paytable;         // 赔率配置（扩展）
} SlotGameConfig_t;

//#pragma pack(pop)

// 配置标识常量
#define GAME_CONFIG_MAGIC   0x534C4F54  // "SLOT"
#define GAME_CONFIG_VERSION 0x00010001  // v1.0.1

// 配置结构初始化/拷贝
void GameConfig_Init(SlotGameConfig_t* slotConfig);  // 初始化
void GameConfig_Copy(SlotGameConfig_t* pDest, SlotGameConfig_t* pSrc);


#endif // _GAME_CONFIG_H_