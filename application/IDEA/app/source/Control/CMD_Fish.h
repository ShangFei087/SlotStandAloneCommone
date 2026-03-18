#ifndef CMD_FISH_H_
#define CMD_FISH_H_

#include <string.h>
#include "qs.h"
#include "GameConfig.h"

//--------------------------------------------全局常量（3款游戏通用）-------------------------------------------//
// 调试模式枚举
#define DCM_Normal 0
#define DCM_PointResData 1

// 全局参数常量
#define GE_WheelChessNum 15
#define GE_MaxIDNum 20
#define GE_Line20Num  20  // 20 线
#define GE_Line15Num  15  // 15 线
#define GE_MaxFreeNum  20


// 结果类型定义
#define RT_Lose 0
#define RT_Win 1
#define RT_FreeWin 2
#define RT_BonusWin 3
//#define RT_Jackpot  4 彩金中奖应该独立出来

// 开奖方式定义
#define OT_Normal 0
#define OT_Give   1

// 彩金类型定义
#define JT_None 0
#define JT_Major 1
#define JT_Minor 2
#define JT_Mini 3

//-------------------------------------------------------------------------------------

// 单条线检查结果
typedef struct {
	uint8_t bIsEliminate;            // 是否消除（是否中奖）
	uint8_t bHasWild;                // 是否包含 Wild
	uint8_t chessTypeVec[5];         // 该线 5 个位置的图标类型
	uint8_t posVec[5];               // 该线 5 个位置的索引
	uint8_t nWildNum;                // Wild 个数
	uint8_t nAvailChessType;         // 有效中奖图标类型
	uint8_t nEliminateNum;           // 连中的图标个数
	uint8_t nEliminateLineId;        // 中奖线编号
} CheckOnLineResult_t;

// 初始化 CheckOnLineResult_t
void CheckOnLineResult_Init(CheckOnLineResult_t* pResult);

//-------------------------------------------------------------------------------------
// 矩阵结构
typedef struct {
	uint8_t resultType;
	uint8_t idVecSize;
	uint8_t dataArray[GE_WheelChessNum];
} Matrix_u;

// Matrix_u 相关接口
uint8_t Matrix_u_getIntData(Matrix_u* pMatrix, uint8_t pos);
void Matrix_u_setIntData(Matrix_u* pMatrix, uint8_t* pData);
void Matrix_u_print(Matrix_u* pMatrix);
uint8_t Matrix_u_getTypeNum(Matrix_u* pMatrix, uint8_t type);
void Matrix_u_reset(Matrix_u* pMatrix);
void Matrix_u_copy(Matrix_u* pDest, Matrix_u* pSrc);

void Matrix_u_insertScatter(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_insertCol(Matrix_u* pMatrix, uint8_t type, uint8_t col);// 在指定列插入同一种图标
void Matrix_u_insertBonus(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_GenerateAnyResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus);
void Matrix_u_GenerateLooseResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus, uint8_t hasBoost);

uint8_t Matrix_u_computerMatrix_243(Matrix_u* pMatri, int32_t* idVec); // 243 方式计算
int32_t Matrix_u_computerMatrixById(Matrix_u* pMatrix, int32_t* idVec,SlotGameConfig_t* gameConfig, uint32_t gameId);// 按线方式计算
uint8_t Matrix_u_checkOnLine(Matrix_u* pMatrix, uint8_t lineIdx, CheckOnLineResult_t* pClr, SlotGameConfig_t* gameConfig);

// 开奖输出结构
typedef struct {
	uint8_t  openType;                      // 开奖类型：普通/赠送
	uint8_t  resType;                       // 结果类型
	uint8_t  JPType;                        // 彩金类型
	int32_t  IDVec[GE_MaxIDNum];            // 中奖线 ID 信息（最多 20 条）
	int32_t  nMatrixBet;                    // 线奖总倍数
	Matrix_u matrix;                        // 开奖矩阵

	/* 当结果类型为 RT_FreeWin 时，以下字段有效 */
	int32_t nTotalFreeBet;                  // 预计免费游戏总倍数
	uint8_t nTotalFreeTime;                 // 免费游戏总次数
	int32_t FreeBetArray[GE_MaxFreeNum];

	//uint8_t uint16_t
	/* 当结果类型为 RT_BonusWin 时，以下字段有效 */
	int32_t nBonusBet;
	uint8_t BlindSymbol;                   // 神秘图标或乘数值（按玩法解释）
	int32_t BonusData[GE_WheelChessNum];   // Bonus 结果数据
	uint8_t nBonusType;
	
	int32_t nJPBet;
} OutResult_t;

// OutResult_t 相关接口
void OutResult_Init(OutResult_t* pResult);
void OutResult_reset(OutResult_t* pResult);

// 调试控制结构
typedef struct {
	uint8_t mode;       // 调试模式
	uint8_t resType;    // 指定结果类型
	uint8_t bonusType;  // 指定 Bonus 类型
	uint8_t jpType;     // 指定彩金类型
} DebugControlMode_t;

void DebugControlMode_Init(DebugControlMode_t* pMode);
void  DLL_SetControlDebugMode(DebugControlMode_t* pDebugMode);
extern DebugControlMode_t gDebugControlMode;


// 调试统计信息
typedef struct {
	int64_t dwPlayScore;          // 总下注
	int64_t dwWinScore;           // 总赢钱
	int64_t dwTotalPlayTime;      // 总局数
	int64_t dwNormalOpenTime;     // 普通开局次数
	int64_t dwGiveOpenTime;       // 赠送开局次数
	int64_t dwNormalWinTime;      // 普通中奖次数
	int64_t dwBonusTime;          // Bonus 次数
	int64_t dwFreeGameTime;       // 免费游戏触发次数
	int64_t dwLooseTime;          // 输局次数
	int64_t dwJackpotTime;        // Jackpot 次数
	int64_t dwNormalWinTotalBet;  // 普通中奖总倍数
	int64_t dwFreeGameTotalBet;   // 免费游戏总倍数
	int64_t dwBonusGameTotalBet;  // Bonus 游戏总倍数
	int64_t dwJackpotTotalBet;    // Jackpot 总赢钱
	int64_t dwFreeGameBetError;   // 免费游戏剩余倍数误差
} DebugInfo;

void DebugInfo_reset(DebugInfo* pInfo);
void DebugInfo_accum(DebugInfo* pDest, DebugInfo* pSrc);
int8_t* DebugInfo_toJason(DebugInfo* pInfo);
// DebugInfo_toJason 返回堆内存字符串，调用方必须 free

// 全局变量声明


#endif // CMD_FISH_H_
