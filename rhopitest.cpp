#include "rhopi.h"

#include <assert.h>
#include <iostream>
using std::cout;
using std::endl;
#include "test.h"
int main() {
	uint64_t rot = 0x123456789abcdef0;
	for (uint8_t i = 0; i < 64; i++) {
		assert(rotatel(rotater(rot, i), i) == rot);
		assert(rotater(rotatel(rot, i), i) == rot);
	}
	for (uint8_t i = 1; i < 64; i++) {
		assert(rotatel(rotatel(rot, i), 64-i) == rot);
		assert(rotater(rotater(rot, i), 64-i) == rot);
	}
	ethhash hash1, hash2;
	randomize(hash1);
	assignHash(hash2, hash1);
	keccakRhoPi(hash1);
	inverseRhoPi(hash1);
	assertEqual(hash1, hash2);
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < i; j++) {
			keccakRhoPi(hash1);
		}
		for (int j = 0; j < i; j++) {
			inverseRhoPi(hash1);
		}
		assertEqual(hash1, hash2);
	}

	return 0;
}
