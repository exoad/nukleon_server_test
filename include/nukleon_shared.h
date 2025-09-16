#ifndef NUKLEON_SHARED_H
#define NUKLEON_SHARED_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

typedef int32_t NkInt32;
typedef int64_t NkInt64;
typedef uint32_t NkUInt32;
typedef uint64_t NkUInt64;
typedef char NkInt8;
typedef uint8_t NkUInt8;
typedef uintptr_t NkUPointer;
typedef size_t NkSize;
typedef float NkFloat32;
typedef double NkFloat64;
typedef int16_t NkInt16;
typedef uint16_t NkUInt16;
typedef void NkVoid;
typedef const NkInt8* NkString;
typedef FILE CFile;
typedef bool NkBool;
typedef NkVoid* NkAny;

#define null NULL
#define NK_PRINT(format, ...) printf(format "\n", ##__VA_ARGS__)

#endif