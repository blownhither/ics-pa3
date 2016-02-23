#include "trap.h"
#include "FLOAT.h"

int main(){
	FLOAT a = int2F(-15);
	int b = F2int(a);
	nemu_assert(-15==b);
	HIT_GOOD_TRAP;
	return 0;
}
