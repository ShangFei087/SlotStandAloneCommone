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
        return; // 或者错误处理
    }
    // 复制整个结构体，而不仅仅是 dataArray
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
//// ChessLine_t 结构用于替代 std::map
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
//// 简单的映射表实现
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
//    // 清空输出数组
//    for (int32_t i = 0; i < GE_MaxIDNum; ++i) idVec[i] = 0;
//    int32_t idVecCount = 0;
//    int32_t nLocalWinBet = 0;
//
//    // 创建映射表
//    ChessLineMap* chessLineMap = (ChessLineMap*)malloc(sizeof(ChessLineMap));
//
//    ChessLineMap_init(&chessLineMap, GE_ChessTypeNum); // 假设最多12种不同的棋子
//
//    //判断普通中奖
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
//    //如果第一列有Wild,增加中奖图标
//    if (iswild)
//    {
//        //从第二列遍历
//        for (int32_t c = 1; c < GE_ColNum; ++c)
//        {
//            //遍历每列的图标
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
//    // 检查后面每一列是否有对应id
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
//                // 对映射表中的每个普通图标增加计数
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
//    // 计算赔率
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
//    //判断免费中奖
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
//    //判断大奖中奖
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
    uint8_t nCheckIndex = 0;            //线上当前要检测的索引
    int8_t nFirstAvailChessIndex = -1; //本条线上的第一个有效的ID的索引位置,普通图标,不是Wild,Bonus,Scatter
    uint8_t nFirstChessType = 0;        //本条线上的第一个ID值
    uint8_t nFirstAvailChessType = 0;   //本条线上第一个有效的ID,普通图标,不是Wild,Bonus,Scatter
    uint8_t nIdPos = 0;

    clr->bIsEliminate = 0;
    clr->bHasWild = 0;

    uint8_t nConWildNum = 0;  //连续的wildNum
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
            //找到第一个普通图标
            if (nFirstAvailChessIndex == -1)
            {
                nFirstAvailChessIndex = nIdPos;
                nFirstAvailChessType = matrix->dataArray[nFirstAvailChessIndex];
            }
        }
    }

    
    //如果全是wild
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

    //未找到第一个普通图标，表明该线全是wild，bonus或scatter
    if (nFirstAvailChessIndex == -1)
    {
        return 0;
    }

    //第一个图标为Bonus或者Scatter则本线无消除
    if (nFirstAvailChessType== gameConfig->header.Scatter|| nFirstAvailChessType == gameConfig->header.Bonus) 
    {
        //除了scatter，bonus都是wild
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

    //查找后续图标中有多少个图标是同首个非wild图标同样的类型或者为wild类型
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

    //三个以上必定中奖，两个看是否赔率中有中奖 //不一定
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

    //超过CT_Wild类型的图标单独算奖
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
            //出现3个以上连续的wild，比较看是算wild赔率大还是连同普通图标计算赔率大，选择较大的一个
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

    //阵列同15条线一次对比
    for (uint8_t i = 0; i < gameConfig->header.lineCount; ++i)
    {
        CheckOnLineResult_Init(&clr);
        Matrix_u_checkOnLine(pMatrix, i, &clr, gameConfig);

        if (clr.bIsEliminate) 
        {
            if (clr.bHasWild) 
            {
                // 有些游戏消除线中有wild需要做翻倍等特殊处理
            }

            nLocalWinBet += GET_BET_VALUE(gameConfig->header.id, clr.nAvailChessType, clr.nEliminateNum - 2);

            //ID的万位千位表示所在线，百位表示消除数目，十个位表示Type
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
        //判断大奖中奖
        for (uint8_t i = 0; i < GE_WheelChessNum; ++i)
        {
            if (pMatrix->dataArray[i] == gameConfig->header.Bonus)
            {
                ++_bonusCount;
            }
        }
        if (_bonusCount >= 3)
        {
            //五分之一中免费奖
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
        //判断免费中奖
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

        //判断大奖中奖
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
        // 可以添加错误处理或直接返回
        return;
    }

    // 初始化枚举类型
    pResult->openType = OT_Normal;      // 默认为普通开奖
    pResult->resType = RT_Lose;         // 默认为输

    // 初始化ID数组
    for (int8_t i = 0; i < GE_MaxIDNum; i++) {
        pResult->IDVec[i] = 0;         // 使用-1表示无效ID
    }

    // 初始化赔率
    pResult->nMatrixBet = 0;            // 阵列赔率为0
    pResult->nTotalFreeBet = 0;         // 免费游戏总赔率
    pResult->nTotalFreeTime = 0;        // 免费游戏次数
    pResult->nBonusBet = 0;             // 彩金赔率
    pResult->nBonusType = 0;            // 彩金类型
    pResult->BlindSymbol = 0;            // 彩金类型

    // 初始化阵列
    Matrix_u_reset(&pResult->matrix);   // 调用Matrix_u的reset函数

    // 初始化免费游戏赔率数组
    for (int8_t i = 0; i < GE_MaxFreeNum; i++) {
        pResult->FreeBetArray[i] = 0;   // 初始化为0
    }

    // 初始化大奖数据数组
    for (int8_t i = 0; i < GE_WheelChessNum; i++)
    {
        pResult->BonusData[i] = 0;      // 初始化为0
    }

    pResult->JPType = JT_None;
    pResult->nJPBet = 0;
}
void OutResult_reset(OutResult_t* pResult)
{
    if (pResult == NULL) {
        return;
    }

    // 重置枚举类型为默认值
    pResult->openType = OT_Normal;
    pResult->resType = RT_Lose;

    // 重置ID数组
    for (int8_t i = 0; i < GE_MaxIDNum; i++) {
        pResult->IDVec[i] =0;
    }

    // 重置所有赔率为0
    pResult->nMatrixBet = 0;
    pResult->nTotalFreeBet = 0;
    pResult->nTotalFreeTime = 0;
    pResult->nBonusBet = 0;
    pResult->nBonusType = 0;
    pResult->BlindSymbol = 0;            // 彩金类型

    // 重置阵列
    Matrix_u_reset(&pResult->matrix);

    // 重置免费游戏赔率数组
    for (int8_t i = 0; i < GE_MaxFreeNum; i++) {
        pResult->FreeBetArray[i] = 0;
    }

    // 重置彩金数据数组
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
    pDest->dwFreeGameBetError += pSrc->dwFreeGameBetError;
    pDest->dwNormalWinTime += pSrc->dwNormalWinTime;
    pDest->dwNormalWinTotalBet+= pSrc->dwNormalWinTotalBet;
}
//-------------------------------------------------------------------------------------
int8_t* DebugInfo_ArrayToString(int32_t* pArray, int32_t length, BOOL keepZero)
{
    int8_t buffer[1024] = "[";
    int8_t temp[32];

    for (int32_t i = 0; i < length; ++i)
    {
        if (keepZero || pArray[i] != 0)
        {
            sprintf(temp, "%d,", pArray[i]);
            strcat(buffer, temp);
        }
    }

    int32_t len = strlen(buffer);
    if (buffer[len - 1] == ',')
        buffer[len - 1] = ']';
    else
        strcat(buffer, "]");

    int8_t* result = (int8_t*)malloc(strlen(buffer) + 1);
    if (result)
        strcpy(result, buffer);
    free(result);
    return result;
}
int8_t* DebugInfo_toJason(DebugInfo* pInfo)
{
    int8_t buffer[2048];
    int8_t temp[256];

    strcpy(buffer, "{");

    sprintf(temp, "\"dwPlayScore\":%lld,", pInfo->dwPlayScore);
    strcat(buffer, temp);

    sprintf(temp, "\"dwWinScore\":%lld,", pInfo->dwWinScore);
    strcat(buffer, temp);

    sprintf(temp, "\"dwTotalPlayTime\":%d,", pInfo->dwTotalPlayTime);
    strcat(buffer, temp);

    sprintf(temp, "\"dwNormalOpenTime\":%d,", pInfo->dwNormalOpenTime);
    strcat(buffer, temp);

    sprintf(temp, "\"dwGiveOpenTime\":%d,", pInfo->dwGiveOpenTime);
    strcat(buffer, temp);

    sprintf(temp, "\"dwBonusTime\":%d,", pInfo->dwBonusTime);
    strcat(buffer, temp);

    sprintf(temp, "\"dwFreeGameTime\":%d,", pInfo->dwFreeGameTime);
    strcat(buffer, temp);

    sprintf(temp, "\"dwLooseTime\":%d,", pInfo->dwLooseTime);
    strcat(buffer, temp);

    sprintf(temp, "\"dwFreeGameTotalBet\":%d,", pInfo->dwFreeGameTotalBet);
    strcat(buffer, temp);

    sprintf(temp, "\"dwBonusGameTotalBet\":%d,", pInfo->dwBonusGameTotalBet);
    strcat(buffer, temp);

    sprintf(temp, "\"dwFreeGameBetError\":%d", pInfo->dwFreeGameBetError);
    strcat(buffer, temp);

    strcat(buffer, "}");

    int8_t* result = (int8_t*)malloc(strlen(buffer) + 1);
    if (result)
        strcpy(result, buffer);

    return result;
}