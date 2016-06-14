#ifndef __IRQ_H__
#define __IRQ_H__

#include "common.h"

/* TODO: The decleration order of the members in the ``TrapFrame''
 * structure below is wrong. Please re-orgainize it for the C
 * code to use the trap frame correctly.
 */


/*
typedef struct TrapFrame {
	uint32_t esi, ebx, eax, eip, edx, error_code, eflags, ecx, cs, old_esp, edi, ebp;
	int32_t irq;
} TrapFrame;
*/

typedef struct TrapFrame {
	uint32_t edi,		/*pusha*/
		esi,
		ebp,
		old_esp,
		ebx,
		edx,
		ecx,
		eax;
	int32_t irq;		/*push*/
	uint32_t error_code,/*push*/
		eip,			/*int*/
		cs,
		eflags;
} TrapFrame;

int fs_open(const char *pathname, int flags);	/* 在我们的实现中可以忽略flags */
int fs_read(int fd, void *buf, int len);
int fs_write(int fd, void *buf, int len);
int fs_lseek(int fd, int offset, int whence);
int fs_close(int fd);
 
#endif
