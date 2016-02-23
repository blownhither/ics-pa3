#include "trap.h"
#include "FLOAT.h"

int main(){
	FLOAT a = f2F(2.0);
	int b = F2int(a);
	nemu_assert(2==b);
	HIT_GOOD_TRAP;
	return 0;
}
