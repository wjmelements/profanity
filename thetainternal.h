#include <bitset>
using std::bitset;

typedef bitset<25> slice_t;
typedef bitset<5> parity_t;
#define NUM_IMAGES (1ull << 25)
slice_t bitTheta(slice_t slice);
slice_t bitTheta(slice_t slice, parity_t nextParity);
slice_t applyParity(slice_t slice, parity_t nextParity);
uint32_t getBitThetaPreimage(uint32_t inputSlice, uint32_t adjParity);
void keccakBitTheta(ethhash &hash);
