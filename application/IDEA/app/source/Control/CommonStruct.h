#ifndef _CommonStruct_H_
#define _CommonStruct_H_

#include "ComputerData.h"
#include "GameConfig.h"
//-------------------------------------------------------------------------------------
typedef struct {
	uint8_t bIsEliminate;            // 是否发生消除
	uint8_t bHasWild;                // 是否包含 Wild
	uint8_t chessTypeVec[5];         // 消除线中可用棋子类型（5 个）
	uint8_t posVec[5];               // 消除线中各位置索引（对应 5 列）
	uint8_t nWildNum;                // Wild 数量
	uint8_t nAvailChessType;         // 可用棋子类型（Wild/Scatter/Bonus）
	uint8_t nEliminateNum;           // 消除数量
	uint8_t nEliminateLineId;        // 消除对应的线/行 ID
} CheckOnLineResult_t;

// 初始化 CheckOnLineResult_t
void CheckOnLineResult_Init(CheckOnLineResult_t* pResult);
//-------------------------------------------------------------------------------------
// Matrix_u 线判断结果结构
typedef struct {
	uint8_t resultType;
	uint8_t idVecSize;
	uint8_t dataArray[GE_WheelChessMaxNum];
} Matrix_u;

// Matrix_u 生成/统计连线相关接口
uint8_t Matrix_u_getIntData(Matrix_u* pMatrix, SlotGameConfig_t slotgameconfig, uint8_t pos);
void Matrix_u_setIntData(Matrix_u* pMatrix, SlotGameConfig_t slotgameconfig, uint8_t* pData);
uint8_t Matrix_u_getTypeNum(Matrix_u* pMatrix, SlotGameConfig_t slotgameconfig, uint8_t type);
void Matrix_u_reset(Matrix_u* pMatrix);
void Matrix_u_copy(Matrix_u* pDest, Matrix_u* pSrc);

void Matrix_u_insertCol(Matrix_u* pMatrix, uint8_t type, uint8_t col);// 在指定列插入指定棋子类型
// 插入 Scatter/Bonus 并生成任意/Loose 结果
void Matrix_u_insertScatter(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_insertBonus(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_GenerateAnyResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus);
void Matrix_u_GenerateLooseResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus, uint8_t hasBoost);

uint8_t Matrix_u_computerMatrix_243(Matrix_u* pMatri, uint16_t* idVec); // 计算 243 条线组合结果
int32_t Matrix_u_computerMatrixById(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, uint32_t gameId);// 根据组合 ID 生成矩阵

//-------------------------------------------------------------------------------------
// ComboResultInfo_t 连线/组合结果信息
typedef struct {
	uint16_t IDVec[GE_MaxIDNum];
	uint16_t nMatrixBet;  // 当前局下注金额
	Matrix_u matrix; // 当前局矩阵数据
} ComboResultInfo_t;
// ComboResultInfo_t 初始化/重置
void ComboResultInfo_t_Init(ComboResultInfo_t* obj);
void ComboResultInfo_t_Reset(ComboResultInfo_t* obj);
//-------------------------------------------------------------------------------------
// RoundInfo_t 轮次结算信息（普通/免费/Bonus）
typedef struct {
	int8_t resType;
	int32_t nMatrixBet;      // 本局下注金额
	uint8_t nPlayBetValue;   // 当前下注/返还下注值（按协议使用）
	Matrix_u extraMxu;      // 扩展信息（附加 mxu）
	// 当前局的组合结果缓存
	ComboResultInfo_t* pCurCri;
    Matrix_u pFreeMxu[GE_MaxFreeNum];
	uint32_t FreeBetArray[GE_MaxFreeNum];
	uint16_t FreeIDVec[GE_MaxFreeNum][GE_MaxIDNum];
	uint8_t nFreeNum;
	uint32_t nFreeBet;
	//Matrix_u LooseMxus[GE_MaxFreeNum];
	int8_t WildPosArray[GE_MaxFreeNum][GE_WheelChessMaxNum];
	// Bonus 结果
	uint32_t nBonusBet;
	int8_t nBonusType;
	int8_t BlindSymbol; // 盲符号/转轮等级
	uint16_t BonusData[GE_WheelChessMaxNum];
} RoundInfo_t;
// RoundInfo_t 初始化/重置/复制
void RoundInfo_t_Init(RoundInfo_t* obj);
void RoundInfo_t_Reset(RoundInfo_t* obj);
void RoundInfo_t_Copy(RoundInfo_t* dest, const RoundInfo_t* src);
//-------------------------------------------------------------------------------------
// FreeGameInfo_t 免费游戏状态
typedef struct {
	RoundInfo_t roundInfo;
	uint16_t nBetVal;
	uint8_t nCurFreeIdx;
	uint8_t nTotalFreeTime;
	int32_t nRemainFreeBet;
} FreeGameInfo_t;

