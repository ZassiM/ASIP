/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===------------------------- fallback_malloc.h --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _FALLBACK_MALLOC_H
#define _FALLBACK_MALLOC_H

#include "__cxxabi_config.h"
#include <cstddef> // for size_t

namespace __cxxabiv1 {

// Allocate some memory from _somewhere_
_LIBCXXABI_HIDDEN void * __aligned_malloc_with_fallback(size_t size);

// Allocate and zero-initialize memory from _somewhere_
_LIBCXXABI_HIDDEN void * __calloc_with_fallback(size_t count, size_t size);

_LIBCXXABI_HIDDEN void __aligned_free_with_fallback(void *ptr);
_LIBCXXABI_HIDDEN void __free_with_fallback(void *ptr);

} // namespace __cxxabiv1

#endif
