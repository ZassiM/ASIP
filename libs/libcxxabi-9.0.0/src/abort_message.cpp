/*
-- Copyright (c) 2015-2019 Synopsys, Inc.  This Synopsys software and all
-- associated documentation are proprietary to Synopsys, Inc. and may only be
-- used pursuant to the terms and conditions of a written license agreement
-- with Synopsys, Inc.  All other use, reproduction, modification, or
-- distribution of the Synopsys software or the associated documentation is
-- strictly prohibited.
*/
//===------------------------- abort_message.cpp --------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "abort_message.h"

#ifdef __BIONIC__
#include <android/api-level.h>
#if __ANDROID_API__ >= 21
#include <syslog.h>
extern "C" void android_set_abort_message(const char* msg);
#else
#include <assert.h>
#endif // __ANDROID_API__ >= 21
#endif // __BIONIC__

#ifdef __APPLE__
#   if defined(__has_include) && __has_include(<CrashReporterClient.h>)
#       define HAVE_CRASHREPORTERCLIENT_H
#       include <CrashReporterClient.h>
#   endif
#endif

void abort_message(const char* format, ...)
{
    // write message to stderr
#if !defined(NDEBUG) || !defined(LIBCXXABI_BAREMETAL)
#ifdef __APPLE__
    fprintf(stderr, "libc++abi.dylib: ");
#endif
    va_list list;
    va_start(list, format);
    vfprintf(stderr, format, list);
    va_end(list);
    fprintf(stderr, "\n");
#endif

#if defined(__APPLE__) && defined(HAVE_CRASHREPORTERCLIENT_H)
    // record message in crash report
    char* buffer;
    va_list list2;
    va_start(list2, format);
    vasprintf(&buffer, format, list2);
    va_end(list2);
    CRSetCrashLogMessage(buffer);
#elif defined(__BIONIC__)
    char* buffer;
    va_list list2;
    va_start(list2, format);
    vasprintf(&buffer, format, list2);
    va_end(list2);

#if __ANDROID_API__ >= 21
    // Show error in tombstone.
    android_set_abort_message(buffer);

    // Show error in logcat.
    openlog("libc++abi", 0, 0);
    syslog(LOG_CRIT, "%s", buffer);
    closelog();
#else
    // The good error reporting wasn't available in Android until L. Since we're
    // about to abort anyway, just call __assert2, which will log _somewhere_
    // (tombstone and/or logcat) in older releases.
    __assert2(__FILE__, __LINE__, __func__, buffer);
#endif // __ANDROID_API__ >= 21
#endif // __BIONIC__

    abort();
}
