#include "theta.h"
#include "thetainternal.h"
#include <unistd.h>
static uint8_t preimageIndices[NUM_IMAGES];
static uint32_t preimages[NUM_IMAGES][32];
int create_preimages() {
	bzero(&preimageIndices, sizeof(preimageIndices));
	for (uint32_t i = 0; i < NUM_IMAGES; i++) {
		const slice_t slice(i);
		const slice_t theta = bitTheta(slice);
		for (uint8_t j = 0; j < 32; j++) {
			const parity_t parity(j);
			const uint32_t output = applyParity(theta, parity).to_ulong();
			uint32_t preimage = i | (j << 25);
			assert(preimageIndices[output] < 32);
			preimages[output][preimageIndices[output]++] = preimage;
		}
	}
	for (uint32_t i = 0; i < NUM_IMAGES; i++) {
		if (write(1, &preimages[i], sizeof(preimages[i])) == -1) {
			perror("write");
			return -1;
		}
	}
	return 0;
}

int main() {
	return create_preimages();
}
