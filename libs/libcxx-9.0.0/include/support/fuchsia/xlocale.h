/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
// -*- C++ -*-
//===------------------- support/fuchsia/xlocale.h ------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef _LIBCPP_SUPPORT_FUCHSIA_XLOCALE_H
#define _LIBCPP_SUPPORT_FUCHSIA_XLOCALE_H

#if defined(__Fuchsia__)

#include <cstdlib>
#include <cwchar>
#include <support/xlocale/__posix_l_fallback.h>
#include <support/xlocale/__strtonum_fallback.h>

#endif // defined(__Fuchsia__)

#endif // _LIBCPP_SUPPORT_FUCHSIA_XLOCALE_H
