#include <stdint.h>
#include <assert.h>

#include "chi.h"
#include "rhopi.h"
#include "theta.h"
#include "iota.h"
#include <iostream>

int main() {
	ethhash hash;
	for (int r = 24; r --> 0;) {
		// IOTA
		hash.q[0] ^= rndc[r];
		// CHI
		inverseChi(hash);
		// Rho Pi
		inverseRhoPi(hash);
		// Theta
		inverseTheta(hash);
	}
	hash.d[33] ^= 0x80000000;

	return 0;
}
