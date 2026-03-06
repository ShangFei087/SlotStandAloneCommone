#ifndef _CommonStruct_H_
#define _CommonStruct_H_

#include "CMD_Fish.h"


// ComboResultInfo_t 结构体
typedef struct {
	int32_t IDVec[GE_MaxIDNum];
	int32_t nMatrixBet;  // 总倍率
	Matrix_u matrix; // matrix数据
} ComboResultInfo_t;
// ComboResultInfo_t 初始化函数
void ComboResultInfo_t_Init(ComboResultInfo_t* obj);
void ComboResultInfo_t_Reset(ComboResultInfo_t* obj);

// 一轮结果结构体
typedef struct {
	int8_t resType;
	int32_t nMatrixBet;      // 阵列赔率
	int32_t nPlayBetValue;   // 压分

	Matrix_u extraMxu;   // 用于免费或彩金游戏阵列

	// 免费数据
	ComboResultInfo_t* pCurCri;
    Matrix_u pFreeMxu[GE_MaxFreeNum];
	int32_t FreeBetArray[GE_MaxFreeNum];
    int32_t FreeIDVec[GE_MaxFreeNum][GE_MaxIDNum];
	int8_t nFreeNum;
	int32_t nFreeBet;
	Matrix_u LooseMxus[GE_MaxFreeNum];
	int8_t WildPosArray[GE_MaxFreeNum][GE_WheelChessNum];

	// 彩金数据
	int32_t nBonusBet;
	int8_t nBonusType;
	int8_t BlindSymbol; //神秘图标或者乘数游戏的倍数
	int32_t BonusData[GE_WheelChessNum];
} RoundInfo_t;
// RoundInfo_t 初始化函数
void RoundInfo_t_Init(RoundInfo_t* obj);
void RoundInfo_t_Reset(RoundInfo_t* obj);
void RoundInfo_t_Copy(RoundInfo_t* dest, const RoundInfo_t* src);

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



#endif // _CommonStruct_H_


