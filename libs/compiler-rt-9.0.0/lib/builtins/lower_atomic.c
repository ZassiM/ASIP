/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
/* lower_atomic.c
 *
 * Reimplementation of atomic.c for a known non-preemptive environment
 */

#include <stdint.h>
#include <string.h>

// Clang objects if you redefine a builtin.  This little hack allows us to
// define a function with the same name as an intrinsic.
#pragma redefine_extname __atomic_load_c __atomic_load
#pragma redefine_extname __atomic_store_c __atomic_store
#pragma redefine_extname __atomic_exchange_c __atomic_exchange
#pragma redefine_extname __atomic_compare_exchange_c __atomic_compare_exchange

void __atomic_load_c(int size, void* src, void* dest, int model) {
    memcpy(dest, src, size);
}

void __atomic_store_c(int size, void* dest, void* src, int model) {
    memcpy(dest, src, size);
}

int __atomic_compare_exchange_c(int size, void* ptr, void* expected, void* desired, int success, int failure) {
    if (memcmp(ptr, expected, size) == 0) {
        memcpy(ptr, desired, size);
        return 1;
    }
    memcpy(expected, ptr, size);
    return 0;
}

void __atomic_exchange_c(int size, void* ptr, void* val, void* old, int model) {
    memcpy(old, ptr, size);
    memcpy(ptr, val, size);
}

#define OPTIMISED_CASES\
    OPTIMISED_CASE(1, uint8_t)\
    OPTIMISED_CASE(2, uint16_t)\
    OPTIMISED_CASE(4, uint32_t)\
    OPTIMISED_CASE(8, uint64_t)

#define OPTIMISED_CASE(n, type) type __atomic_load_##n(type* src, int model) { return *src; }
OPTIMISED_CASES
#undef OPTIMISED_CASE

#define OPTIMISED_CASE(n, type) void __atomic_store_##n(type* dest, type val, int model) { *dest = val; }
OPTIMISED_CASES
#undef OPTIMISED_CASE

#define OPTIMISED_CASE(n, type) type __atomic_exchange_##n(type* dest, type val, int model) { type tmp = *dest; *dest = val; return tmp; }
OPTIMISED_CASES
#undef OPTIMISED_CASE

#define OPTIMISED_CASE(n, type) int __atomic_compare_exchange_##n(type* ptr, type* expected, type desired, int success, int failure) {\
    if (*ptr == *expected) { *ptr = desired; return 1; }\
    *expected = *ptr; return 0; }
OPTIMISED_CASES
#undef OPTIMISED_CASE

#define ATOMIC_RMW(n, type, opname, op) \
    type __atomic_fetch_##opname##_##n(type* ptr, type val, int model) {\
        type tmp = *ptr;\
        *ptr = tmp op val;\
        return tmp;\
    }

#define OPTIMISED_CASE(n, type) ATOMIC_RMW(n, type, add, +)
OPTIMISED_CASES
#undef OPTIMISED_CASE
#define OPTIMISED_CASE(n, type) ATOMIC_RMW(n, type, sub, -)
OPTIMISED_CASES
#undef OPTIMISED_CASE
#define OPTIMISED_CASE(n, type) ATOMIC_RMW(n, type, and, &)
OPTIMISED_CASES
#undef OPTIMISED_CASE
#define OPTIMISED_CASE(n, type) ATOMIC_RMW(n, type, or, |)
OPTIMISED_CASES
#undef OPTIMISED_CASE
#define OPTIMISED_CASE(n, type) ATOMIC_RMW(n, type, xor, ^)
OPTIMISED_CASES
#undef OPTIMISED_CASE
