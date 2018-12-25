#ifndef rotate_h
#define rotate_h
static inline uint64_t rotatel(uint64_t t, uint8_t rot) {
	return (t << rot) | (t >> (sizeof(t) * 8 - rot));
}
static inline uint64_t rotater(uint64_t t, uint8_t rot) {
	return (t >> rot) | (t << (sizeof(t) * 8 - rot));
}
#endif
