#ifndef NUKLEON_TYPE_H
#define NUKLEON_TYPE_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef int32_t Int32;
typedef int64_t Int64;
typedef uint32_t UInt32;
typedef uint64_t UInt64;
typedef char Int8;
typedef uint8_t UInt8;
typedef uintptr_t UPtr;
typedef size_t Size;
typedef float Float32;
typedef double Float64;
typedef int16_t Int16;
typedef uint16_t UInt16;
typedef void Void;
typedef const Int8* CharSeq;
typedef FILE CFile;
typedef bool Bool;
typedef Void* Any;

#define null NULL

#ifdef _MSC_VER
#define __nk_inline static __forceinline
#else
#define __nk_inline static inline
#endif

#ifdef __GNUC__
#define __nk_unused __attribute__((unused))
#define __nk_pure __attribute__((pure))
#define __nk_const __attribute__((const))
#define __nk_hot __attribute__((hot))
#define __nk_flatten __attribute((flatten))
#define __nk_always_inline __attribute__((always_inline))
#else
#define __nk_unused
#define __nk_pure
#define __nk_const
#define __nk_hot
#define __nk_flatten
#define __nk_always_inline
#endif

#define NK_PRINT(format, ...) printf(format, ##__VA_ARGS__)

#define NK_PRINTLN(format, ...) printf(format "\n", ##__VA_ARGS__)

#define NK_PANIC(msg) do { \
    fprintf(stderr, "PANIC: %s at %s:%d in %s()\n", \
            msg, __FILE__, __LINE__, __func__); \
    exit(1); \
} while(0)

#define NK_PANICF(fmt, ...) do  \
{ \
    fprintf(stderr, "PANIC: " fmt " at %s:%d in %s()\n", \
            __VA_ARGS__, __FILE__, __LINE__, __func__); \
    exit(1); \
} while(0)

#define NK_ASSERT(condition, msg) do {                          \
    if(!(condition))                                         \
    {                                                         \
        fprintf(stderr, "ASSERTION FAILED: %s at %s:%d in %s()\n", msg, __FILE__, __LINE__, __func__); \
        exit(1);                                              \
    }                                                         \
} while(0)

#define NK_CHECK(condition, msg) do { \
    if(!(condition))  \
    {             \
        NK_PANIC(msg);                 \
    }                               \
} while(0)

CharSeq nkCanonicalizeBool(Bool v);

#endif