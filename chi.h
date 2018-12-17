#include "ethhash.h"
// inverse chi for 5 individual bits
static uint8_t chi[32] = {
	0, 11, 22, 9, 13, 4, 18, 15,
	26, 1, 8, 3, 5, 12, 30, 7,
	21, 20, 2, 23, 16, 17, 6, 19,
	10, 27, 24, 25, 29, 28, 14, 31,
};
void inverseChi(ethhash &hash);
void keccakChi(ethhash &hash);
