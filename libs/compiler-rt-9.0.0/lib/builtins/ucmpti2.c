/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===-- ucmpti2.c - Implement __ucmpti2 -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements __ucmpti2 for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

#include "int_lib.h"

#ifdef CRT_HAS_128BIT

// Returns:  if (a <  b) returns 0
//           if (a == b) returns 1
//           if (a >  b) returns 2

COMPILER_RT_ABI si_int __ucmpti2(tu_int a, tu_int b) {
  utwords x;
  x.all = a;
  utwords y;
  y.all = b;
  if (x.s.high < y.s.high)
    return 0;
  if (x.s.high > y.s.high)
    return 2;
  if (x.s.low < y.s.low)
    return 0;
  if (x.s.low > y.s.low)
    return 2;
  return 1;
}

#endif // CRT_HAS_128BIT
