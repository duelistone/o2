#include "hash.h"

#define ZOBRIST_HASH 0

#if ZOBRIST_HASH
// Random 24 bit numbers from random.org
u32 randomNumbers[128] = {
0xa612c2, 0x387c17, 0xa075d6, 0x38eb5f, 0x9cf5c1, 0x7364fb,
0x221faf, 0x0fcf5b, 0x21c527, 0x19fa0b, 0x938e2b, 0x91b514, 
0x75f7ac, 0xd203d9, 0x8550fb, 0x334e87, 0xa3189e, 0xda5cbb,
0x52be5b, 0xf9c469, 0x290746, 0xd00270, 0xdb7961, 0x2d5b84,
0x12c1e3, 0x63be19, 0x7bc704, 0x44c8c8, 0x8c38bc, 0x87f5c4,
0x42f970, 0xa19ec9, 0x87ff7a, 0x462692, 0xf26cb8, 0x67f102,
0xf613b5, 0xdc3691, 0x2c9b68, 0x8537d2, 0xf54f48, 0xc13ba4, 
0x84c56a, 0x419609, 0x49e718, 0xb3d104, 0x7dc2b6, 0xa95b82,
0x697948, 0x710d60, 0x3bb159, 0x95d947, 0x8e1510,
0x1d5afd, 0x07bfd4, 0x4c1146, 0x2d05f6, 0x6aa927,  
0xa7245e, 0x9639db, 0xae3b5e, 0x2e7136, 0xf5265a, 
0x2759dd, 0x4a7739, 0x42a3f4, 0xe638a0, 0x4cb6b1,
0x5dc5a0, 0x8479bb, 0xec8a3e, 0xa6e4b4, 0x77d6f4,
0xb90a3c, 0x35a86b, 0x965014, 0x7f7e73, 0xfcc729,
0x1094bf, 0xc63817, 0xab45cf, 0xcc2663, 0x367c62,
0x7a1fb7, 0xf72ae8, 0x9f2511, 0x627371, 0xa3c655,
0xebc1e7, 0xcf4db1, 0xdb0c0c, 0x9a7b08, 0xed269a,
0x706dc1, 0x271741, 0x935259, 0xf0c4f1, 0xda22c3,
0x760876, 0xdf8e0d, 0xd61d74, 0x3a827c, 0xb929c0,
0x01e832, 0xcf86b1, 0x8b2f58, 0x1252b0, 0xf25940,
0x45fd58, 0xa0671e, 0x31f2d6, 0x700f2c, 0x9f06c6,
0xf442e1, 0x272e70, 0xcfe7f9, 0x577558, 0xe143bd, 
0x575ed9, 0x00699b, 0xe0e150, 0x3e5a13, 0x0b7f68, 
0xb87e5a, 0x2a8fa6, 0xec7d03,
0x9d9c8c, 0xeebed9}; 

// 24 bit zobrist hash
u32 boardHash(u64 black, u64 white) {
    int result = 0;
    while (black) {
        int k = CLZ(black);
        black ^= BIT(k);
        result ^= randomNumbers[2 * k];
    }
    while (white) {
        int k = CLZ(white);
        white ^= BIT(k);
        result ^= randomNumbers[2 * k + 1];
    }
    return result;
}
#else
// Uses FNV-1a hash
u32 boardHash(u64 black, u64 white) {
    u32 h = 2166136261u;
     
    // Take one octet of data at a time
    h = (h ^ (u8) black) * 16777619; black >>= 8;
    h = (h ^ (u8) black) * 16777619; black >>= 8;
    h = (h ^ (u8) black) * 16777619; black >>= 8;
    h = (h ^ (u8) black) * 16777619; black >>= 8;
    h = (h ^ (u8) black) * 16777619; black >>= 8;
    h = (h ^ (u8) black) * 16777619; black >>= 8;
    h = (h ^ (u8) black) * 16777619; black >>= 8;
    h = (h ^ (u8) black) * 16777619;
    h = (h ^ (u8) white) * 16777619; white >>= 8;
    h = (h ^ (u8) white) * 16777619; white >>= 8;
    h = (h ^ (u8) white) * 16777619; white >>= 8;
    h = (h ^ (u8) white) * 16777619; white >>= 8;
    h = (h ^ (u8) white) * 16777619; white >>= 8;
    h = (h ^ (u8) white) * 16777619; white >>= 8;
    h = (h ^ (u8) white) * 16777619; white >>= 8;
    h = (h ^ (u8) white) * 16777619;

    // Trim hash down to HASH_LEN bits
    h = (h >> HASH_LEN) ^ (h & ((1u << HASH_LEN) - 1));

    return h;
}
#endif

