#include "trap.h"

int is_leap_year(int n) {
	return (n % 4 == 0 && n % 100 != 0) || (n % 400 == 0);
}

int ans[] = {0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0};

#define NR_DATA (sizeof(ans) / sizeof(ans[0]))

int main() {
	int i;
	for(i = 0; i < NR_DATA; i ++) {
		nemu_assert(is_leap_year(i + 1890) == ans[i]);
			
			//set_bp();
	}

	nemu_assert(i == NR_DATA);

	HIT_GOOD_TRAP;

	return 0;
}
