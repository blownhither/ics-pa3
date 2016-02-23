#include "FLOAT.h"

typedef long long uint64_t;
FLOAT F_mul_F(FLOAT a, FLOAT b) {
	FLOAT ret = ((uint64_t)a*b)>>16;	//TODO:check
#ifdef MZYDEBUG
	printf("F_mul_F(FLOAT 0x%x, FLOAT 0x%x): 0x%x\n",a,b,ret);
#endif
	return ret ;
}

FLOAT F_div_F(FLOAT a, FLOAT b) {
	FLOAT ret = (a/b)<16;			//TODO:check
	return 0;
}
//askdasdasdhios
FLOAT f2F(float a) {
	 
	return 0;
}

FLOAT Fabs(FLOAT a) {
	FLOAT ret = (a>>31)? -a : a;		//TODO:check
	return 0;
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

