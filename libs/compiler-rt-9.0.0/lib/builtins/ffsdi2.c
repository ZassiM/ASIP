/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===-- ffsdi2.c - Implement __ffsdi2 -------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements __ffsdi2 for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

#include "int_lib.h"

// Returns: the index of the least significant 1-bit in a, or
// the value zero if a is zero. The least significant bit is index one.

COMPILER_RT_ABI si_int __ffsdi2(di_int a) {
  dwords x;
  x.all = a;
  if (x.s.low == 0) {
    if (x.s.high == 0)
      return 0;
    return __builtin_ctz(x.s.high) + (1 + sizeof(si_int) * CHAR_BIT);
  }
  return __builtin_ctz(x.s.low) + 1;
}
