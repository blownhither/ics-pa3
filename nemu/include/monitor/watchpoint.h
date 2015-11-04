#ifndef __WATCHPOINT_H__
#define __WATCHPOINT_H__

#include "common.h"

typedef struct watchpoint {
	int NO;
	struct watchpoint *next , *last;
	char expr[256]; 
	uint32_t old_value; 
	/* TODO: Add more members if necessary */


} WP;

#endif
