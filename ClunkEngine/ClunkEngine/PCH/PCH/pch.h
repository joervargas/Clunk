#pragma once

// std
#include <set>
#include <random>
#include <limits>
#include <vector>
#include <array>
#include <string>
#include <cstring>
#include <bitset>
#include <cassert>
#include <memory>
#include <sstream>
#include <fstream>
#include <assert.h>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>

// #include <Defines/Defines.h>
// #include <ClMath/ClMath.h>


// Unsigned int types

/** @brief Unsigned 8-bit integer */
typedef unsigned char u8;

/** @brief Unsigned 16-bit integer */
typedef unsigned short u16;

/** @brief Unsigned 32-bit integer */
typedef unsigned int u32;

/** @brief Unsigned 64-bit integer */
typedef unsigned long long u64;

// Signed int types.

/** @brief Signed 8-bit integer */
typedef signed char i8;

/** @brief Signed 16-bit integer */
typedef signed short i16;

/** @brief Signed 32-bit integer */
typedef signed int i32;

/** @brief Signed 64-bit integer */
typedef signed long long i64;

// Floating point types

/** @brief 32-bit floating point number */
typedef float f32;

/** @brief 64-bit floating point number */
typedef double f64;

// Boolean types

/** @brief 32-bit boolean type, used for APIs which require it */
typedef int b32;

/** @brief 8-bit boolean type */
typedef char b8;

/** @brief Shorthand for std::string */
typedef std::string String;

/** @brief A range, typically of memory */
typedef struct range {
    /** @brief The offset in bytes. */
    u64 offset;
    /** @brief The size in bytes. */
    u64 size;
} range;

// Properly define static assertions.
#if defined(__clang__) || defined(__gcc__)
    /** @brief Static assertion */
    #define STATIC_ASSERT _Static_assert
#else

    /** @brief Static assertion */
    #define STATIC_ASSERT static_assert
#endif

// Ensure all types are of the correct size.

/** @brief Assert u8 to be 1 byte.*/
STATIC_ASSERT(sizeof(u8) == 1, "Expected u8 to be 1 byte.");

/** @brief Assert u16 to be 2 bytes.*/
STATIC_ASSERT(sizeof(u16) == 2, "Expected u16 to be 2 bytes.");

/** @brief Assert u32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(u32) == 4, "Expected u32 to be 4 bytes.");

/** @brief Assert u64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(u64) == 8, "Expected u64 to be 8 bytes.");

/** @brief Assert i8 to be 1 byte.*/
STATIC_ASSERT(sizeof(i8) == 1, "Expected i8 to be 1 byte.");

/** @brief Assert i16 to be 2 bytes.*/
STATIC_ASSERT(sizeof(i16) == 2, "Expected i16 to be 2 bytes.");

/** @brief Assert i32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(i32) == 4, "Expected i32 to be 4 bytes.");

/** @brief Assert i64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(i64) == 8, "Expected i64 to be 8 bytes.");

/** @brief Assert f32 to be 4 bytes.*/
STATIC_ASSERT(sizeof(f32) == 4, "Expected f32 to be 4 bytes.");

/** @brief Assert f64 to be 8 bytes.*/
STATIC_ASSERT(sizeof(f64) == 8, "Expected f64 to be 8 bytes.");

#define EPSILON 0.000001f

// runtime assertion
#define CLUNK_ASSERT assert

// static assertion
#if defined(__clang__) || defined(__gcc__)
    #define CLUNK_STATIC_ASSERT _Static_assert
#else
    #define CLUNK_STATIC_ASSERT static_assert
#endif

// function inlining
#if defined(__clang__) || defined(__gcc__)
    #define CLUNK_INLINE __attribute__((always_inline)) inline
    #define CLUNK_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
    #define CLUNK_INLINE __forceinline
    #define CLUNK_NOINLINE __declspec(noinline)
#else
    #define CLUNK_INLINE inline
    #define CLUNK_NOINLINE
#endif

// Platform detection
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_SDL 1
    #define _WIN32_WINNT 0x0502
    #define VA_LIST va_list
    #define VSN_PRINTF(buff, buff_size, format_char_p, args) vsnprintf(buff, buff_size, format_char_p, args)
    #ifndef _WIN64
        #error "64-bit is required on Windows!"
    #endif
#elif defined(__linux__) || defined(__gnu_linux__)
    // Linux OS
    #define PLATFORM_LINUX 1
    #define PLATFORM_SDL 1
    #define VA_LIST __builtin_va_list
    #define VSN_PRINTF(buff, buff_size, format_char_p, args) vsnprintf(buff, buff_size, format_char_p, args)
    #if defined(__ANDROID__)
        #define PLATFORM_ANDROID 1
        #define PLATFORM_SDL 1 // Comment out if use native libraries
    #endif
#elif defined(__unix__)
    // Catch anything not caught by the above.
    #define PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
    // Posix
    #define PLATFORM_POSIX 1
#elif defined(__APPLE__)
    // Apple platforms
    #define PLATFORM_APPLE 1
    #define PLATFORM_SDL 1
    #include <TargetConditionals.h>
    #if TARGET_IPHONE_SIMULATOR
        // iOS Simulator
        #define PLATFORM_IOS 1
        #define PLATFORM_IOS_SIMULATOR 1
    #elif TARGET_OS_IPHONE
        #define PLATFORM_IOS 1
        #define PLATFORM_SDL 1 // Comment out if use native libraries
        // iOS device
    #elif TARGET_OS_MAC
    // Other kinds of Mac OS
    #else
        #error "Unknown Apple platform"
    #endif
#else
    #error "Unknown platform!"
#endif


#ifdef VEXPORT
// Exports
    #ifdef _MSC_VER
        #define VAPI __declspec(dllexport)
    #else
        #define VAPI __attribute__((visibility("default")))
    #endif
#else
// Imports
    #ifdef _MSC_VER
        /** @brief Import/export qualifier */
        #define VAPI __declspec(dllimport)
    #else
        /** @brief Import/export qualifier */
        #define CLAPI
    #endif
#endif

#ifdef VK_GFX
    // #define VK_NO_PROTOTYPES
    #define VULKAN_GFX
#endif

/**
 * @brief "Safer Delete" Deletes a pointer on heap and sets it to null.
 */
#define CLUNK_DELETE(m) if(m != NULL) { delete(m); m = NULL; }

/***
 * @brief attach lambda function to function call
*/
#define FUSE_BIND(f) [this](auto&&... args)->decltype(auto) \
    { return this->f(std::forward<decltype(args)>(args)...); }

#define INVALID_ID 0
#define MAX_DELTATIME 0.05f

namespace Clunk
{
    using uuid64 = size_t;
    uuid64 GenerateUUID();

    CLUNK_INLINE const u32 GetUniqueTypeID()
    {
        static u32 type = 1u;
        return type++;
    }

    template<typename T>
    CLUNK_INLINE const u32 TypeID()
    {
        static const u32 type = GetUniqueTypeID();
        return type;
    }
}