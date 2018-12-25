#include "rhopi.h"

static const int piln[24] = {
	10, 7,  11, 17, 18, 3, 5,  16, 8,  21, 24, 4,
	15, 23, 19, 13, 12, 2, 20, 14, 22, 9,  6,  1
};

static const uint8_t rotc[24] = {
	1,  3,  6,  10, 15, 21, 28, 36, 45, 55, 2,  14,
	27, 41, 56, 8,  25, 43, 62, 18, 39, 61, 20, 44
};

uint64_t rotatel(uint64_t t, uint8_t rot) {
	return (t << rot) | (t >> (sizeof(t) * 8 - rot));
}
uint64_t rotater(uint64_t t, uint8_t rot) {
	return (t >> rot) | (t << (sizeof(t) * 8 - rot));
}

void keccakRhoPi(ethhash &hash) {
	uint64_t bc;
	uint64_t t = hash.q[1]; // hash.q[piln[23]]
	for (int i = 0; i < 24; i++) {
		int j = piln[i];
		bc = hash.q[j];
		hash.q[j] = rotatel(t, rotc[i]);
		t = bc;
	}
}
void inverseRhoPi(ethhash &hash) {
	uint64_t bc = hash.q[piln[0]];
	for (int i = 0; i < 23; i++) {
		hash.q[piln[i]] = rotater(hash.q[piln[i+1]], rotc[i+1]);
	}
	hash.q[piln[23]] = rotater(bc, rotc[0]);
}


