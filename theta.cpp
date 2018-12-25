#include "theta.h"

#include "rotate.h"
#include "thetainternal.h"


static slice_t sliceOf(const ethhash cube, uint8_t i) {
	slice_t slice;
	for (uint8_t j = 0; j < 25; j++) {
		slice[j] = cube.q[j] & (1ull << i);
	}
	return slice;
}

static void setSlice(ethhash &cube, uint8_t i , slice_t slice) {
	for (uint8_t j = 0; j < 25; j++) {
		if (slice[j]) {
			cube.q[j] |= (1ull << i);
		} else {
			cube.q[j] &= ~(1ull << i);
		}
		//cube.q[j] = (cube.q[j] & ~(1ull << i)) | (slice[j] << i);
	}
}

static parity_t parityOf(slice_t slice) {
	parity_t parity;
	parity[0] = slice[0] ^ slice[5] ^ slice[10] ^ slice[15] ^ slice[20];
	parity[1] = slice[1] ^ slice[6] ^ slice[11] ^ slice[16] ^ slice[21];
	parity[2] = slice[2] ^ slice[7] ^ slice[12] ^ slice[17] ^ slice[22];
	parity[3] = slice[3] ^ slice[8] ^ slice[13] ^ slice[18] ^ slice[23];
	parity[4] = slice[4] ^ slice[9] ^ slice[14] ^ slice[19] ^ slice[24];
	return parity;
}

slice_t bitTheta(slice_t slice) {
	parity_t parity = parityOf(slice);
	slice_t output;
	for (int i = 0; i < 5; i++) {
		bool t = parity[(i+4) % 5];
		output[i] = slice[i] ^ t;
		output[i + 5] = slice[i + 5] ^ t;
		output[i + 10] = slice[i + 10] ^ t;
		output[i + 15] = slice[i + 15] ^ t;
		output[i + 20] = slice[i + 20] ^ t;
	}
	return output;
}

slice_t applyParity(slice_t slice, parity_t nextParity) {
	slice_t output;
	for (int i = 0; i < 5; i++) {
	bool t = nextParity[(i+1) % 5];
		output[i] = slice[i] ^ t;
		output[i + 5] = slice[i + 5] ^ t;
		output[i + 10] = slice[i + 10] ^ t;
		output[i + 15] = slice[i + 15] ^ t;
		output[i + 20] = slice[i + 20] ^ t;
	}
	return output;
}

slice_t bitTheta(slice_t slice, parity_t nextParity) {
	bitset<5> parity;
	parity[0] = slice[0] ^ slice[5] ^ slice[10] ^ slice[15] ^ slice[20];
	parity[1] = slice[1] ^ slice[6] ^ slice[11] ^ slice[16] ^ slice[21];
	parity[2] = slice[2] ^ slice[7] ^ slice[12] ^ slice[17] ^ slice[22];
	parity[3] = slice[3] ^ slice[8] ^ slice[13] ^ slice[18] ^ slice[23];
	parity[4] = slice[4] ^ slice[9] ^ slice[14] ^ slice[19] ^ slice[24];
	bitset<25> output;
	for (int i = 0; i < 5; i++) {
		// there are two possibilities for the rotated value
		// so it only actually adds five bits of entropy
		// there are now only 32 preimages
		// entropy bits are further useful for filling the next dimension
		// which preimage has that bit is on the other five values
		bool t = parity[(i+4) % 5] ^ nextParity[(i+1) % 5];
		output[i] = slice[i] ^ t;
		output[i + 5] = slice[i + 5] ^ t;
		output[i + 10] = slice[i + 10] ^ t;
		output[i + 15] = slice[i + 15] ^ t;
		output[i + 20] = slice[i + 20] ^ t;
	}
	return output;
}

#define OUTPUT_SIZE (NUM_IMAGES << 5)
#define OUTPUT_FILE_SIZE (OUTPUT_SIZE * 4)

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <vector>
#include <strings.h>
#include <assert.h>
using std::vector;

static uint8_t preimageIndices[NUM_IMAGES];
uint32_t preimages[NUM_IMAGES][32];

