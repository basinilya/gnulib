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

#include <math.h>

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

  { /* NaN.  */
    int exp = -9999;
    long double mantissa;
    x = 0.0L / 0.0L;
    mantissa = frexpl (x, &exp);
    ASSERT (mantissa != mantissa);
  }

  { /* Positive infinity.  */
    int exp = -9999;
    long double mantissa;
    x = 1.0L / 0.0L;
    mantissa = frexpl (x, &exp);
    ASSERT (mantissa == x);
  }

  { /* Negative infinity.  */
    int exp = -9999;
    long double mantissa;
    x = -1.0L / 0.0L;
    mantissa = frexpl (x, &exp);
    ASSERT (mantissa == x);
  }

  { /* Positive zero.  */
    int exp = -9999;
    long double mantissa;
    x = 0.0L;
    mantissa = frexpl (x, &exp);
    ASSERT (exp == 0);
    ASSERT (mantissa == x);
  }

  { /* Negative zero.  */
    int exp = -9999;
    long double mantissa;
    x = -0.0L;
    mantissa = frexpl (x, &exp);
    ASSERT (exp == 0);
    ASSERT (mantissa == x);
  }

  for (i = 1, x = 1.0L; i <= LDBL_MAX_EXP; i++, x *= 2.0L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == 0.5L);
    }
  for (i = 1, x = 1.0L; i >= LDBL_MIN_EXP; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == 0.5L);
    }
  for (; i >= LDBL_MIN_EXP - 100 && x > 0.0L; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == 0.5L);
    }

  for (i = 1, x = -1.0L; i <= LDBL_MAX_EXP; i++, x *= 2.0L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == -0.5L);
    }
  for (i = 1, x = -1.0L; i >= LDBL_MIN_EXP; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == -0.5L);
    }
  for (; i >= LDBL_MIN_EXP - 100 && x < 0.0L; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == -0.5L);
    }

  for (i = 1, x = 1.01L; i <= LDBL_MAX_EXP; i++, x *= 2.0L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == 0.505L);
    }
  for (i = 1, x = 1.01L; i >= LDBL_MIN_EXP; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == 0.505L);
    }
  for (; i >= LDBL_MIN_EXP - 100 && x > 0.0L; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa >= 0.5L);
      ASSERT (mantissa < 1.0L);
      ASSERT (mantissa == my_ldexp (x, - exp));
    }

  for (i = 1, x = 1.73205L; i <= LDBL_MAX_EXP; i++, x *= 2.0L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == 0.866025L);
    }
  for (i = 1, x = 1.73205L; i >= LDBL_MIN_EXP; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i);
      ASSERT (mantissa == 0.866025L);
    }
  for (; i >= LDBL_MIN_EXP - 100 && x > 0.0L; i--, x *= 0.5L)
    {
      int exp = -9999;
      long double mantissa = frexpl (x, &exp);
      ASSERT (exp == i || exp == i + 1);
      ASSERT (mantissa >= 0.5L);
      ASSERT (mantissa < 1.0L);
      ASSERT (mantissa == my_ldexp (x, - exp));
    }

  return 0;
}