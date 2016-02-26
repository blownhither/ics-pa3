#include "trap.h"
#include "FLOAT.h"
FLOAT a,c;
int b;
int main(){
//	for(c=-(0x8000);c<0x8000;++c){
//		nemu_assert(c==(F2int(int2F(c))));
//	}
//	set_bp();
	a = f2F(2.0);
	nemu_assert((int2F(2)-a)<10);
	set_bp();
	b = F2int(a);
	nemu_assert(2==b);

	HIT_GOOD_TRAP;
	return 0;
}
