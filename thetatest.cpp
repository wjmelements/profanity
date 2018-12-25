#include "theta.h"
#include "thetainternal.h"
#include <iostream>
using std::cout;
using std::endl;
using std::hex;

int test_bitTheta() {
	for (uint32_t i = 0; i < NUM_IMAGES; i++) {
		bitset<25> input(i);
		assert(input.to_ulong() == i);
		for (uint8_t j = 0; j < 32; j++) {
			bitset<5> adjacentEntropy(j);
			bitset<25> output1 = bitTheta(input, adjacentEntropy);
			bitset<25> output2 = applyParity(bitTheta(input), adjacentEntropy);
			assert(output1 == output2);
			// does not work both ways:
			//bitset<25> output3 = bitTheta(applyParity(input, adjacentEntropy));
			//assert(output1 == output3);
			//assert(output3 == output2);
		}
	}
	return 0;
}
int test_inverseBitTheta() {
	for (uint32_t i = 0; i < NUM_IMAGES; i++) {
		for (uint32_t j = 0; j < 32; j++) {
			uint32_t image = getBitThetaPreimage(i, j);
			parity_t parity(image >> 25);
			slice_t input(image & 0x1ffffff);
			assert(bitTheta(input, parity) == i);
		}
	}
	return 0;
}

#include "test.h"
int test_keccakBitTheta() {
	for (uint32_t i = 0; i < 0x4000; i++) {
		ethhash hash1, hash2;
		randomize(hash1);
		assignHash(hash2, hash1);
		assertEqual(hash1, hash2);
		keccakBitTheta(hash1);
		keccakTheta(hash2);
		assertEqual(hash1, hash2);
	}
	return 0;
}

int test_inverseTheta() {
	for (uint32_t i = 0; i < 0x40; i++) {
		ethhash hash1, hash2;
		randomize(hash1);
		assignHash(hash2, hash1);
		assertEqual(hash1, hash2);
		keccakTheta(hash1);
		inverseTheta(hash1);
		assertEqual(hash1, hash2);
		inverseTheta(hash1);
		keccakTheta(hash1);
		assertEqual(hash1, hash2);
		for (int i = 2; i < 40; i++) {
			for (int j = 0; j < i; j++) {
				inverseTheta(hash1);
			}
			for (int j = 0; j < i; j++) {
				keccakTheta(hash1);
			}
			assertEqual(hash1, hash2);
		}
	}
	return 0;
}

int main() {
	//test_bitTheta();
	//test_keccakBitTheta();
	initThetaInverse();
	cout << "Initialized" << endl;
	//test_inverseBitTheta();
	test_inverseTheta();
	return 0;
}
