/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===-- addvti3.c - Implement __addvti3 -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements __addvti3 for the compiler_rt library.
//
//===----------------------------------------------------------------------===//

#include "int_lib.h"

#ifdef CRT_HAS_128BIT

// Returns: a + b

// Effects: aborts if a + b overflows

COMPILER_RT_ABI ti_int __addvti3(ti_int a, ti_int b) {
  ti_int s = (tu_int)a + (tu_int)b;
  if (b >= 0) {
    if (s < a)
      compilerrt_abort();
  } else {
    if (s >= a)
      compilerrt_abort();
  }
  return s;
}

#endif // CRT_HAS_128BIT
