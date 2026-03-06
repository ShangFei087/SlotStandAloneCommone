// GameConfig.h
#ifndef _GAME_CONFIG_H_
#define _GAME_CONFIG_H_

#include "qs.h"

#define PAY_WAY_LINE 0  //9 10 15 30 50   payway line
#define PAY_WAY_WAY 1 //243 1024 2048	  payway way

// 配置头结构（固定大小：64字节）
typedef struct {
	uint32_t	id;                 //内部id
	uint8_t		enabled;            // 是否启用
	uint8_t		gameMode;           // 游戏模式：0=普通，1=技巧，2=专家
	uint8_t		difficulty;         // 难度：0-8
	uint32_t	minBet;             // 最小押注
	uint32_t	maxBet;             // 最大押注
	uint8_t		ChessTypeNum;       //总计图标个数
	uint8_t		ChessNorTypeNum;    //普通图标个数
	uint32_t	lineCount;          // 线数：15或243
	uint8_t		reelCount;          // 列数：5
	uint8_t		rowCount;           // 行数：3
	uint32_t	MaxIDNyn;           //最大可以消除线数
	uint8_t		freeGameMax;        // 最大免费游戏次数
	uint8_t		Scatter;            // 免费图标
	uint8_t		Wild;               //Wild图标
	uint8_t		Bonus;				//大奖图标
	int8_t		normalRollTableId;	//普通滚轮表id
	int8_t		freeRollTableId;	//免费滚轮表id
	uint8_t reserved[1];			// 保留字段
} GameConfigHeader_t;


typedef struct 
{
	//uint8_t* wildSymbol;                         // Wild符号赔率
	//uint8_t*scatterSymbol;                      // Scatter符号赔率（2-7个）
	//uint8_t* bonusSymbol;                        // Bonus符号赔率
	//uint8_t* freeGameTrigger;                    // 对应3,4,5,6,7,8个Scatter的免费次数
	//uint8_t *gLineCheckIDArray;					//每一条线的检测ID索引 

	//uint8_t  normalRollTableId;					// 每个滚轮数据大小
	//uint8_t  freeRollTableId;					// 每个滚轮数据大小
	//uint8_t  normalRollTableSizeId;				// 每个滚轮数据大小
	//uint8_t  freeRollTableSizeId;					// 每个滚轮数据大小
	uint8_t reserved[1];						// 保留

} PaytableConfig_t;


// 游戏全局配置（完整结构）
typedef struct {
    GameConfigHeader_t header;           // 配置头
	PaytableConfig_t   paytable;         // 赔率表
} SlotGameConfig_t;

//#pragma pack(pop)

// 配置管理函数
#define GAME_CONFIG_MAGIC   0x534C4F54  // "SLOT"
#define GAME_CONFIG_VERSION 0x00010001  // v1.0.1

// 配置创建和初始化
void GameConfig_Init(SlotGameConfig_t* slotConfig);  //初始化
void GameConfig_Copy(SlotGameConfig_t* pDest, SlotGameConfig_t* pSrc);


#endif // _GAME_CONFIG_H_