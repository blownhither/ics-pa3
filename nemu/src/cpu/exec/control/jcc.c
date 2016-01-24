#include "cpu/exec/helper.h"

#define DATA_BYTE 1
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jcc-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jcc-template.h"
#undef DATA_BYTE


make_helper_v(jo_si) //0x0f 80
make_helper_v(jno_si) //0x0f 81
make_helper_v(jb_si) //0x0f 82
make_helper_v(jnb_si) //0x0f 83
make_helper_v(je_si) //0x0f 84
make_helper_v(jne_si) //0x0f 85
make_helper_v(jna_si) //0x0f 86
make_helper_v(ja_si) //0x0f 87
make_helper_v(js_si) //0x0f 88
make_helper_v(jns_si) //0x0f 89
make_helper_v(jp_si) //0x0f 8a
make_helper_v(jpo_si) //0x0f 8b
make_helper_v(jl_si) //0x0f 8c
make_helper_v(jge_si) //0x0f 8d
make_helper_v(jle_si) //0x0f 8e
make_helper_v(jg_si) //0x0f 8f
