#include "theta.h"
#include "rhopi.h"
#include "chi.h"
#include "iota.h"

#include <assert.h>
#include <iostream>
using std::hex;
using std::cout;
using std::endl;

void keccak(ethhash &hash) {
	hash.d[33] ^= 0x80000000;
	for (int r = 0; r < 24; r++) {
		// Theta
		keccakTheta(hash);
		// RhoPi
		keccakRhoPi(hash);
		// Chi
		keccakChi(hash);
		// IOTA
		hash.q[0] ^= rndc[r];
	}
}

int test_empty() {
	ethhash hash;
	bzero(&hash, sizeof(hash));
	hash.b[0] ^= 0x01; // length 0

	keccak(hash);

	// 0xc5d2460186f7233c927e7db2dcc703c0e500b653ca82273b7bfad8045d85a470
	ethhash expected;
	expected.b[0] = 0xc5;
	expected.b[1] = 0xd2;
	expected.b[2] = 0x46;
	expected.b[3] = 0x01;
	expected.b[4] = 0x86;
	expected.b[5] = 0xf7;
	expected.b[6] = 0x23;
	expected.b[7] = 0x3c;
	expected.b[8] = 0x92;
	expected.b[9] = 0x7e;
	expected.b[10] = 0x7d;
	expected.b[11] = 0xb2;
	expected.b[12] = 0xdc;
	expected.b[13] = 0xc7;
	expected.b[14] = 0x03;
	expected.b[15] = 0xc0;
	expected.b[16] = 0xe5;
	expected.b[17] = 0x00;
	expected.b[18] = 0xb6;
	expected.b[19] = 0x53;
	expected.b[20] = 0xca;
	expected.b[21] = 0x82;
	expected.b[22] = 0x27;
	expected.b[23] = 0x3b;
	expected.b[24] = 0x7b;
	expected.b[25] = 0xfa;
	expected.b[26] = 0xd8;
	expected.b[27] = 0x04;
	expected.b[28] = 0x5d;
	expected.b[29] = 0x85;
	expected.b[30] = 0xa4;
	expected.b[31] = 0x70;
	for (int i = 0; i < 32; i++) {
		assert(hash.b[i] == expected.b[i]);
	}
	return 0;
}

int main() {
	test_empty();
	return 0;
}
