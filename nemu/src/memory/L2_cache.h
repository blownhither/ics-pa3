#ifndef __H_L2_CACHE_
#define __H_L2_CACHE_

#ifndef FORCE_L2_CACHE_CONFIG

#define BLOCK_LEN 6
#define ASSOCT_WAY 16
#define INDEX_LEN 12
#define TAG_LEN 9
//TAG_LEN = 27 - INDEX_LEN - BLOCK_LEN

#define BLOCK_SIZE (1<<BLOCK_LEN)
#define GROUP_NUM (1<<INDEX_LEN)
/*
#if LINE_SIZE<=8
#define LINE_TYPE uint8_t
#elif LINE_SIZE<=16
#define LINE_SIZE uint16_t
#else
#define LINE_SIZE uint32_t
#endif
*/
#endif


#endif
