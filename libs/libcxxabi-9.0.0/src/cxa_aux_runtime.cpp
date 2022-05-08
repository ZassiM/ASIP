/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===------------------------ cxa_aux_runtime.cpp -------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//
// This file implements the "Auxiliary Runtime APIs"
// https://itanium-cxx-abi.github.io/cxx-abi/abi-eh.html#cxx-aux
//===----------------------------------------------------------------------===//

#include "__cxxabi_config.h"
#ifndef _LIBCPP_LITE
#include "cxxabi.h"
#else
#define _LIBCXXABI_NORETURN  __attribute__((noreturn))
#endif
#include <new>
#include <typeinfo>

namespace __cxxabiv1 {
extern "C" {
_LIBCXXABI_FUNC_VIS _LIBCXXABI_NORETURN void __cxa_bad_cast(void) {
#ifndef _LIBCXXABI_NO_EXCEPTIONS
  throw std::bad_cast();
#else
  std::terminate();
#endif
}

_LIBCXXABI_FUNC_VIS _LIBCXXABI_NORETURN void __cxa_bad_typeid(void) {
#ifndef _LIBCXXABI_NO_EXCEPTIONS
  throw std::bad_typeid();
#else
  std::terminate();
#endif
}

_LIBCXXABI_FUNC_VIS _LIBCXXABI_NORETURN void
__cxa_throw_bad_array_new_length(void) {
#ifndef _LIBCXXABI_NO_EXCEPTIONS
  throw std::bad_array_new_length();
#else
  std::terminate();
#endif
}
} // extern "C"
} // abi
