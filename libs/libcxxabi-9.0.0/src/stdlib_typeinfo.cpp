/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===----------------------------- typeinfo.cpp ---------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <typeinfo>

namespace std
{

// type_info

type_info::~type_info()
{
}

// bad_cast

bad_cast::bad_cast() _NOEXCEPT
{
}

bad_cast::~bad_cast() _NOEXCEPT
{
}

const char*
bad_cast::what() const _NOEXCEPT
{
  return "std::bad_cast";
}

// bad_typeid

bad_typeid::bad_typeid() _NOEXCEPT
{
}

bad_typeid::~bad_typeid() _NOEXCEPT
{
}

const char*
bad_typeid::what() const _NOEXCEPT
{
  return "std::bad_typeid";
}

}  // std
