#include "NatureAlg.h"

#include "ComputerData.h"
#include "../GameAlgo/common/JRand.h"

void NatureAlg_GenRollTable()
{
    //// 分配临时表内存
    //int32_t** tmpTable = (int32_t**)malloc(5 * sizeof(int32_t*));
    //int32_t* tmpTableSizes = (int32_t*)calloc(5, sizeof(int32_t));
    //int32_t* tmpTableCapacities = (int32_t*)malloc(5 * sizeof(int32_t));

    //// 初始化临时表容量
    //for (int32_t i = 0; i < 5; i++) {
    //    tmpTableCapacities[i] = 100; // 初始容量
    //    tmpTable[i] = (int32_t*)malloc(tmpTableCapacities[i] * sizeof(int32_t));
    //}

    //// 生成滚轮表
    //for (int32_t i = 0; i < GE_ChessTypeNum; ++i)
    //{
    //    for (int32_t j = 0; j < 5; ++j)
    //    {
    //        // 检查是否需要扩容
    //        if (tmpTableSizes[j] + SymbolDis[i][j] >= tmpTableCapacities[j]) {
    //            tmpTableCapacities[j] *= 2;
    //            tmpTable[j] = (int32_t*)realloc(tmpTable[j], tmpTableCapacities[j] * sizeof(int32_t));
    //        }

    //        // 添加符号
    //        for (int32_t k = 0; k < SymbolDis[i][j]; ++k) {
    //            tmpTable[j][tmpTableSizes[j]++] = i;
    //        }
    //    }
    //}

    // 先清理原有的 RollTable
    //NatureAlg_Cleanup(alg);
    // alg->RollTable = (int32_t**)malloc(5 * sizeof(int32_t*));

    //// 随机打乱并填充 RollTable
    //for (int32_t i = 0; i < 5; ++i)
    //{
    //    alg->RollTableSizes[i] = tmpTableSizes[i];
    //    alg->RollTable[i] = (int32_t*)malloc(tmpTableSizes[i] * sizeof(int32_t));
    //    int32_t remaining = tmpTableSizes[i];

    //    for (int32_t j = 0; j < tmpTableSizes[i]; ++j)
    //    {
    //        int32_t rndNum = JRandFrom(0, remaining);
    //        int32_t nNum = tmpTable[i][rndNum];
    //        assert(nNum >= 0 && nNum < GE_ChessTypeNum);
    //        alg->RollTable[i][j] = nNum;

    //        // 移除已选元素
    //        for (int32_t k = rndNum; k < remaining - 1; ++k) {
    //            tmpTable[i][k] = tmpTable[i][k + 1];
    //        }
    //        remaining--;
    //    }
    //}

    //// 打印结果（调试用）
    //printf("RollTableArray[5][] = {\n");
    //for (int32_t i = 0; i < 5; ++i)
    //{
    //    printf("int32_t r%d[] = {", i);
    //    for (int32_t j = 0; j < alg->RollTableSizes[i]; ++j)
    //    {
    //        printf("%d", alg->RollTable[i][j]);
    //        if (j != alg->RollTableSizes[i] - 1) printf(" ,");
    //    }
    //    printf("};\n");
    //}
    //printf("}");

}

void NatureAlg_GenRndMxu(int8_t id, Matrix_u* mxu)
{
    int8_t rt_size = 0;
    int8_t rndNum = 0;
    int8_t r0 = 0;
    int8_t r1 = 0;
    int8_t r2 = 0;
    int8_t num = 0;
    for (int8_t i = 0; i < COL_MAX; ++i)
    {
        rt_size = GET_ROLLTABLE_SIZE(id,i);
        rndNum = JRandFrom(0, rt_size - 1);
         r0 = rndNum - 1;
        if (r0 < 0) r0 = rt_size - 1;
         r1 = rndNum;
         r2 = rndNum + 1;
        if (r2 >= rt_size) r2 = 0;
     
        num = 0;
        //第一行
        mxu->dataArray[i] = GET_ROLL_VALUE(id,i,r2) ;
        num = mxu->dataArray[i];
        if (num < 0)
        {
            return;
        }
        //第二行
        mxu->dataArray[5 + i] = GET_ROLL_VALUE(id, i, r1);
        num = mxu->dataArray[5 + i];
      
        if (num < 0)
        {
            return;
        }
        //第三行
        mxu->dataArray[10 + i] = GET_ROLL_VALUE(id, i, r0);
        num = mxu->dataArray[10 + i];
        if (num < 0)
        {
            return;
        }
    }
}