// FreeGameInfo_t 初始化/重置
void FreeGameInfo_t_Init(FreeGameInfo_t* obj);
void FreeGameInfo_t_Reset(FreeGameInfo_t* obj);
//-------------------------------------------------------------------------------------
// JackpotOnlineInfo_t Jackpot 在线信息
typedef struct {
	uint16_t nBetVal;
	uint8_t nJpOnlineType;
	uint32_t nJpOnlineWin;
}JackpotOnlineInfo_t;

// JackpotOnlineInfo_t 初始化/重置
void JackpotOnlineInfo_t_Init(JackpotOnlineInfo_t* obj);
void JackpotOnlineInfo_t_Reset(JackpotOnlineInfo_t* obj);
//-------------------------------------------------------------------------------------
// OutResult_t 输出结果的序列化数据
typedef struct {
	uint8_t  openType;                      // 打开类型：普通/赠送
	uint8_t  resType;                       // 结果类型
	uint16_t  IDVec[GE_MaxIDNum];            // 中奖 ID 列表
	int32_t  nMatrixBet;                    // 下注金额
	Matrix_u matrix;                        // 矩阵数据

	/* 当 resType 为 RT_FreeWin 时，下列字段生效 */
	int32_t nTotalFreeBet;                  // 免费局赢取金额（预留）
	uint8_t nTotalFreeTime;                 // 免费局次数（预留）
	int32_t FreeBetArray[GE_MaxFreeNum];
	int8_t WildPosArray[GE_WheelChessMaxNum];

	/* 当 resType 为 RT_BonusWin 时，下列字段生效 */
	int32_t nBonusBet;
	uint8_t BlindSymbol;                   // 盲符号/转轮等级
	uint16_t BonusData[GE_WheelChessMaxNum];   // Bonus 金额
	uint8_t nBonusType;

	int32_t nJPBet;
	uint8_t nJPType;                        // Jackpot 类型
} OutResult_t;

// OutResult_t 初始化/重置
void OutResult_Init(OutResult_t* pResult);
void OutResult_reset(OutResult_t* pResult);
//-------------------------------------------------------------------------------------
// DebugControlMode_t 调试控制模式结构
typedef struct {
	uint8_t mode;       // 调试模式
	uint8_t resType;    // 目标结果类型
	uint8_t bonusType;  // 目标 Bonus 类型
	uint8_t jpType;     // 目标 Jackpot 类型
} DebugControlMode_t;

void DebugControlMode_Init(DebugControlMode_t* pMode);
void  DLL_SetControlDebugMode(DebugControlMode_t* pDebugMode);
extern DebugControlMode_t gDebugControlMode;
//-------------------------------------------------------------------------------------
// DebugInfo 调试统计信息
typedef struct {
	uint32_t dwFlag;
	uint32_t dwPlayScore;				// 游戏总下注分
	uint32_t dwWinScore;					// 游戏总赢分
	uint32_t dwTotalPlayTime;			// 总游戏时长/次数
	uint32_t dwNormalOpenTime;			// 普通开局次数/时间
	uint32_t dwGiveOpenTime;				// 赠送开局次数/时间
	uint32_t dwNormalWinTime;			// 普通赢局次数/时间
	uint32_t dwBonusTime;				// Bonus 发生次数/时间
	uint32_t dwFreeGameTime;				// 免费游戏次数/时间
	uint32_t dwLooseTime;				// 输局次数/时间
	uint32_t dwJackpotTime;				// Jackpot 发生次数/时间
	uint32_t dwJackpotOnlineTime;		// Jackpot 在线发生次数/时间
	uint32_t dwBaseWinScore;				// 基础赢分
	uint32_t dwFreeWinScore;				// 免费赢分
	uint32_t dwBonusWinScore;			// Bonus 赢分
	uint32_t dwJackpotWinScore;			// Jackpot 赢分
	uint32_t dwJackpotOnlineWinScore;    // Jackpot 在线赢分
	uint32_t dwFreeGameBetError;			// 免费游戏下注错误计数
} DebugInfo;

void DebugInfo_reset(DebugInfo* pInfo);
void DebugInfo_accum(DebugInfo* pDest, DebugInfo* pSrc);
int8_t* DebugInfo_toJason(DebugInfo* pInfo);// DebugInfo 转 JSON 字符串；调用方需释放返回值
#endif // _CommonStruct_H_

