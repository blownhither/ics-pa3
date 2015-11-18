#include "cpu/exec/helper.h"
 
/*
#define DATA_BYTE 1
#include "sub-template.h"
#undef DATA_BYTE
*/
//TODO: implement 1
#define DATA_BYTE 2
#include "sub-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "sub-template.h"
#undef DATA_BYTE


make_helper_v(sub_si2rm)
/*int mov_i2r_v(swaddr_t eip) {
 *	return (ops_decoded.is_data_size_16 ? mov_i2r_w : mov_i2r_l) (eip);  \
}*/


/*
make_helper_v(sub_i2rm)
make_helper_v(sub_i2r)
make_helper_v(sub_r2rm)
make_helper_v(sub_rm2r)
*/
