/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===-- absvdi2.c - Implement __absvdi2 -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements __absvdi2 for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

#include "int_lib.h"

// Returns: absolute value

// Effects: aborts if abs(x) < 0

COMPILER_RT_ABI di_int __absvdi2(di_int a) {
  const int N = (int)(sizeof(di_int) * CHAR_BIT);
  if (a == ((di_int)1 << (N - 1)))
    compilerrt_abort();
  const di_int t = a >> (N - 1);
  return (a ^ t) - t;
}
