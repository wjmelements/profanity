#include <bitset>
using std::bitset;

typedef bitset<25> slice_t;
typedef bitset<5> parity_t;
#define NUM_IMAGES (1ull << 25)
slice_t bitTheta(slice_t slice);
slice_t bitTheta(slice_t slice, parity_t nextParity);
slice_t applyParity(slice_t slice, parity_t nextParity);
extern uint32_t preimages[NUM_IMAGES][32];
void keccakBitTheta(ethhash &hash);
