#include <stdint.h>

#include <iostream>
using std::cout;
using std::endl;
#include <unordered_map>
using std::unordered_map;
#include <vector>
using std::vector;
#include <bitset>
using std::bitset;

unordered_map<bitset<5>, vector<bitset<5> > > results;
void chi_depth(int i, bitset<5> inputs) {
	if (i == 5) {
	    bitset<5> outputs;
		outputs[0] = (inputs[0] != (!inputs[1] && inputs[2]));
		outputs[1] = (inputs[1] != (!inputs[2] && inputs[3]));
		outputs[2] = (inputs[2] != (!inputs[3] && inputs[4]));
		outputs[3] = (inputs[3] != (!inputs[4] && inputs[0]));
		outputs[4] = (inputs[4] != (!inputs[0] && inputs[1]));
		results[outputs].push_back(inputs);
	} else {
		inputs[i] = false;
		chi_depth(i+1, inputs);
		inputs[i] = true;
		chi_depth(i+1, inputs);
	}
}

void print_depth(int i, bitset<5> outputs) {
	if (i < 0) {
		//cout << outputs.to_ulong() << ':' << ' ';
		for (auto it = results[outputs].cbegin(); it != results[outputs].cend(); it++) {
			cout << it->to_ulong() << ' ';
		}
		cout << ','<< endl;
	} else {
		outputs[i] = false;
		print_depth(i-1, outputs);
		outputs[i] = true;
		print_depth(i-1, outputs);
	}
}

#include "chi.h"
#include <assert.h>
// chi was reversible and also periodic
void inverseChi(ethhash &hash) {
	for (int j = 20; j >= 0; j -= 5) {
		uint64_t bc[5] = {0,0,0,0,0};
		for (uint8_t shift = 0; shift < 64; shift++) {
			// calculate inverse chi for each bit
			uint64_t chin = 0;
			chin |= (hash.q[j] & (1llu << shift)) >> shift;
			chin |= ((hash.q[j+1] & (1llu << shift)) >> shift) << 1llu;
			chin |= ((hash.q[j+2] & (1llu << shift)) >> shift) << 2llu;
			chin |= ((hash.q[j+3] & (1llu << shift)) >> shift) << 3llu;
			chin |= ((hash.q[j+4] & (1llu << shift)) >> shift) << 4llu;
			assert(chin < 32);
			uint8_t invChi = chi[chin];
			bc[0] |= (invChi & 1llu) << shift;
			bc[1] |= (invChi & 2llu) >> 1 << shift;
			bc[2] |= (invChi & 4llu) >> 2 << shift;
			bc[3] |= (invChi & 8llu) >> 3 << shift;
			bc[4] |= (invChi & 16llu) >> 4 << shift;
		}
		hash.q[j] = bc[0];
		hash.q[j+1] = bc[1];
		hash.q[j+2] = bc[2];
		hash.q[j+3] = bc[3];
		hash.q[j+4] = bc[4];
	}
}

void keccakChi(ethhash &hash) {
	for (int j = 0; j < 25; j += 5) {
		uint64_t bc[5];
		bc[0] = hash.q[j + 0];
		bc[1] = hash.q[j + 1];
		bc[2] = hash.q[j + 2];
		bc[3] = hash.q[j + 3];
		bc[4] = hash.q[j + 4];

		hash.q[j + 0] ^= (~bc[1]) & bc[2];
		hash.q[j + 1] ^= (~bc[2]) & bc[3];
		hash.q[j + 2] ^= (~bc[3]) & bc[4];
		hash.q[j + 3] ^= (~bc[4]) & bc[0];
		hash.q[j + 4] ^= (~bc[0]) & bc[1];
	}
}

/*
int main() {
	//bitset<5> zero;
	//chi_depth(0, zero);
	//print_depth(4, zero);
	return 0;
}
*/
