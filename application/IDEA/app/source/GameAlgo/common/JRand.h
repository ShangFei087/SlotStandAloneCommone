#ifndef   JRAND_H
#define   JRAND_H

#ifdef _DEBUG
#define _USE_SFMT 1
#endif//_DEBUG
#if _USE_SFMT
#include "SFMT.h"
#endif//_USE_SFMT

#include "qs.h"

typedef unsigned int U32;

//20240114 使用最新版随机数SFMT-src-1.5.1
/*
*@功能:获得随机数，32位无整型,使用前必须初始化种子
*@返回值：返回随机数
*/
U32  JRand( void );

uint16_t JRandExt(void);
/*
*@功能:获得区间[Min,Max]随机数，32位无整型
*@返回值：返回随机数
*/
U32 JRandFrom(U32  Min, U32  Max);
/*
*@功能:初始化随机因子
*/
void     JSrand( U32 seed );

void	 JSrandArray(U32* seed, U32 len);
/*
*@功能:获得数值符号
*@参数:x
*/
int GetValueF( float x  );
/*
*@功能:-1至1之间随机浮点数
*/
double GetRandf( void );


/**
 *  @brief	 返回[0,1]之间随机数double
 */
double GetRandf01( void );

#endif//JRAND_H