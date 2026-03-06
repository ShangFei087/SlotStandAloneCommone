#ifndef CMD_FISH_H_
#define CMD_FISH_H_

#include <string.h>
#include "qs.h"
#include "GameConfig.h"

//--------------------------------------------全局配置(3个游戏最大的值)-------------------------------------------//
//调试模式枚举
#define DCM_Normal 0
#define DCM_PointResData 1

//全局参数每局
#define GE_WheelChessNum 15
#define GE_MaxIDNum 20
#define GE_Line20Num  20  //20线数
#define GE_Line15Num  15  //15线
#define GE_MaxFreeNum  20


//开奖结果类型
#define RT_Lose 0
#define RT_Win 1
#define RT_FreeWin 2
#define RT_BonusWin 3
#define RT_JpMaior  4

//开奖方式类型
#define OT_Normal 0
#define OT_Give   1

//彩金类型
#define JT_None 0
#define JT_Major 1
#define JT_minor 2
#define JT_mini 3

//-------------------------------------------------------------------------------------

//检查一条线的结果
typedef struct {
	uint8_t bIsEliminate;              //是否能消除
	uint8_t bHasWild;                  //是否有WIld
	uint8_t chessTypeVec[5];         //改线上的5个图标
	uint8_t posVec[5];               //改线上的5个位置
	uint8_t  nWildNum;              //wild数目
	uint8_t  nAvailChessType;       //有效的消除Type
	uint8_t  nEliminateNum;         //消除数目
	uint8_t  nEliminateLineId;      //消除线索引
} CheckOnLineResult_t;

//初始化CheckOnLineResult_t
void CheckOnLineResult_Init(CheckOnLineResult_t* pResult);

//-------------------------------------------------------------------------------------
//阵列
typedef struct {
	uint8_t resultType;
	uint8_t idVecSize;
	uint8_t dataArray[GE_WheelChessNum];
} Matrix_u;

// Matrix_u 相关函数声明
uint8_t Matrix_u_getIntData(Matrix_u* pMatrix, uint8_t pos);
void Matrix_u_setIntData(Matrix_u* pMatrix, uint8_t* pData);
void Matrix_u_print(Matrix_u* pMatrix);
uint8_t Matrix_u_getTypeNum(Matrix_u* pMatrix, uint8_t type);
void Matrix_u_reset(Matrix_u* pMatrix);
void Matrix_u_copy(Matrix_u* pDest, Matrix_u* pSrc);

void Matrix_u_insertScatter(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_insertCol(Matrix_u* pMatrix, uint8_t type, uint8_t col);//填满一列相同类型图标
void Matrix_u_insertBonus(Matrix_u* pMatrix, uint8_t num);
void Matrix_u_GenerateAnyResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus);
void Matrix_u_GenerateLooseResult(Matrix_u* pMatrix, uint8_t hasWild, uint8_t hasScatter, uint8_t hasBonus, uint8_t hasBoost);

uint8_t Matrix_u_computerMatrix_243(Matrix_u* pMatri, int32_t* idVec); //243线计算
int32_t Matrix_u_computerMatrixById(Matrix_u* pMatrix, int32_t* idVec,SlotGameConfig_t* gameConfig, uint32_t gameId);//有线计算
uint8_t Matrix_u_checkOnLine(Matrix_u* pMatrix, uint8_t lineIdx, CheckOnLineResult_t* pClr, SlotGameConfig_t* gameConfig);

//输出结果
typedef struct {
	uint8_t  openType;                   //开奖类型，普通-赠送
	uint8_t resType;                   //结果类型
	uint8_t JPType;
	int32_t IDVec[GE_MaxIDNum];             //开奖的ID类型，最多为20个
	int32_t nMatrixBet;                     //阵列所对应的赔率
	Matrix_u matrix;                        //阵列

	/*若开奖类型不为RT_FreeWin，则以下值均无意义*/
	int32_t nTotalFreeBet;                  //表示免费游戏总赔率
	uint8_t nTotalFreeTime;                 //改数表示免费游戏次数
	int32_t FreeBetArray[GE_MaxFreeNum];

	//uint8_t uint16_t
	/*若开奖类型不为RT_BonusWin，则以下值均无意义*/
	int32_t nBonusBet;
	uint8_t BlindSymbol; //神秘图标
	int32_t BonusData[GE_WheelChessNum];                  //大奖结果
	uint8_t nBonusType;
	
	int32_t nJPBet;
} OutResult_t;

// OutResult_t 相关函数声明
void OutResult_Init(OutResult_t* pResult);
void OutResult_reset(OutResult_t* pResult);

//调试控制结构
typedef struct {
	uint8_t mode;    //调试模式
	uint8_t resType;       //结果类型
	uint8_t bonusType;       //大奖类型
	uint8_t jpType;       //大奖类型
} DebugControlMode_t;

void DebugControlMode_Init(DebugControlMode_t* pMode);
void  DLL_SetControlDebugMode(DebugControlMode_t* pDebugMode);
extern DebugControlMode_t gDebugControlMode;


//调试信息
typedef struct {
	int64_t dwPlayScore;       //算法总玩分
	int64_t dwWinScore;        //算法总赢分
	int64_t dwTotalPlayTime;      //总玩次数
	int64_t dwNormalOpenTime;     //普通开奖次数
	int64_t dwGiveOpenTime;       //赠送开奖次数
	int64_t dwNormalWinTime;      //普通奖次数
	int64_t dwBonusTime;          //彩金次数
	int64_t dwFreeGameTime;       //免费游戏开出次数
	int64_t dwLooseTime;          //不中奖次数
	int64_t dwNormalWinTotalBet;   //普通赢送出的总赔率
	int64_t dwFreeGameTotalBet;   //免费游戏送出的总赔率
	int64_t dwBonusGameTotalBet;  //BonusGame送出的总赔率
	int64_t dwFreeGameBetError;   //免费游戏导致的误差
} DebugInfo;

void DebugInfo_reset(DebugInfo* pInfo);
void DebugInfo_accum(DebugInfo* pDest, DebugInfo* pSrc);
int8_t* DebugInfo_ArrayToString(int32_t* pArray, int32_t length, BOOL keepZero);
int8_t* DebugInfo_toJason(DebugInfo* pInfo);

// 全局变量声明


#endif // CMD_FISH_H_
