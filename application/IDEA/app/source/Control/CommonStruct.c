
#include "CommonStruct.h"
#include <string.h>
#include <stdlib.h>

// ComboResultInfo_t 函数实现
void ComboResultInfo_t_Init(ComboResultInfo_t* obj)
{
	if (obj == NULL) return;
	memset(obj, 0, sizeof(ComboResultInfo_t));
}

void ComboResultInfo_t_Reset(ComboResultInfo_t* obj)
{
	if (obj == NULL) return;
	memset(obj, 0, sizeof(ComboResultInfo_t));
}

// RoundInfo_t 初始化函数
void RoundInfo_t_Init(RoundInfo_t* obj) {
    if (obj == NULL) return;

    obj->resType = RT_Lose; // 假设有默认枚举值
    obj->nMatrixBet = 0;
    obj->nPlayBetValue = 0;

    // 初始化extraMxu
    Matrix_u_reset(&obj->extraMxu);

    // 初始化免费数据相关
    obj->pCurCri = NULL;
    obj->nFreeNum = 0;
    obj->nFreeBet = 0;
   
    for (int8_t i = 0; i < GE_MaxFreeNum; i++) 
    {
        // 初始化pFreeMxu数组
        Matrix_u_reset(&obj->pFreeMxu[i]);
        // 初始化FreeBetArray数
        obj->FreeBetArray[i] = 0;
        // 初始化FreeIDVec二维数组
        for (int8_t j = 0; j < GE_MaxIDNum; j++) {
            obj->FreeIDVec[i][j] = 0;
        }

        for (int8_t j = 0; j < GE_WheelChessMaxNum; j++) {
            obj->WildPosArray[i][j] = 0;
        }
        // 初始化LooseMxus数组
        Matrix_u_reset(&obj->LooseMxus[i]);
    }

    // 初始化彩金数据
    obj->nBonusBet = 0;
    obj->nBonusType = 0;
    obj->BlindSymbol = 0;
    for (int8_t i = 0; i < GE_WheelChessMaxNum; i++) 
    {
        obj->BonusData[i] = 0;
    }
}

// RoundInfo_t 重置函数
void RoundInfo_t_Reset(RoundInfo_t* obj) {
    if (obj == NULL) return;

    // 重置基本数据
    obj->resType = RT_Lose;
    obj->nMatrixBet = 0;
    obj->nPlayBetValue = 0;

    // 重置extraMxu
    Matrix_u_reset(&obj->extraMxu);

    // 重置免费数据相关指针（不释放内存，只置NULL）
    obj->pCurCri = NULL;
    obj->nFreeNum = 0;
    obj->nFreeBet = 0;

   
    for (int8_t i = 0; i < GE_MaxFreeNum; i++)
    {
        // 重置pFreeMxu数组
        Matrix_u_reset(&obj->pFreeMxu[i]);
        // 重置FreeBetArray数组
        obj->FreeBetArray[i] = 0;
        // 重置FreeIDVec二维数组
        for (int32_t j = 0; j < GE_MaxIDNum; j++) {
            obj->FreeIDVec[i][j] = 0;
        }
        for (int32_t j = 0; j < GE_WheelChessMaxNum; j++) {
            obj->WildPosArray[i][j] = 0;
        }
        // 重置LooseMxus数组
        Matrix_u_reset(&obj->LooseMxus[i]);
    }

    // 重置彩金数据
    obj->nBonusBet = 0;
    obj->nBonusType = 0;
    obj->BlindSymbol = 0;
    for (int8_t i = 0; i < GE_WheelChessMaxNum; i++) {
        obj->BonusData[i] = 0;
    }
}

// RoundInfo_t 拷贝函数
void RoundInfo_t_Copy(RoundInfo_t* dest, const RoundInfo_t* src) {
    if (dest == NULL || src == NULL) return;

    // 拷贝基本数据
    dest->resType = src->resType;
    dest->nMatrixBet = src->nMatrixBet;
    dest->nPlayBetValue = src->nPlayBetValue;

    // 拷贝extraMxu
    Matrix_u_copy(&dest->extraMxu, &src->extraMxu); // 注意：需要类型转换

    // 拷贝免费数据相关（指针只拷贝地址，不深拷贝指向的内容）
    dest->pCurCri = src->pCurCri;
    dest->nFreeNum = src->nFreeNum;
    dest->nFreeBet = src->nFreeBet;

    
    for (int8_t i = 0; i < GE_MaxFreeNum; i++)
    {
        // 拷贝pFreeMxu数组
        Matrix_u_copy(&dest->pFreeMxu[i], (Matrix_u*)&src->pFreeMxu[i]);
        // 拷贝FreeBetArray数组
        dest->FreeBetArray[i] = src->FreeBetArray[i];
        // 拷贝FreeIDVec二维数组
        for (int8_t j = 0; j < GE_MaxIDNum; j++) {
            dest->FreeIDVec[i][j] = src->FreeIDVec[i][j];
        }

        for (int32_t j = 0; j < GE_WheelChessMaxNum; j++) {
            dest->WildPosArray[i][j] = src->WildPosArray[i][j];
        }
        // 拷贝LooseMxus数组
        Matrix_u_copy(&dest->LooseMxus[i], (Matrix_u*)&src->LooseMxus[i]);
    }
  
    // 拷贝彩金数据
    dest->nBonusBet = src->nBonusBet;
    dest->nBonusType = src->nBonusType;
    dest->BlindSymbol = src->BlindSymbol;
    for (int8_t i = 0; i < GE_WheelChessMaxNum; i++) {
        dest->BonusData[i] = src->BonusData[i];
    }
}

// FreeGameInfo_t 函数实现
void FreeGameInfo_t_Init(FreeGameInfo_t* obj)
{
	if (obj == NULL) return;
	memset(obj, 0, sizeof(FreeGameInfo_t));
	RoundInfo_t_Init(&obj->roundInfo);
}

void FreeGameInfo_t_Reset(FreeGameInfo_t* obj)
{
	if (obj == NULL) return;
	obj->nCurFreeIdx = 0;
	obj->nRemainFreeBet = 0;
	obj->nBetVal = 0;
	obj->nTotalFreeTime = 0;
	// 注意：原C++代码中不重置roundInfo，保持相同行为
}


// JackpotOnlineInfo_t 函数实现
void JackpotOnlineInfo_t_Init(JackpotOnlineInfo_t* obj)
{
    if (obj == NULL) return;
    memset(obj, 0, sizeof(JackpotOnlineInfo_t));
}

void JackpotOnlineInfo_t_Reset(JackpotOnlineInfo_t* obj)
{
    if (obj == NULL) return;
    memset(obj, 0, sizeof(JackpotOnlineInfo_t));
}
