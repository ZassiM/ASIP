/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===-- lib/adddf3.c - Double-precision addition ------------------*- C -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements double-precision soft-float addition with the IEEE-754
// default rounding (to nearest, ties to even).
//
//===----------------------------------------------------------------------===//

#define DOUBLE_PRECISION
#include "fp_add_impl.inc"

COMPILER_RT_ABI double __adddf3(double a, double b) { return __addXf3__(a, b); }

#if defined(__ARM_EABI__)
#if defined(COMPILER_RT_ARMHF_TARGET)
AEABI_RTABI double __aeabi_dadd(double a, double b) { return __adddf3(a, b); }
#else
COMPILER_RT_ALIAS(__adddf3, __aeabi_dadd)
#endif
#endif
