#include "FLOAT.h"

FLOAT F_mul_F(FLOAT a, FLOAT b) {
	uint8_t sa = (a>>31)&1, sb = (b>>31)&1;	
	if(sa) a=-a;
	if(sb) b=-b;
	FLOAT ret = ((uint64_t)a*b)>>16;	
#ifdef MZYDEBUG
	printf("F_mul_F(FLOAT 0x%x, FLOAT 0x%x): 0x%x\n",a,b,ret);
#endif
	return (sa^sb)? -ret : ret ;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	uint8_t sa = (a>>31)&1, sb = (b>>31)&1;
	if(sa) a=-a;
	if(sb) b=-b;
	uint64_t bin = ((uint64_t)a)<<16;
/*
	FLOAT ret = 0;	
	while(bin>b){	//TODO: ...
		bin -= b;
		ret ++;
	}
	if(bin>(b>>1))
		ret ++;
	if(sa^sb)
		ret = -ret;
	return ret;
*/
	/*	using binary search	*/
	FLOAT l = 0, r = 0x80000000-1, mid = (l+r)/2;
	while(l<r) {
		if(((uint64_t)mid)*b == bin) break;
		else if((uint64_t)mid*b < bin)
			l = mid + 1;
		else
			r = mid - 1;
		mid = (l+r)/2;
	}
	if(sa^sb)
		mid = -mid;
	return mid;

}
	
FLOAT f2F(float a) {
	uint32_t bin = *(uint32_t *)&a;
	uint32_t manti = bin & 0x7fffff;
	uint8_t exp = (bin>>23)&0xff, sign = (bin>>31)&1;
	FLOAT ret;
	nemu_assert(exp!=0xff);		//NaN, inf, -inf
	if(exp>0) {
		nemu_assert(exp <= 142);//overflow
		if(134-exp>=0)
			ret = (manti | 0x800000) >> (-exp+134);
		else
			ret = (manti | 0x800000) << (exp-134);
	}
	else {
		ret = 0;		//Denormalized
	}
	if(sign)
		ret = -ret;
	return ret;
}

FLOAT Fabs(FLOAT a) {
	FLOAT ret = (a>>31)? -a : a;		
	return ret;
}

FLOAT sqrt(FLOAT x) {
	FLOAT dt, t = int2F(2);

	do {
		dt = F_div_int((F_div_F(x, t) - t), 2);
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

FLOAT pow(FLOAT x, FLOAT y) {
	/* we only compute x^0.333 */
	FLOAT t2, dt, t = int2F(2);

	do {
		t2 = F_mul_F(t, t);
		dt = (F_div_F(x, t2) - t) / 3;
		t += dt;
	} while(Fabs(dt) > f2F(1e-4));

	return t;
}

