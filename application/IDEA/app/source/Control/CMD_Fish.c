#include "CMD_Fish.h"
#include "ComputerData.h"
#include "Test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

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
//-------------------------------------------------------------------------------------
uint8_t Matrix_u_getTypeNum(Matrix_u* pMatrix, uint8_t type)
{
    uint8_t nNum = 0;
    uint8_t i = 0;
    for (; i < GE_WheelChessNum; ++i)
    {
        if (pMatrix->dataArray[i] == type) nNum++;
    }
       
    return nNum;
}
//-------------------------------------------------------------------------------------
void Matrix_u_reset(Matrix_u* pMatrix)
{
    memset(pMatrix->dataArray, 0, sizeof(pMatrix->dataArray));
    pMatrix->resultType = RT_Lose;
    pMatrix->idVecSize = 0;
}
//-------------------------------------------------------------------------------------
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
//-------------------------------------------------------------------------------------
uint8_t Matrix_u_getIntData(Matrix_u* pMatrix, uint8_t pos)
{
    if (pos > GE_WheelChessNum) return -1;
    uint8_t type = 0;
    type = pMatrix->dataArray[pos];
  
    return type;
}
//-------------------------------------------------------------------------------------
void Matrix_u_setIntData(Matrix_u* pMatrix, uint8_t* pData)
{
    for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
    {
        pMatrix->dataArray[i] = pData[i];
    }
       
}
//243

//-------------------------------------------------------------------------------------
//// ChessLine_t �ṹ������� std::map
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
//// �򵥵�ӳ���ʵ��
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
//    // ����������
//    for (int32_t i = 0; i < GE_MaxIDNum; ++i) idVec[i] = 0;
//    int32_t idVecCount = 0;
//    int32_t nLocalWinBet = 0;
//
//    // ����ӳ���
//    ChessLineMap* chessLineMap = (ChessLineMap*)malloc(sizeof(ChessLineMap));
//
//    ChessLineMap_init(&chessLineMap, GE_ChessTypeNum); // �������12�ֲ�ͬ������
//
//    //�ж���ͨ�н�
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
//    //�����һ����Wild,�����н�ͼ��
//    if (iswild)
//    {
//        //�ӵڶ��б���
//        for (int32_t c = 1; c < GE_ColNum; ++c)
//        {
//            //����ÿ�е�ͼ��
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
//    // ������ÿһ���Ƿ��ж�Ӧid
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
//                // ��ӳ����е�ÿ����ͨͼ�����Ӽ���
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
//    // ��������
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
//    //�ж�����н�
//    int32_t scatterCount = 0;
//    int32_t freeOdds = 0;
//    for (int32_t i = 0; i < GE_WheelChessNum; ++i)
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
//    //�жϴ��н�
//    int32_t _bonusCount = 0;
//    int32_t bounsOdds = 0;
//    for (int32_t i = 0; i < GE_WheelChessNum; ++i)
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

