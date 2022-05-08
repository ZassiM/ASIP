/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===---------- eprintf.c - Implements __eprintf --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "int_lib.h"
#include <stdio.h>

// __eprintf() was used in an old version of <assert.h>.
// It can eventually go away, but it is needed when linking
// .o files built with the old <assert.h>.
//
// It should never be exported from a dylib, so it is marked
// visibility hidden.
#ifndef _WIN32
__attribute__((visibility("hidden")))
#endif
COMPILER_RT_ABI void
__eprintf(const char *format, const char *assertion_expression,
          const char *line, const char *file) {
  fprintf(stderr, format, assertion_expression, line, file);
  fflush(stderr);
  compilerrt_abort();
}
