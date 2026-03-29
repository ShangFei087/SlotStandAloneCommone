#include "NatureAlg.h"
#include "ComputerData.h"

void NatureAlg_GenRndMxu(int8_t id, Matrix_u* mxu, uint8_t rowCount)
{
	uint8_t col;
	uint8_t r;
	uint8_t maxRows;
	uint8_t tableIdx;

	if (mxu == NULL)
		return;

	Matrix_u_reset(mxu);

	maxRows = (uint8_t)(GE_WheelChessMaxNum / COL_MAX);
	if (rowCount < 1)
		rowCount = 1;
	else if (rowCount > maxRows)
		rowCount = maxRows;

	if (id < 0 || id >= (int8_t)RollTabel_MAX)
		return;

	tableIdx = (uint8_t)id;

	for (col = 0; col < COL_MAX; ++col)
	{
		uint8_t len = GET_ROLLTABLE_SIZE(tableIdx, col);
		uint8_t stop;
		uint8_t stripIdx;

		if (len == 0)
			continue;

		stop = JRandFrom(0,len - 1);
		for (r = 0; r < rowCount; ++r)
		{
			stripIdx = (uint8_t)((stop + r) % len);
			mxu->dataArray[r * COL_MAX + col] = GET_ROLL_VALUE(tableIdx, col, stripIdx);
		}
	}
}
