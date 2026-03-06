/**
* @file
* @author  Huang Wen <Email:ww1383@163.com, QQ:214890094, WeChat:w18926268887>
* @version 1.0
*
* @section LICENSE
*
* Permission is hereby granted, free of charge, to any person obtaining a
* copy of this software and associated documentation files (the "Software"),
* to deal in the Software without restriction, including without limitation
* the rights to use, copy, modify, merge, publish, distribute, sublicense,
* and/or sell copies of the Software, and to permit persons to whom the
* Software is furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included
* in all copies or substantial portions of the Software.
*
* @section DESCRIPTION
*
* This file is ...
*/
#ifndef _QS_PRIME_H_
#define _QS_PRIME_H_


#ifdef __cplusplus
extern "C" {
#endif


/* Generates a probable prime a between b and c such that a-1 is
	divisible by d.

	Assumes b < c, digits < MAX_NN_DIGITS.

	Returns RE_NEED_RANDOM if randomStruct not seeded, RE_DATA if
	unsuccessful.
*/

BOOL
qs_prime_generate(
	NN_DIGIT *a,
	NN_DIGIT *b,
	NN_DIGIT *c,
	NN_DIGIT *d,
	uint32_t  digits
);

#ifdef __cplusplus
}
#endif //#ifdef __cplusplus

#endif //#ifndef _QS_PRIME_H_
