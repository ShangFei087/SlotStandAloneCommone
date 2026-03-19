#include "NatureAlg.h"

#include "ComputerData.h"
#include "../GameAlgo/common/JRand.h"

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
