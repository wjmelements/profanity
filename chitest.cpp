#include "chi.h"
#include <assert.h>
#include <stdlib.h>

#include "test.h"
int main() {
	//bitset<5> zero;
	//chi_depth(0, zero);
	//print_depth(4, zero);
	ethhash hash1, hash2;
	randomize(hash1);
	assignHash(hash2, hash1);
	for (int i = 0; i < 32; i++) {
		for (int j = 0; j < i; j++) {
			keccakChi(hash1);
		}
		for (int j = 0; j < i; j++) {
			inverseChi(hash1);
		}
		assertEqual(hash1, hash2);
	}
	// chi also has a maximum period of 12
	for (int j = 0; j < 500; j++) {
		keccakChi(hash1);
		keccakChi(hash2);

		for (int i = 0; i < 12; i++) {
			keccakChi(hash1);
		}
		assertEqual(hash1, hash2);
	}
	// so it is reversible by going forward 11 times
	for (int j = 0; j < 1000; j++) {
		randomize(hash1);
		assignHash(hash2, hash1);
		for (int i = 0; i < 12; i++) {
			keccakChi(hash1);
		}
		assertEqual(hash1, hash2);
	}

	return 0;
}
