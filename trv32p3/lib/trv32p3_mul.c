/*
-- File : trv32p3_mul.c
--
-- Contents : Emulation of 32-/64-bit multiplication
--
-- Copyright (c) 2020 Synopsys, Inc. This Synopsys processor model
-- captures an ASIP Designer Design Technique. The model and all associated
-- documentation are proprietary to Synopsys, Inc. and may only be used
-- pursuant to the terms and conditions of a written license agreement with
-- Synopsys, Inc.  All other use, reproduction, modification, or distribution
-- of the Synopsys processor model or the associated  documentation is
-- strictly prohibited.
*/

extern "C++"
int mul_called (int a, int _b) property (functional loop_free)
{
  int p = 0;
  unsigned b = _b;
  do {
    if( b & 1 ) p += a;
    a <<= 1;
    b >>= 1;
  } while (b != 0);
  return p;
}

extern "C++"
long long mul_called (long long a, long long _b) property (functional loop_free)
{
  long long p = 0;
  unsigned long long b = _b;
  do {
    if( b & 1 ) p += a;
    a <<= 1;
    b >>= 1;
  } while (b != 0);
  return p;
}