#if 0
#define MIX(a,b,c) \
{ \
    a -= b; a -= c; a ^= (c >> 13); \
    b -= c; b -= a; b ^= (a << 8); \
    c -= a; c -= b; c ^= (b >> 13); \
    a -= b; a -= c; a ^= (c >> 12); \
    b -= c; b -= a; b ^= (a << 16); \
    c -= a; c -= b; c ^= (b >> 5); \
    a -= b; a -= c; a ^= (c >> 3); \
    b -= c; b -= a; b ^= (a << 10); \
    c -= a; c -= b; c ^= (b >> 15); \
}

u32 boardHash(u64 black, u64 white) {
    unsigned a, b;
    unsigned c = 274762803; // Can be changed
    unsigned len = 16;
    unsigned tc = TC(black, white);

    // Extract bytes
    u8 bytes[16];
    u8 *k = bytes;
    int loopNumber = 0;
    while (black) {
        k[2 * loopNumber] = black & 0xFF;
        k[2 * loopNumber + 1] = white & 0xFF;
        black >>= 8;
        white >>= 8;
        loopNumber++;
    }

    a = b = 0x9e3779b9;

    a += (k[0] + ((unsigned)k[1] << 8) + ((unsigned)k[2] << 16) + ((unsigned)k[3] << 24));
    b += (k[4] + ((unsigned)k[5] << 8) + ((unsigned)k[6] << 16) + ((unsigned)k[7] << 24));
    c += (k[8] + ((unsigned)k[9] << 8) + ((unsigned)k[10] << 16) + ((unsigned)k[11] << 24));

    MIX(a, b, c);

    k = k + 12;
    len -= 12;

    c += tc;

    switch (len)
    {
    case 11: c += ((unsigned)k[10] << 24);
    case 10: c += ((unsigned)k[9] << 16);
    case 9: c += ((unsigned)k[8] << 8);
    /* First byte of c reserved for total count */
    case 8: b += ((unsigned)k[7] << 24);
    case 7: b += ((unsigned)k[6] << 16);
    case 6: b += ((unsigned)k[5] << 8);
    case 5: b += k[4];
    case 4: a += ((unsigned)k[3] << 24);
    case 3: a += ((unsigned)k[2] << 16);
    case 2: a += ((unsigned)k[1] << 8);
    case 1: a += k[0];
    }

    MIX(a, b, c);

    return (c >> 24) ^ (c & 0xFFFFFFu);
}
    
// Uses One-at-a-time hash
u32 boardHash(u64 black, u64 white) {
    u32 h = 0;
    h += (u8) black; h += (h << 10); h ^= (h >> 6); black >>= 8;
    h += (u8) white; h += (h << 10); h ^= (h >> 6); white >>= 8;
    h += (u8) black; h += (h << 10); h ^= (h >> 6); black >>= 8;
    h += (u8) white; h += (h << 10); h ^= (h >> 6); white >>= 8;
    h += (u8) black; h += (h << 10); h ^= (h >> 6); black >>= 8;
    h += (u8) white; h += (h << 10); h ^= (h >> 6); white >>= 8;
    h += (u8) black; h += (h << 10); h ^= (h >> 6); black >>= 8;
    h += (u8) white; h += (h << 10); h ^= (h >> 6); white >>= 8;
    h += (u8) black; h += (h << 10); h ^= (h >> 6); black >>= 8;
    h += (u8) white; h += (h << 10); h ^= (h >> 6); white >>= 8;
    h += (u8) black; h += (h << 10); h ^= (h >> 6); black >>= 8;
    h += (u8) white; h += (h << 10); h ^= (h >> 6); white >>= 8;
    h += (u8) black; h += (h << 10); h ^= (h >> 6); black >>= 8;
    h += (u8) white; h += (h << 10); h ^= (h >> 6); white >>= 8;
    h += (u8) black; h += (h << 10); h ^= (h >> 6);
    h += (u8) white; h += (h << 10); h ^= (h >> 6);
    h += (h << 3); h ^= (h >> 11); h += (h << 15);

    return (h >> 24) ^ (h & 0xFFFFFFu);
}

#endif
