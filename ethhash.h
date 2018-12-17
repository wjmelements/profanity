#ifndef ethhash_h
#define ethhash_h

#include "stdint.h"
typedef union {
	uint8_t b[200];
	uint64_t q[25];
	uint32_t d[50];
} ethhash;


#endif // ethhash_h
