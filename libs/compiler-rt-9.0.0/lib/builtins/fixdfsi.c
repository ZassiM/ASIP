/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===-- fixdfsi.c - Implement __fixdfsi -----------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#define DOUBLE_PRECISION
#include "fp_lib.h"
typedef si_int fixint_t;
typedef su_int fixuint_t;
#include "fp_fixint_impl.inc"

COMPILER_RT_ABI si_int __fixdfsi(fp_t a) { return __fixint(a); }

#if defined(__ARM_EABI__)
#if defined(COMPILER_RT_ARMHF_TARGET)
AEABI_RTABI si_int __aeabi_d2iz(fp_t a) { return __fixdfsi(a); }
#else
COMPILER_RT_ALIAS(__fixdfsi, __aeabi_d2iz)
#endif
#endif
