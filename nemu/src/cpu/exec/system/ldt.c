#include "cpu/exec/helper.h"
 /*
#define DATA_BYTE 1
#include "ldt-template.h"
#undef DATA_BYTE
*/
#define DATA_BYTE 2
#include "ldt-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "ldt-template.h"
#undef DATA_BYTE

make_helper_v(lidt_rm)
make_helper_v(lgdt_rm)
