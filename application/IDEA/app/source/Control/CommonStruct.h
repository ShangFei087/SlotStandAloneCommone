#ifndef _CommonStruct_H_
#define _CommonStruct_H_

#include "ComputerData.h"
#include "GameConfig.h"
//-------------------------------------------------------------------------------------
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
	uint8_t dataArray[GE_WheelChessMaxNum];
} Matrix_u;

// Matrix_u 相关接口
uint8_t Matrix_u_getIntData(Matrix_u* pMatrix, SlotGameConfig_t slotgameconfig, uint8_t pos);
void Matrix_u_setIntData(Matrix_u* pMatrix, SlotGameConfig_t slotgameconfig, uint8_t* pData);
uint8_t Matrix_u_getTypeNum(Matrix_u* pMatrix, SlotGameConfig_t slotgameconfig, uint8_t type);
void Matrix_u_reset(Matrix_u* pMatrix);
void Matrix_u_copy(Matrix_u* pDest, Matrix_u* pSrc);

void Matrix_u_insertCol(Matrix_u* pMatrix, uint8_t type, uint8_t col);// 在指定列插入同一种图标
//调试模式强行插入图标
void Matrix_u_insertScatter(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_insertBonus(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_GenerateAnyResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus);
void Matrix_u_GenerateLooseResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus, uint8_t hasBoost);

uint8_t Matrix_u_computerMatrix_243(Matrix_u* pMatri, int32_t* idVec); // 243 方式计算
int32_t Matrix_u_computerMatrixById(Matrix_u* pMatrix, int32_t* idVec, SlotGameConfig_t* gameConfig, uint32_t gameId);// 按线方式计算

//-------------------------------------------------------------------------------------
// ComboResultInfo_t 结构体
typedef struct {
	int32_t IDVec[GE_MaxIDNum];
	int32_t nMatrixBet;  // 总倍率
	Matrix_u matrix; // matrix数据
} ComboResultInfo_t;
// ComboResultInfo_t 初始化函数
void ComboResultInfo_t_Init(ComboResultInfo_t* obj);
void ComboResultInfo_t_Reset(ComboResultInfo_t* obj);
//-------------------------------------------------------------------------------------
// 一轮结果结构体
typedef struct {
	int8_t resType;
	int32_t nMatrixBet;      // 阵列赔率
	int32_t nPlayBetValue;   // 压分

	Matrix_u extraMxu;   // 用于免费或大奖游戏阵列

	// 免费数据
	ComboResultInfo_t* pCurCri;
    Matrix_u pFreeMxu[GE_MaxFreeNum];
	int32_t FreeBetArray[GE_MaxFreeNum];
    int32_t FreeIDVec[GE_MaxFreeNum][GE_MaxIDNum];
	int8_t nFreeNum;
	int32_t nFreeBet;
	Matrix_u LooseMxus[GE_MaxFreeNum];
	int8_t WildPosArray[GE_MaxFreeNum][GE_WheelChessMaxNum];

	// 大奖数据
	int32_t nBonusBet;
	int8_t nBonusType;
	int8_t BlindSymbol; //神秘图标或者乘数游戏的倍数
	int32_t BonusData[GE_WheelChessMaxNum];
} RoundInfo_t;
// RoundInfo_t 初始化函数
void RoundInfo_t_Init(RoundInfo_t* obj);
void RoundInfo_t_Reset(RoundInfo_t* obj);
void RoundInfo_t_Copy(RoundInfo_t* dest, const RoundInfo_t* src);
//-------------------------------------------------------------------------------------
// 免费游戏信息结构体
typedef struct {
	RoundInfo_t roundInfo;
	int32_t nBetVal;
	int8_t nCurFreeIdx;
	int8_t nTotalFreeTime;
	int32_t nRemainFreeBet;
} FreeGameInfo_t;

// FreeGameInfo_t 初始化函数
void FreeGameInfo_t_Init(FreeGameInfo_t* obj);
void FreeGameInfo_t_Reset(FreeGameInfo_t* obj);
//-------------------------------------------------------------------------------------
//联网彩金信息结构体
typedef struct {
	int32_t nBetVal;
	int8_t nJpOnlineType;
	int32_t nJpOnlineWin;
}JackpotOnlineInfo_t;

// JackpotOnlineInfo_t 初始化函数
void JackpotOnlineInfo_t_Init(JackpotOnlineInfo_t* obj);
void JackpotOnlineInfo_t_Reset(JackpotOnlineInfo_t* obj);
//-------------------------------------------------------------------------------------
// 开奖输出结构
typedef struct {
	uint8_t  openType;                      // 开奖类型：普通/赠送
	uint8_t  resType;                       // 结果类型
	int32_t  IDVec[GE_MaxIDNum];            // 中奖线 ID 信息（最多 20 条）
	int32_t  nMatrixBet;                    // 线奖总倍数
	Matrix_u matrix;                        // 开奖矩阵

	/* 当结果类型为 RT_FreeWin 时，以下字段有效 */
	int32_t nTotalFreeBet;                  // 预计免费游戏总倍数
	uint8_t nTotalFreeTime;                 // 免费游戏总次数
	int32_t FreeBetArray[GE_MaxFreeNum];
	int8_t WildPosArray[GE_WheelChessMaxNum];

	/* 当结果类型为 RT_BonusWin 时，以下字段有效 */
	int32_t nBonusBet;
	uint8_t BlindSymbol;                   // 神秘图标或乘数值（按玩法解释）
	int32_t BonusData[GE_WheelChessMaxNum];   // Bonus 结果数据
	uint8_t nBonusType;

	int32_t nJPBet;
	uint8_t nJPType;                        // 彩金类型
} OutResult_t;

// OutResult_t 相关接口
void OutResult_Init(OutResult_t* pResult);
void OutResult_reset(OutResult_t* pResult);
//-------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------
// 调试统计信息
typedef struct {
	int64_t dwPlayScore;				// 总下注
	int64_t dwWinScore;					// 总赢钱
	int64_t dwTotalPlayTime;			// 总局数
	int64_t dwNormalOpenTime;			// 普通开局次数
	int64_t dwGiveOpenTime;				// 赠送开局次数
	int64_t dwNormalWinTime;			// 普通中奖次数
	int64_t dwBonusTime;				// Bonus 次数
	int64_t dwFreeGameTime;				// 免费游戏触发次数
	int64_t dwLooseTime;				// 输局次数
	int64_t dwJackpotTime;				// Jackpot 次数
	int64_t dwJackpotOnlineTime;		// 联网Jackpot 次数
	int64_t dwBaseWinScore;				// 线奖赢钱总额（金额口径）
	int64_t dwFreeWinScore;				// 免费游戏赢钱总额（金额口径）
	int64_t dwBonusWinScore;			// Bonus 游戏赢钱总额（金额口径）
	int64_t dwJackpotWinScore;			// Jackpot 赢钱总额（金额口径）
	int64_t dwJackpotOnlineWinScore;    // 联网Jackpot 赢钱总额（金额口径）
	int64_t dwFreeGameBetError;			// 免费游戏剩余倍数误差
} DebugInfo;

void DebugInfo_reset(DebugInfo* pInfo);
void DebugInfo_accum(DebugInfo* pDest, DebugInfo* pSrc);
int8_t* DebugInfo_toJason(DebugInfo* pInfo);// DebugInfo_toJason 返回堆内存字符串，调用方必须 free
#endif // _CommonStruct_H_


