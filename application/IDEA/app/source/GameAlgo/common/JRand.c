#include "JRand.h"
//#include "SimpleConfig.h" //包含全局变量，主要是后台设置

//在机器中需要使用机器的硬件随机数。
#include <math.h>
#if _USE_SFMT
//20240114 使用最新版随机数SFMT-src-1.5.1
sfmt_t sfmt;//内部状态，全局变量
/*
*@功能:获得随机数，32位无整型,使用前必须初始化种子
*@返回值：返回随机数
*/
U32  JRand(void)
{
	U32 Number = sfmt_genrand_uint32(&sfmt);
	return Number;
}

uint16_t JRandExt(void)
{
	return JRand();
}
/*
*@功能:获得区间[Min,Max]随机数，32位无整型
*@返回值：返回随机数
*/
U32 JRandFrom(U32  Min, U32  Max)
{
	/*
	* double myRand = rand()/(1.0 + RAND_MAX); 
    int range = rangeHigh - rangeLow + 1;
    int myRand_scaled = (myRand * range) + rangeLow;
    return myRand_scaled;
	*/
	U32  Number = 0;
	
	Number = JRand() % (Max - Min + 1) + Min;
	return  Number;
}
/*
*@功能:初始化随机因子
*/
void JSrand(U32 seed)
{
	sfmt_init_gen_rand(&sfmt, seed);
}

void JSrandArray(U32* seed, U32 len) {
	sfmt_init_by_array(&sfmt, seed, len);
}
/*
*@功能:获得[-1 1]之间的随机数，
*@返回值：返回随机数
*/
double GetRandf(void)
{
	return sfmt_genrand_real1(&sfmt) * 2 - 1;
}

/* generates a random number on [0,1]-real-interval */
/*
 *  @brief	 返回[0,1]之间随机数double
 */
double  GetRandf01(void)
{
	double Number;
	Number = sfmt_genrand_real1(&sfmt);
	return  Number;// 在[0, 1] - real - interval 上生成一个随机数。
}
#else
/*
*@功能:获得随机数，32位无整型,使用前必须初始化种子
*@返回值：返回随机数
*/
U32  JRand(void)
{
	U32 Number;
	qs_rand32(&Number, 1);
	return Number;
}

uint16_t JRandExt(void)
{
	uint16_t rnd;
	qs_rand16(&rnd, 1);

	return rnd;
}
/*
*@功能:获得区间[Min,Max]随机数，32位无整型
*@返回值：返回随机数
*/
U32 JRandFrom(U32  Min, U32  Max)
{
	U32  Number = 0;
	Number = JRand() % (Max - Min + 1) + Min;
	return  Number;
}

/*
*@功能:初始化随机因子
*/
void JSrand(U32 seed)
{
	//Do Nothing
}

void JSrandArray(U32* seed, U32 len) {
	//Do Nothing
}
/*
*@功能:获得[-1 1]之间的随机数，
*@返回值：返回随机数
*/
double GetRandf(void)
{
	double sf = GetRandf01();
	return sf * 2 - 1;
}

/* generates a random number on [0,1]-real-interval */
/*
 *  @brief	 返回[0,1]之间随机数double
 */
double  GetRandf01(void)
{
	double Number;
	uint16_t sf;
	uint16_t xNumber;
	qs_rand16(&xNumber, 1);
	sf = xNumber & 0x7fff;	//去掉最高位的符号
	Number = (double)sf / (double)0x7fff;
	return  Number;// 在[0, 1] - real - interval 上生成一个随机数。
}

#endif//_USE_SFMT
/*
*@功能:获得数值符号
*@参数:x
*/
 int GetValueF(float x)
{
	if (x > 0)
		return 1;
	else if (x < 0)
		return -1;
	else
		return 0;
}