/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===-- lib/truncsfhf2.c - single -> half conversion --------------*- C -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#define SRC_SINGLE
#define DST_HALF
#include "fp_trunc_impl.inc"

// Use a forwarding definition and noinline to implement a poor man's alias,
// as there isn't a good cross-platform way of defining one.
COMPILER_RT_ABI NOINLINE uint16_t __truncsfhf2(float a) {
  return __truncXfYf2__(a);
}

COMPILER_RT_ABI uint16_t __gnu_f2h_ieee(float a) { return __truncsfhf2(a); }

#if defined(__ARM_EABI__)
#if defined(COMPILER_RT_ARMHF_TARGET)
AEABI_RTABI uint16_t __aeabi_f2h(float a) { return __truncsfhf2(a); }
#else
COMPILER_RT_ALIAS(__truncsfhf2, __aeabi_f2h)
#endif
#endif
