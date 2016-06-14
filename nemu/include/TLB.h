#ifndef __TLB_H_
#define __TLB_H_

#include "nemu.h"

#define TLB_SIZE 64

typedef struct TranslationLookasideBuffer{
	bool valid;
	lnaddr_t tag;	//vpn, 20 prefix bit of lnaddr
	hwaddr_t ppn;	//need (ppn<<12) + offset when used
} TLB;



#endif
