#include "CommonStruct.h"
#include <string.h>
#include <stdlib.h>
#include "Test.h"
#include "Matrix/Matrix_u_TriggersById.h"
DebugControlMode_t gDebugControlMode;
//-------------------------------------------------------------------------------------
void CheckOnLineResult_Init(CheckOnLineResult_t* pResult)
{
    memset(pResult, 0, sizeof(CheckOnLineResult_t));
    pResult->bIsEliminate = 0;
    pResult->bHasWild = 0;
    pResult->nAvailChessType = 0;
    pResult->nEliminateNum = 0;
    pResult->nEliminateLineId = 0;
    pResult->nWildNum = 0;
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
uint8_t Matrix_u_getTypeNum(Matrix_u* pMatrix, SlotGameConfig_t slotgameconfig,uint8_t type)
{
    uint8_t nNum = 0;
    uint8_t i = 0;
    for (; i < slotgameconfig.header.wheelChessNum; ++i)
    {
        if (pMatrix->dataArray[i] == type) nNum++;
    }

    return nNum;
}
uint8_t Matrix_u_getIntData(Matrix_u* pMatrix, SlotGameConfig_t slotgameconfig, uint8_t pos)
{
    if (pos > slotgameconfig.header.wheelChessNum) return -1;
    uint8_t type = 0;
    type = pMatrix->dataArray[pos];

    return type;
}
void Matrix_u_setIntData(Matrix_u* pMatrix, SlotGameConfig_t slotgameconfig, uint8_t* pData)
{
    for (uint8_t i = 0; i < slotgameconfig.header.wheelChessNum; ++i)
    {
        pMatrix->dataArray[i] = pData[i];
    }

}
void Matrix_u_reset(Matrix_u* pMatrix)
{
    memset(pMatrix->dataArray, 0, sizeof(pMatrix->dataArray));
    pMatrix->resultType = RT_Lose;
    pMatrix->idVecSize = 0;
}
void Matrix_u_copy(Matrix_u* pDest, Matrix_u* pSrc)
{
    if (pDest == NULL || pSrc == NULL) {
        return; // 输入为空，直接返回
    }
    // 复制矩阵结构体成员（包含 dataArray）
    pDest->resultType = pSrc->resultType;
    pDest->idVecSize = pSrc->idVecSize;
    memcpy(pDest->dataArray, pSrc->dataArray, sizeof(pSrc->dataArray));
}
void Matrix_u_insertCol(Matrix_u* pMatrix, uint8_t type, uint8_t col)
{
    uint8_t index = 0;
    uint8_t i = 0;
    for (; i < 3; ++i)
    {
        index = 5 * i + col;
        pMatrix->dataArray[index] = type;
    }
}

//243

//------------------------------------------------------------------------------------
//// ChessLine_t 结构映射（示例：std::map）
//typedef struct {
//    uint8_t chessID;
//    uint8_t chessNum;
//    uint8_t chessOnline[5];
//} ChessLine_t;
//void ChessLine_calcChessNum(ChessLine_t* pChessLine)
//{
//    pChessLine->chessNum = 0;
//    for (uint8_t i = 0; i < 5; ++i)
//    {
//        if (pChessLine->chessOnline[i] > 0) pChessLine->chessNum++;
//        else break;
//    }
//}
//void ChessLine_init(ChessLine_t* pChessLine, uint8_t id, uint8_t num, uint8_t line)
//{
//    pChessLine->chessID = id;
//    pChessLine->chessNum = num;
//    memset(pChessLine->chessOnline, 0, sizeof(pChessLine->chessOnline));
//    pChessLine->chessOnline[0] = line;
//}
//// 简单映射结构实现
//typedef struct {
//    ChessLine_t entries;
//    uint8_t count;
//    uint8_t capacity;
//} ChessLineMap;
//
//void ChessLineMap_init(ChessLineMap* pMap, uint8_t capacity)
//{
//    pMap->count = 0;
//    pMap->capacity = capacity;
//}
//
//void ChessLineMap_free(ChessLineMap* pMap)
//{
//    pMap->count = 0;
//    pMap->capacity = 0;
//}
//
//ChessLine_t* ChessLineMap_find(ChessLineMap* pMap, uint8_t id)
//{
//    for (uint8_t i = 0; i < pMap->count; ++i)
//    {
//        if (pMap.entries[i].chessID == id)
//            return &pMap->entries[i];
//    }
//    return NULL;
//}
//
//void ChessLineMap_insert(ChessLineMap* pMap, int32_t id, int32_t num, int32_t line)
//{
//    if (pMap->count >= pMap->capacity) return;
//
//    ChessLine_init(&pMap->entries[pMap->count], id, num, line);
//    pMap->count++;
//}
//
//int32_t Matrix_u_computerMatrix_243(Matrix_u* pMatrix, int32_t* idVec)
//{
//    // 初始化 idVec（清零并重置计数）
//    for (int32_t i = 0; i < GE_MaxIDNum; ++i) idVec[i] = 0;
//    int32_t idVecCount = 0;
//    int32_t nLocalWinBet = 0;
//
//    // 初始化映射结构
//    ChessLineMap* chessLineMap = (ChessLineMap*)malloc(sizeof(ChessLineMap));
//
//    ChessLineMap_init(&chessLineMap, GE_ChessTypeNum); // 初始化最多 12 种不同棋子
//
//    // 统计前 3 个位置的棋子出现情况（含 Wild 逻辑）
//    int32_t id = 0;
//    BOOL iswild = FALSE;
//    for (int32_t i = 0; i < 3; ++i)
//    {
//        id = pMatrix->dataArray[i * 5];
//        ChessLine_t* pEntry = ChessLineMap_find(chessLineMap, id);
//        if (id == CT_Wild)
//        {
//            iswild = TRUE;
//        }
//        if (pEntry == NULL)
//        {
//            ChessLineMap_insert(chessLineMap, id, 1, 1);
//        }
//        else
//        {
//            pEntry->chessOnline[0]++;
//        }
//    }
//
//    // 若出现 Wild，则补齐后续列中可能出现的棋子映射
//    if (iswild)
//    {
//        // 遍历第二列及之后的列
//        for (int32_t c = 1; c < GE_ColNum; ++c)
//        {
//            // 遍历每一行的棋子并加入映射
//            for (int32_t r = 0; r < GE_RowNum; ++r)
//            {
//                id = pMatrix->dataArray[r * GE_ColNum + c];
//                if (ChessLineMap_find(chessLineMap, id) == NULL)
//                {
//                    ChessLineMap_insert(chessLineMap, id, 1, 1);
//                }
//            }
//        }
//    }
//
//    // 遍历每条线，统计对应 id 的出现次数
//    for (int32_t lineIdx = 1; lineIdx < 5; ++lineIdx)
//    {
//        for (int32_t i = 0; i < 3; ++i)
//        {
//            id = pMatrix->dataArray[lineIdx + i * 5];
//            if (id == CT_Wild) assert(lineIdx >= 1);
//
//            ChessLine_t* pEntry = ChessLineMap_find(chessLineMap, id);
//            if (pEntry != NULL)
//            {
//                pEntry->chessOnline[lineIdx]++;
//            }
//            if (id == CT_Wild)
//            {
//                // 若为 Wild，则映射表中每个普通棋子的计数也要加一
//                for (int32_t j = 0; j < chessLineMap->count; ++j)
//                {
//
//                    if (chessLineMap->entries[j].chessID < CT_Wild)
//                        chessLineMap->entries[j].chessOnline[lineIdx]++;
//                }
//            }
//        }
//    }
//
//    // 计算组合并累计本局可中下注数
//    int32_t totalLine = 1;
//    for (int32_t i = 0; i < GE_ChessTypeNum; ++i)
//    {
//        ChessLine_t* pEntry = ChessLineMap_find(chessLineMap, i);
//        if (pEntry != NULL)
//        {
//            id = pEntry->chessID;
//            ChessLine_calcChessNum(pEntry);
//            totalLine = 1;
//            if (pEntry->chessNum >= 2)
//            {
//                if (gBetArray[id][pEntry->chessNum - 2] > 0)
//                {
//                    for (int32_t j = 0; j < pEntry->chessNum; ++j)
//                    {
//                        assert(pEntry->chessOnline[j] > 0);
//                        totalLine *= pEntry->chessOnline[j];
//                    }
//                    nLocalWinBet += totalLine * gBetArray[id][pEntry->chessNum - 2];
//                    if (idVecCount < GE_MaxIDNum)
//                    {
//                        idVec[idVecCount++] = totalLine * 1000 + pEntry->chessNum * 100 + id;
//                    }
//                }
//                pMatrix->resultType = RT_Win;
//            }
//        }
//
//    }
//
//    pMatrix->idVecSize = idVecCount;
//
//    // 判断是否命中 Scatter（免费）
//    int32_t scatterCount = 0;
//    int32_t freeOdds = 0;
//    for (int32_t i = 0; i < GE_WheelChessMaxNum; ++i)
//    {
//        if (pMatrix->dataArray[i] == CT_Scater)
//        {
//            ++scatterCount;
//        }
//    }
//
//    if (scatterCount >= 3)
//    {
//        pMatrix->resultType = RT_FreeWin;
//    }
//
//    // 判断是否命中 Bonus（彩金）
//    int32_t _bonusCount = 0;
//    int32_t bounsOdds = 0;
//    for (int32_t i = 0; i < GE_WheelChessMaxNum; ++i)
//    {
//        if (pMatrix->dataArray[i] == CT_Bonus)
//        {
//            ++_bonusCount;
//        }
//    }
//
//    if (_bonusCount >= 3)
//    {
//        pMatrix->resultType = RT_BonusWin;
//        nLocalWinBet += bounsOdds;
//    }
//
//    ChessLineMap_free(&chessLineMap);
//    free(chessLineMap);
//    return nLocalWinBet;
//}
//-------------------------------------------------------------------------------------

int32_t Matrix_u_computerMatrixById(Matrix_u* pMatrix, uint16_t* idVec, SlotGameConfig_t* gameConfig, uint32_t gameId, RoundInfo_t* info)
{
    uint32_t nLocalWinBet = computerMatrixById(pMatrix, idVec, gameConfig, gameId, info);
    return (int32_t)nLocalWinBet;
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
// RoundInfo_t 初始化函数
void RoundInfo_t_Init(RoundInfo_t* obj) {
    if (obj == NULL) return;

    obj->resType = RT_Lose; // 假设有默认枚举值
    obj->nMatrixBet = 0;
    obj->nPlayBetValue = 0;

    // 初始化extraMxu
    Matrix_u_reset(&obj->extraMxu);
    obj->nJPCount = 0;
    for (int8_t i = 0; i < GAME_Local_JP_MAX; i++)
    {
        obj->JPTypeArray[i] = JT_None;
        obj->JPBetArray[i] = 0;
    }
    obj->nTotalJackpotBet = 0;

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
        // NOTE: LooseMxus 在 RoundInfo_t 中已移除/不再使用
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
    obj->nJPCount = 0;
    for (int8_t i = 0; i < GAME_Local_JP_MAX; i++)
    {
        obj->JPTypeArray[i] = JT_None;
        obj->JPBetArray[i] = 0;
    }
    obj->nTotalJackpotBet = 0;

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
        // NOTE: LooseMxus 在 RoundInfo_t 中已移除/不再使用
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
    dest->nJPCount = src->nJPCount;
    for (int8_t i = 0; i < GAME_Local_JP_MAX; i++)
    {
        dest->JPTypeArray[i] = src->JPTypeArray[i];
        dest->JPBetArray[i] = src->JPBetArray[i];
    }
    dest->nTotalJackpotBet = src->nTotalJackpotBet;

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
        // NOTE: LooseMxus 在 RoundInfo_t 中已移除/不再使用
    }
  
    // 拷贝彩金数据
    dest->nBonusBet = src->nBonusBet;
    dest->nBonusType = src->nBonusType;
    dest->BlindSymbol = src->BlindSymbol;
    for (int8_t i = 0; i < GE_WheelChessMaxNum; i++) {
        dest->BonusData[i] = src->BonusData[i];
    }
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
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
	obj->nFreeTotalWin = 0;
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
void OutResult_Init(OutResult_t* pResult)
{
    if (pResult == NULL) {
        // 空指针保护
        return;
    }

    // 初始化结果枚举
    pResult->openType = OT_Normal;      // 默认普通开奖
    pResult->resType = RT_Lose;         // 默认未中奖

    // 初始化 ID 数组
    for (int8_t i = 0; i < GE_MaxIDNum; i++) {
        pResult->IDVec[i] = 0;
    }

    // 初始化核心数值
    pResult->nMatrixBet = 0;            // 线奖倍数
    pResult->nTotalFreeBet = 0;         // 免费总倍数
    pResult->nTotalFreeTime = 0;        // 免费总次数
    pResult->nBonusBet = 0;             // Bonus 倍数
    pResult->nBonusType = 0;            // Bonus 类型
    pResult->BlindSymbol = 0;           // 神秘图标/乘数

    // 初始化矩阵
    Matrix_u_reset(&pResult->matrix);

    // 初始化免费游戏数组
    for (int8_t i = 0; i < GE_MaxFreeNum; i++) {
        pResult->FreeBetArray[i] = 0;
    }

    for (int8_t i = 0; i < GE_WheelChessMaxNum; i++)
    {
        pResult->WildPosArray[i] = 0;
    }

    // 初始化 Bonus 数据数组
    for (int8_t i = 0; i < GE_WheelChessMaxNum; i++)
    {
        pResult->BonusData[i] = 0;
    }

    pResult->nJPCount = 0;
    for (int8_t i = 0; i < GAME_Local_JP_MAX; i++)
    {
        pResult->JPTypeArray[i] = JT_None;
        pResult->JPBetArray[i] = 0;
    }
    pResult->nTotalJackpotBet = 0;
}
void OutResult_reset(OutResult_t* pResult)
{
    if (pResult == NULL) {
        return;
    }

    // 重置结果枚举
    pResult->openType = OT_Normal;
    pResult->resType = RT_Lose;

    // 重置 ID 数组
    for (int8_t i = 0; i < GE_MaxIDNum; i++) {
        pResult->IDVec[i] = 0;
    }

    // 重置核心数值
    pResult->nMatrixBet = 0;
    pResult->nTotalFreeBet = 0;
    pResult->nTotalFreeTime = 0;
    pResult->nBonusBet = 0;
    pResult->nBonusType = 0;
    pResult->BlindSymbol = 0;            // 神秘图标/乘数

    // 重置矩阵
    Matrix_u_reset(&pResult->matrix);

    // 重置免费游戏数组
    for (int8_t i = 0; i < GE_MaxFreeNum; i++) {
        pResult->FreeBetArray[i] = 0;
    }

    for (int8_t i = 0; i < GE_WheelChessMaxNum; i++)
    {
        pResult->WildPosArray[i] = 0;
    }

    // 重置 Bonus 数据
    for (int8_t i = 0; i < GE_WheelChessMaxNum; i++) {
        pResult->BonusData[i] = 0;
    }

    pResult->nJPCount = 0;
    for (int8_t i = 0; i < GAME_Local_JP_MAX; i++)
    {
        pResult->JPTypeArray[i] = JT_None;
        pResult->JPBetArray[i] = 0;
    }
    pResult->nTotalJackpotBet = 0;
}

int32_t OutResult_GetLocalJPBetTotal(const OutResult_t* pResult)
{
	if (pResult != NULL && pResult->nTotalJackpotBet > 0)
	{
		return pResult->nTotalJackpotBet;
	}

	int32_t sum = 0;
	int32_t n;

	if (pResult == NULL)
	{
		return 0;
	}
	n = (int32_t)pResult->nJPCount;
	if (n > GAME_Local_JP_MAX)
	{
		n = GAME_Local_JP_MAX;
	}
	for (int32_t i = 0; i < n; ++i)
	{
		sum += pResult->JPBetArray[i];
	}
	return sum;
}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
void DebugControlMode_Init(DebugControlMode_t* pMode)
{
    pMode->mode = DCM_Normal;
    pMode->resType = RT_Lose;
    pMode->bonusType = 0;
    pMode->jpType = 0;
}
void  DLL_SetControlDebugMode(DebugControlMode_t* pDebugMode)
{
#ifndef _DebugControlMode
    return;
#endif
    gDebugControlMode.mode = pDebugMode->mode;
    gDebugControlMode.resType = pDebugMode->resType;
    gDebugControlMode.bonusType = pDebugMode->bonusType;
    gDebugControlMode.jpType = pDebugMode->jpType;

}
//-------------------------------------------------------------------------------------

//-------------------------------------------------------------------------------------
void DebugInfo_reset(DebugInfo* pInfo)
{
    memset(pInfo, 0, sizeof(DebugInfo));
}
void DebugInfo_accum(DebugInfo* pDest, DebugInfo* pSrc)
{
    pDest->dwPlayScore += pSrc->dwPlayScore;
    pDest->dwWinScore += pSrc->dwWinScore;
    pDest->dwTotalPlayTime += pSrc->dwTotalPlayTime;
    pDest->dwNormalOpenTime += pSrc->dwNormalOpenTime;
    pDest->dwGiveOpenTime += pSrc->dwGiveOpenTime;
    pDest->dwBonusTime += pSrc->dwBonusTime;
    pDest->dwFreeGameTime += pSrc->dwFreeGameTime;
    pDest->dwLooseTime += pSrc->dwLooseTime;
    pDest->dwFreeWinScore += pSrc->dwFreeWinScore;
    pDest->dwBonusWinScore += pSrc->dwBonusWinScore;
    pDest->dwJackpotTime += pSrc->dwJackpotTime;
    pDest->dwJackpotWinScore += pSrc->dwJackpotWinScore;
    pDest->dwJackpotOnlineTime += pSrc->dwJackpotOnlineTime;
    pDest->dwJackpotOnlineWinScore += pSrc->dwJackpotOnlineWinScore;
    pDest->dwFreeGameBetError += pSrc->dwFreeGameBetError;
    pDest->dwNormalWinTime += pSrc->dwNormalWinTime;
    pDest->dwBaseWinScore += pSrc->dwBaseWinScore;
}
int8_t* DebugInfo_toJason(DebugInfo* pInfo)
{
    char buffer[2048] = { 0 };
    size_t used = 0;
    int32_t written = 0;

    // 返回堆内存 JSON；调用方使用后释放，空输入直接返回 NULL。
    if (pInfo == NULL)
    {
        return NULL;
    }

    // 按剩余空间逐段拼接，避免 sprintf/strcat 的无界写入风险。
    written = snprintf(buffer + used, sizeof(buffer) - used, "{");
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwPlayScore\":%u,", pInfo->dwPlayScore);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwWinScore\":%u,", pInfo->dwWinScore);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwTotalPlayTime\":%u,", pInfo->dwTotalPlayTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwNormalOpenTime\":%u,", pInfo->dwNormalOpenTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwGiveOpenTime\":%u,", pInfo->dwGiveOpenTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwBonusTime\":%u,", pInfo->dwBonusTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwFreeGameTime\":%u,", pInfo->dwFreeGameTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwLooseTime\":%u,", pInfo->dwLooseTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwBaseWinScore\":%u,", pInfo->dwBaseWinScore);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwFreeWinScore\":%u,", pInfo->dwFreeWinScore);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwJackpotTime\":%u,", pInfo->dwJackpotTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwBonusWinScore\":%u,", pInfo->dwBonusWinScore);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwJackpotWinScore\":%u,", pInfo->dwJackpotWinScore);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwJackpotOnlineTime\":%u,", pInfo->dwJackpotOnlineTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwJackpotOnlineWinScore\":%u,", pInfo->dwJackpotOnlineWinScore);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwFreeGameBetError\":%u", pInfo->dwFreeGameBetError);
    if (written < 0) return NULL;
    used += (size_t)written;

    snprintf(buffer + used, sizeof(buffer) - used, "}");

    int8_t* result = (int8_t*)malloc(strlen(buffer) + 1);
    if (result)
        strcpy((char*)result, buffer);

    return result;
}
