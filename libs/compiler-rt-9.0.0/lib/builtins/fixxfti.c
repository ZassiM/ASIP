/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===-- fixxfti.c - Implement __fixxfti -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements __fixxfti for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

#include "int_lib.h"

#ifdef CRT_HAS_128BIT

// Returns: convert a to a signed long long, rounding toward zero.

// Assumption: long double is an intel 80 bit floating point type padded with 6
// bytes ti_int is a 128 bit integral type value in long double is representable
// in ti_int

// gggg gggg gggg gggg gggg gggg gggg gggg | gggg gggg gggg gggg seee eeee eeee
// eeee | 1mmm mmmm mmmm mmmm mmmm mmmm mmmm mmmm | mmmm mmmm mmmm mmmm mmmm
// mmmm mmmm mmmm

COMPILER_RT_ABI ti_int __fixxfti(long double a) {
  const ti_int ti_max = (ti_int)((~(tu_int)0) / 2);
  const ti_int ti_min = -ti_max - 1;
  long_double_bits fb;
  fb.f = a;
  int e = (fb.u.high.s.low & 0x00007FFF) - 16383;
  if (e < 0)
    return 0;
  ti_int s = -(si_int)((fb.u.high.s.low & 0x00008000) >> 15);
  ti_int r = fb.u.low.all;
  if ((unsigned)e >= sizeof(ti_int) * CHAR_BIT)
    return a > 0 ? ti_max : ti_min;
  if (e > 63)
    r <<= (e - 63);
  else
    r >>= (63 - e);
  return (r ^ s) - s;
}

#endif // CRT_HAS_128BIT