uint8_t Matrix_u_checkOnLine(Matrix_u* matrix, uint8_t lineIdx, CheckOnLineResult_t* clr, SlotGameConfig_t* gameConfig) {
    uint8_t nCheckIndex = 0;            // 当前检查下标（保留字段）
    int8_t nFirstAvailChessIndex = -1;  // 第一枚有效普通图标的位置（非 Wild/Bonus/Scatter）
    uint8_t nFirstChessType = 0;        // 第一枚图标类型（保留字段）
    uint8_t nFirstAvailChessType = 0;   // 第一枚有效普通图标类型
    uint8_t nIdPos = 0;

    clr->bIsEliminate = 0;
    clr->bHasWild = 0;

    uint8_t nConWildNum = 0;  // 从左开始连续 Wild 数量
    uint8_t nWildIdx = 0;
    uint8_t BetValue = 0;

    for (uint8_t idIndex = 0; idIndex < gameConfig->header.reelCount; ++idIndex)
    {
        nIdPos = GET_LINE_VALUE(gameConfig->header.id, lineIdx, idIndex);
        //nIdPos = gameConfig->paytable.gLineCheckIDArray[lineIdx * gameConfig->header.reelCount + idIndex];
        clr->posVec[idIndex] = nIdPos;
        clr->chessTypeVec[idIndex] = matrix->dataArray[nIdPos];
      
        if (matrix->dataArray[nIdPos]== gameConfig->header.Wild)
        {
            if (idIndex == nWildIdx) 
            {
                nConWildNum++;
                nWildIdx++;
            }
        }
        else {
            // 找到第一枚普通图标
            if (nFirstAvailChessIndex == -1)
            {
                nFirstAvailChessIndex = nIdPos;
                nFirstAvailChessType = matrix->dataArray[nFirstAvailChessIndex];
            }
        }
    }

    
    // 5 连 Wild 的特殊处理
    if (nConWildNum == 5)
    {
        clr->nAvailChessType = gameConfig->header.Wild;
        clr->nEliminateLineId = lineIdx;
        BetValue = GET_BET_VALUE(gameConfig->header.id, gameConfig->header.Wild, 3 - 2);
        if (BetValue > 0)
        {
            clr->nEliminateNum = 5;
            clr->bHasWild = 1;
            clr->bIsEliminate = 1;
            return 1;
        }
        else {
            return 0;
        }
    }

    // 未找到普通图标，说明该线仅由 Wild/Bonus/Scatter 组成
    if (nFirstAvailChessIndex == -1)
    {
        return 0;
    }

    // 第一枚有效图标是 Bonus/Scatter，不参与线奖判定
    if (nFirstAvailChessType== gameConfig->header.Scatter|| nFirstAvailChessType == gameConfig->header.Bonus) 
    {
        // 仅处理前缀 Wild 的情况
        if (nConWildNum >= 3)
        {
            clr->nAvailChessType = gameConfig->header.Wild;
            clr->nEliminateLineId = lineIdx;
            clr->nEliminateNum = nConWildNum;
            BetValue = GET_BET_VALUE(gameConfig->header.id, gameConfig->header.Wild, nConWildNum - 2);
            if (BetValue > 0)
            {
                clr->bHasWild = 1;
                clr->bIsEliminate = 1;
                return 1;
            }
            else 
            {
                return 0;
            }
        }
        return 0;
    }

    // 统计从左开始与首个有效图标（或 Wild）连续匹配的数量
    int8_t j = 1;
    for (j = 1; j < 5; j++) 
    {
        if (clr->chessTypeVec[j] == gameConfig->header.Scatter || clr->chessTypeVec[j] == gameConfig->header.Bonus)  break;
        if (clr->chessTypeVec[j] == nFirstAvailChessType || clr->chessTypeVec[j]==gameConfig->header.Wild)
        {
            continue;
        }
        else
        {
            break;
        }
    }

    // 至少连中 2 个才有机会构成线奖
    if (j >= 3)
    {
        BetValue = GET_BET_VALUE(gameConfig->header.id, clr->nAvailChessType, j - 2);
        if (BetValue >0)
        {
            clr->bIsEliminate = 1;
        }
    }
    else if (j == 2)
    {
        BetValue = GET_BET_VALUE(gameConfig->header.id, nFirstAvailChessType, 0);
        if (BetValue != 0)
        {
            clr->bIsEliminate = 1;
        }
        else
        {
            clr->bIsEliminate = 0;
            return 0;
        }
    }
    else
    {
        clr->bIsEliminate = 0;
        return 0;
    }

    clr->nAvailChessType = nFirstAvailChessType;
    clr->nEliminateLineId = lineIdx;
    clr->nEliminateNum = j;

    // Wild 及其右侧特殊图标不单独作为普通线奖
    if (nFirstAvailChessType >= gameConfig->header.Wild && nConWildNum < 3)
    {
        clr->bIsEliminate = 0;
    }

    clr->nWildNum = 0;
    if (clr->bIsEliminate)
    {
        if (nConWildNum < 3)
        {
            for (j = 0; j < clr->nEliminateNum; ++j)
            {
                if (clr->chessTypeVec[j]==gameConfig->header.Wild) 
                {
                    clr->bHasWild = 1;
                    clr->nWildNum++;
                }
            }
        }
        else
        {
            // 3 连及以上 Wild 时，比较“按普通图标赔付”和“按 Wild 赔付”，取更优
            int8_t nNormalBet = GET_BET_VALUE(gameConfig->header.id, clr->nAvailChessType, clr->nEliminateNum - 2);
            int8_t nWildBet = GET_BET_VALUE(gameConfig->header.id, gameConfig->header.Wild, nConWildNum - 2);
              
            if (nNormalBet >= nWildBet)
            {
                clr->bHasWild = 1;
            }
            else
            {
                clr->nAvailChessType = gameConfig->header.Wild;
                clr->nEliminateLineId = lineIdx;
                clr->nEliminateNum = nConWildNum;
                clr->bHasWild = 1;
                clr->bIsEliminate = 1;
                return 1;
            }
        }
    }

    return clr->bIsEliminate;
}

