/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===----------------------- functional.cpp -------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "functional"

_LIBCPP_BEGIN_NAMESPACE_STD

#ifdef _LIBCPP_ABI_BAD_FUNCTION_CALL_KEY_FUNCTION
bad_function_call::~bad_function_call() _NOEXCEPT
{
}

const char*
bad_function_call::what() const _NOEXCEPT
{
    return "std::bad_function_call";
}
#endif

_LIBCPP_END_NAMESPACE_STD