int create_preimages() {
	bzero(&preimageIndices, sizeof(preimageIndices));
	bzero(&preimages, sizeof(preimages));
	//memset(&preimages, 0xff, sizeof(preimages));
	for (uint32_t i = 0; i < NUM_IMAGES; i++) {
		const slice_t slice(i);
		const slice_t theta = bitTheta(slice);
		for (uint8_t j = 0; j < 32; j++) {
			const parity_t parity(j);
			const uint32_t output = applyParity(theta, parity).to_ulong();
			uint32_t preimage = i | (j << 25);
			assert(preimageIndices[output] < 32);
			preimages[output][preimageIndices[output]++] = preimage;
			/*
			for (int k = 0; k < 33; k++) {
				assert(k < 32);
				if (preimages[output][k] == 0xffffffffffffffff) {
					preimages[output][k] = preimage;
					break;
				}
			}
			*/
		}
	}
	//write(1, preimages, sizeof(preimages));
	for (uint32_t i = 0; i < NUM_IMAGES; i++) {
		if (write(1, &preimages[i], sizeof(preimages[i])) == -1) {
			perror("write");
			return -1;
		}
	}
	return 0;
}

// this takes ~3 seconds
void initThetaInverse() {
	int fd = open("bittheta.inverse", O_RDONLY);
	if (fd == -1) {
		perror("open bittheta.inverse");
		exit(1);
	}
	assert(OUTPUT_FILE_SIZE == sizeof(preimages));
	for (uint32_t i = 0; i < NUM_IMAGES; i+=1048576) {
		if (read(fd, &preimages[i], 1048576*sizeof(preimages[i])) == -1) {
			perror("read bittheta.inverse");
			exit(1);
		}
	}
	close(fd);
}

void keccakTheta(ethhash &hash) {
	uint64_t parity[5];
	parity[0] = hash.q[0] ^ hash.q[5] ^ hash.q[10] ^ hash.q[15] ^ hash.q[20];
	parity[1] = hash.q[1] ^ hash.q[6] ^ hash.q[11] ^ hash.q[16] ^ hash.q[21];
	parity[2] = hash.q[2] ^ hash.q[7] ^ hash.q[12] ^ hash.q[17] ^ hash.q[22];
	parity[3] = hash.q[3] ^ hash.q[8] ^ hash.q[13] ^ hash.q[18] ^ hash.q[23];
	parity[4] = hash.q[4] ^ hash.q[9] ^ hash.q[14] ^ hash.q[19] ^ hash.q[24];
	for (int i = 0; i < 5; i++) {
		uint64_t t = parity[(i + 4) % 5] ^ rotatel(parity[(i + 1) % 5], 1); 
		hash.q[i] ^= t;
		hash.q[i + 5] ^= t;
		hash.q[i + 10] ^= t;
		hash.q[i + 15] ^= t;
		hash.q[i + 20] ^= t;
	}
}

void keccakBitTheta(ethhash &hash) {
	ethhash out;
	for (uint16_t i = 0; i < 64; i++) {
		const slice_t slice = sliceOf(hash, i);
		const slice_t adjslice = sliceOf(hash, (i + 63) % 64);
		const parity_t adjparity  = parityOf(adjslice);
		const slice_t nextslice = bitTheta(slice, adjparity);
		setSlice(out, i, nextslice);
	}
	hash = out;
}


typedef struct {
	uint32_t candidateLayer;
	uint8_t layerIndex;
} searchstate_t;

void inverseTheta(ethhash &output) {
	ethhash input;
	vector<searchstate_t> searchSpace;
	searchstate_t state;
	slice_t zeroSlice = sliceOf(output, 0);
	for (uint8_t i = 0; i < 32; i++) {
		state.layerIndex = 0;
		state.candidateLayer = preimages[zeroSlice.to_ulong()][i];
		searchSpace.push_back(state);
	}
	do {
		state = searchSpace.back();
		searchSpace.pop_back();
		uint8_t nextLayerIndex = (state.layerIndex + 63) % 64;
		setSlice(input, state.layerIndex, state.candidateLayer & 0x1ffffff);
		slice_t adjSlice = sliceOf(output, nextLayerIndex);
		parity_t adjParity(state.candidateLayer >> 25);
		state.layerIndex = nextLayerIndex;
		for (uint8_t i = 0; i < 32; i++) {
			state.candidateLayer = preimages[adjSlice.to_ulong()][i];
			slice_t inputSlice(state.candidateLayer & 0x1ffffff);
			if (parityOf(inputSlice) != adjParity) {
				continue;
			}
			if (nextLayerIndex == 0) {
				if (inputSlice != sliceOf(input, 0)) {
					continue;
				}
				output = input;
				return;
			}
			searchSpace.push_back(state);
		}
	} while (searchSpace.size());
	assert(false);
}
