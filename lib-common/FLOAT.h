#ifndef __FLOAT_H__
#define __FLOAT_H__

#include "trap.h"


typedef int FLOAT;
static inline int F2int(FLOAT a) {
	int ret = ((a>>16)&0xffff) | ((a>>31)?0xffff0000:0);//always round lower
	ret += (a>>15)&1;//round by 0.5
#ifdef MZYDEBUG
	printf("F2int(FLOAT 0x%x): 0x%x\n",a,ret);
#endif
	return ret;
}

static inline FLOAT int2F(int a) {
	FLOAT ret = a<<16; 	//=== (a<<16) | (a&0x80000000);
	nemu_assert((a>>15)==0||(a>>15)==0xffffffff);	
	//the first 17 bits must be same to avoid overflow
#ifdef MZYDEBUG
	printf("int2F(int 0x%x): 0x%x\n",a,ret);
#endif	
	return ret;
}

static inline FLOAT F_mul_int(FLOAT a, int b) {
	FLOAT ret = a*b;
#ifdef MZYDEBUG
	printf("F_mul_int(FLOAT 0x%x, int 0x%x): 0x%x\n",a,b,ret);
#endif
	return ret;
}

static inline FLOAT F_div_int(FLOAT a, int b) {
	FLOAT ret = a/b;
#ifdef MZYDEBUG
	printf("F_mul_int(FLOAT 0x%x, int 0x%x): 0x%x\n",a,b,ret);
#endif
	return ret;
}

FLOAT f2F(float);
FLOAT F_mul_F(FLOAT, FLOAT);
FLOAT F_div_F(FLOAT, FLOAT);
FLOAT Fabs(FLOAT);
FLOAT sqrt(FLOAT);
FLOAT pow(FLOAT, FLOAT);

#endif//?0x
