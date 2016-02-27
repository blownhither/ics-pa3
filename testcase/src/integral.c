#include "trap.h"
#include "FLOAT.h"

FLOAT f(FLOAT x) { 
	/* f(x) = 1/(1+25x^2) */
	FLOAT ret = F_div_F(int2F(1), int2F(1) + F_mul_int(F_mul_F(x, x), 25));
	set_bp();
	return ret;
}

FLOAT computeT(int n, FLOAT a, FLOAT b, FLOAT (*fun)(FLOAT)) {
	int k;
	FLOAT s,h;
	h = F_div_int((b - a), n);		//0.2
	set_bp();
	s = F_div_int(fun(a) + fun(b), 2 );	//0.038461538 ==1/26
	set_bp();
	nemu_assert(Fabs(f2F(0.038461)-s)<f2F(1e-4));
	set_bp();
	for(k = 1; k < n; k ++) {
		s += fun(a + F_mul_int(h, k));
	}
	set_bp();
	s = F_mul_F(s, h);
	set_bp();
	return s;
}

int main() { 
	FLOAT a = computeT(10, f2F(-1.0), f2F(1.0), f);
	set_bp();
	FLOAT ans = f2F(0.551222);
	set_bp();
	nemu_assert(Fabs(a - ans) < f2F(1e-4));
	HIT_GOOD_TRAP;
	return 0;
}
