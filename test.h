#define printHash(hash) \
	for (int i = 0; i < 25; i++){\
		cout << std::hex << hash.q[i] << ' ';\
	}\
	cout << endl;
#define assertZero(hash) {\
	for (int i = 0; i < 25; i++) {\
		assert(hash.q[i] == 0);\
	}\
}
#define assertEqual(hash1, hash2) {\
	for (int i = 25; i --> 0;) {\
		assert(hash1.q[i] == hash2.q[i]);\
	}\
}
#define assignHash(hash1, hash2) {\
	for (int i = 25; i --> 0;) {\
		hash1.q[i] = hash2.q[i];\
	}\
}
#define randomize(hash) \
	for (int i = 25; i --> 0;) {\
		hash.q[i] = random();\
	}