int32_t Matrix_u_computerMatrixById(Matrix_u* pMatrix, int32_t* idVec, SlotGameConfig_t* gameConfig, uint32_t gameId)
{
    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        idVec[i] = 0;
    }

    uint32_t nLocalWinBet = 0;
    CheckOnLineResult_t clr;
    uint8_t idVecCount = 0;
    uint32_t nID = 0;

    // 按线逐条计算中奖
    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        CheckOnLineResult_Init(&clr);
        Matrix_u_checkOnLine(pMatrix, i, &clr, gameConfig);

        if (clr.bIsEliminate) 
        {
            if (clr.bHasWild) 
            {
                // 部分玩法中，含 Wild 的线奖需要额外处理
            }

            nLocalWinBet += GET_BET_VALUE(gameConfig->header.id, clr.nAvailChessType, clr.nEliminateNum - 2);

            // ID 编码：千位=线号，百位=连中数量，十位/个位=图标类型
            nID = i  * 1000 + clr.nEliminateNum * 100 + clr.nAvailChessType;
          
            pMatrix->resultType = RT_Win;// RT_Win
            idVec[idVecCount++] = nID;
        }
    }
    pMatrix->idVecSize = idVecCount;

    uint8_t scatterCount = 0;
    uint8_t freeOdds = 0;
    uint8_t _bonusCount = 0;
    uint8_t bounsOdds = 0;
    switch (gameId)
    {
    case 3998:
        // 判定 Bonus
        for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
        {
            if (pMatrix->dataArray[i] == gameConfig->header.Bonus)
            {
                ++_bonusCount;
            }
        }
        if (_bonusCount >= 3)
        {
            // 3998 玩法：Bonus 与 Free 二选一
            if (JRand(0, 10000) < 2000)
            {
                pMatrix->resultType = RT_BonusWin;
            }
            else
            {
                pMatrix->resultType = RT_FreeWin; 
            }
          
            nLocalWinBet += bounsOdds;
        }
     break;
    default:
        // 判定 Scatter（免费游戏）
        for (uint8_t i = 0; i < gameConfig->header.lineCount * gameConfig->header.rowCount; ++i)
        {
            if (pMatrix->dataArray[i] == gameConfig->header.Scatter)
            {
                ++scatterCount;
            }
        }
        if (scatterCount >= 3)
        {
            pMatrix->resultType = RT_FreeWin;// RT_FreeWin
        }

        // 判定 Bonus
        for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
        {
            if (pMatrix->dataArray[i] == gameConfig->header.Bonus)
            {
                ++_bonusCount;
            }
        }
        if (_bonusCount >= 3)
        {
            pMatrix->resultType = RT_BonusWin; //RT_BonusWin
            nLocalWinBet += bounsOdds;
        }
     break;
    }
   
    return nLocalWinBet;
}

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

    // 初始化 Bonus 数据数组
    for (int8_t i = 0; i < GE_WheelChessNum; i++)
    {
        pResult->BonusData[i] = 0;
    }

    pResult->JPType = JT_None;
    pResult->nJPBet = 0;
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
        pResult->IDVec[i] =0;
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

    // 重置 Bonus 数据
    for (int8_t i = 0; i < GE_WheelChessNum; i++) {
        pResult->BonusData[i] = 0;
    }
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
	gDebugControlMode.mode=pDebugMode->mode;
    gDebugControlMode.resType = pDebugMode->resType;
    gDebugControlMode.bonusType = pDebugMode->bonusType;
    gDebugControlMode.jpType = pDebugMode->jpType;

}
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
    pDest->dwFreeGameTotalBet += pSrc->dwFreeGameTotalBet;
    pDest->dwBonusGameTotalBet += pSrc->dwBonusGameTotalBet;
    pDest->dwJackpotTime += pSrc->dwJackpotTime;
    pDest->dwJackpotTotalBet += pSrc->dwJackpotTotalBet;
    pDest->dwFreeGameBetError += pSrc->dwFreeGameBetError;
    pDest->dwNormalWinTime += pSrc->dwNormalWinTime;
    pDest->dwNormalWinTotalBet+= pSrc->dwNormalWinTotalBet;
}
//-------------------------------------------------------------------------------------
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

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwPlayScore\":%lld,", pInfo->dwPlayScore);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwWinScore\":%lld,", pInfo->dwWinScore);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwTotalPlayTime\":%d,", pInfo->dwTotalPlayTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwNormalOpenTime\":%d,", pInfo->dwNormalOpenTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwGiveOpenTime\":%d,", pInfo->dwGiveOpenTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwBonusTime\":%d,", pInfo->dwBonusTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwFreeGameTime\":%d,", pInfo->dwFreeGameTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwLooseTime\":%d,", pInfo->dwLooseTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwFreeGameTotalBet\":%d,", pInfo->dwFreeGameTotalBet);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwBonusGameTotalBet\":%d,", pInfo->dwBonusGameTotalBet);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwJackpotTime\":%d,", pInfo->dwJackpotTime);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwJackpotTotalBet\":%d,", pInfo->dwJackpotTotalBet);
    if (written < 0) return NULL;
    used += (size_t)written;

    written = snprintf(buffer + used, sizeof(buffer) - used, "\"dwFreeGameBetError\":%d", pInfo->dwFreeGameBetError);
    if (written < 0) return NULL;
    used += (size_t)written;

    snprintf(buffer + used, sizeof(buffer) - used, "}");

    int8_t* result = (int8_t*)malloc(strlen(buffer) + 1);
    if (result)
        strcpy((char*)result, buffer);

    return result;
}