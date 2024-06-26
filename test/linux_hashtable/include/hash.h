#ifndef __LINUX_HASH_H
#define __LINUX_HASH_H

/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include "type.h"
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
/* 2^31 + 2^29 - 2^25 + 2^22 - 2^19 - 2^16 + 1 */
#define GOLDEN_RATIO_PRIME_32 0x9e370001UL
/*  2^63 + 2^61 - 2^57 + 2^54 - 2^51 - 2^18 + 1 */
#define GOLDEN_RATIO_PRIME_64 0x9e37fffffffc0001UL

#ifndef BITS_PER_LONG
#define BITS_PER_LONG 32
#endif /* BITS_PER_LONG */

#if BITS_PER_LONG == 32
#define GOLDEN_RATIO_PRIME GOLDEN_RATIO_PRIME_32
#define hash_long(val, bits) hash_32(val, bits)
#elif BITS_PER_LONG == 64
#define hash_long(val, bits) hash_64(val, bits)
#define GOLDEN_RATIO_PRIME GOLDEN_RATIO_PRIME_64
#endif

/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/
static inline u64 hash_64(u64 val, unsigned int bits)
{
    u64 hash = val;

#if defined(CONFIG_ARCH_HAS_FAST_MULTIPLIER) && BITS_PER_LONG == 64
    hash = hash * GOLDEN_RATIO_PRIME_64;
#else
    /*  Sigh, gcc can't optimise this alone like it does for 32 bits. */
    u64 n = hash;
    n <<= 18;
    hash -= n;
    n <<= 33;
    hash -= n;
    n <<= 3;
    hash += n;
    n <<= 3;
    hash -= n;
    n <<= 4;
    hash += n;
    n <<= 2;
    hash += n;
#endif

    /* High bits are more random, so use them. */
    return hash >> (64 - bits);
}

static inline u32 hash_32(u32 val, unsigned int bits)
{
    /* On some cpus multiply is faster, on others gcc will do shifts */
    u32 hash = val * GOLDEN_RATIO_PRIME_32;

    /* High bits are more random, so use them. */
    return hash >> (32 - bits);
}

static inline unsigned long hash_ptr(const void *ptr, unsigned int bits)
{
    return hash_long((unsigned long)ptr, bits);
}

static inline u32 hash32_ptr(const void *ptr)
{
    unsigned long val = (unsigned long)ptr;

#if BITS_PER_LONG == 64
    val ^= (val >> 32);
#endif
    return (u32)val;
}
/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/

#endif /* __LINUX_HASH_H */
