/* Test of splitting a 'long double' into fraction and mantissa.
   Copyright (C) 2007 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.  */

/* Written by Bruno Haible <bruno@clisp.org>, 2007.  */

#include <config.h>

#include "printf-frexpl.h"

#include <float.h>
#include <stdlib.h>

#define ASSERT(expr) if (!(expr)) abort ();

static long double
my_ldexp (long double x, int d)
{
  for (; d > 0; d--)
    x *= 2.0L;
  for (; d < 0; d++)
    x *= 0.5L;
  return x;
}

int
main ()
{
  int i;
  long double x;

  for (i = 1, x = 1.0L; i <= LDBL_MAX_EXP; i++, x *= 2.0L)
    {
      int exp = -9999;
      long double mantissa = printf_frexpl (x, &exp);
      ASSERT (exp == i - 1);
      ASSERT (mantissa == 1.0L);
    }
  for (i = 1, x = 1.0L; i >= LDBL_MIN_EXP; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = printf_frexpl (x, &exp);
      ASSERT (exp == i - 1);
      ASSERT (mantissa == 1.0L);
    }
  for (; i >= LDBL_MIN_EXP - 100 && x > 0.0L; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = printf_frexpl (x, &exp);
      ASSERT (exp == LDBL_MIN_EXP - 1);
      ASSERT (mantissa == my_ldexp (1.0L, i - LDBL_MIN_EXP));
    }

  for (i = 1, x = 1.01L; i <= LDBL_MAX_EXP; i++, x *= 2.0L)
    {
      int exp = -9999;
      long double mantissa = printf_frexpl (x, &exp);
      ASSERT (exp == i - 1);
      ASSERT (mantissa == 1.01L);
    }
  for (i = 1, x = 1.01L; i >= LDBL_MIN_EXP; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = printf_frexpl (x, &exp);
      ASSERT (exp == i - 1);
      ASSERT (mantissa == 1.01L);
    }
  for (; i >= LDBL_MIN_EXP - 100 && x > 0.0L; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = printf_frexpl (x, &exp);
      ASSERT (exp == LDBL_MIN_EXP - 1);
      ASSERT (mantissa >= my_ldexp (1.0L, i - LDBL_MIN_EXP));
      ASSERT (mantissa <= my_ldexp (2.0L, i - LDBL_MIN_EXP));
      ASSERT (mantissa == my_ldexp (x, - exp));
    }

  for (i = 1, x = 1.73205L; i <= LDBL_MAX_EXP; i++, x *= 2.0L)
    {
      int exp = -9999;
      long double mantissa = printf_frexpl (x, &exp);
      ASSERT (exp == i - 1);
      ASSERT (mantissa == 1.73205L);
    }
  for (i = 1, x = 1.73205L; i >= LDBL_MIN_EXP; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = printf_frexpl (x, &exp);
      ASSERT (exp == i - 1);
      ASSERT (mantissa == 1.73205L);
    }
  for (; i >= LDBL_MIN_EXP - 100 && x > 0.0L; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = printf_frexpl (x, &exp);
      ASSERT (exp == LDBL_MIN_EXP - 1);
      ASSERT (mantissa >= my_ldexp (1.0L, i - LDBL_MIN_EXP));
      ASSERT (mantissa <= my_ldexp (2.0L, i - LDBL_MIN_EXP));
      ASSERT (mantissa == my_ldexp (x, - exp));
    }

  return 0;
}