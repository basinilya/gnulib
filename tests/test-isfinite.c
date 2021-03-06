/* Test of isfinite() substitute.
   Copyright (C) 2007 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* Written by Ben Pfaff, 2007, using Bruno Haible's code as a
   template. */

#include <config.h>

#include <float.h>
#include <limits.h>
#include <math.h>

#include <stdio.h>
#include <stdlib.h>

#define ASSERT(expr) \
  do									     \
    {									     \
      if (!(expr))							     \
        {								     \
          fprintf (stderr, "%s:%d: assertion failed\n", __FILE__, __LINE__); \
          abort ();							     \
        }								     \
    }									     \
  while (0)

float zerof = 0.0f;
double zerod = 0.0;
long double zerol = 0.0L;

static void
test_isfinitef ()
{
  /* Zero. */
  ASSERT (isfinite (0.0f));
  /* Subnormal values. */
  ASSERT (isfinite (FLT_MIN / 2));
  ASSERT (isfinite (-FLT_MIN / 2));
  /* Finite values.  */
  ASSERT (isfinite (3.141f));
  ASSERT (isfinite (3.141e30f));
  ASSERT (isfinite (3.141e-30f));
  ASSERT (isfinite (-2.718f));
  ASSERT (isfinite (-2.718e30f));
  ASSERT (isfinite (-2.718e-30f));
  /* Infinite values.  */
  ASSERT (!isfinite (1.0f / 0.0f));
  ASSERT (!isfinite (-1.0f / 0.0f));
  /* Quiet NaN.  */
  ASSERT (!isfinite (zerof / zerof));
#if defined FLT_EXPBIT0_WORD && defined FLT_EXPBIT0_BIT
  /* Signalling NaN.  */
  {
    #define NWORDS \
      ((sizeof (float) + sizeof (unsigned int) - 1) / sizeof (unsigned int))
    typedef union { float value; unsigned int word[NWORDS]; } memory_float;
    memory_float m;
    m.value = zerof / zerof;
# if FLT_EXPBIT0_BIT > 0
    m.word[FLT_EXPBIT0_WORD] ^= (unsigned int) 1 << (FLT_EXPBIT0_BIT - 1);
# else
    m.word[FLT_EXPBIT0_WORD + (FLT_EXPBIT0_WORD < NWORDS / 2 ? 1 : - 1)]
      ^= (unsigned int) 1 << (sizeof (unsigned int) * CHAR_BIT - 1);
# endif
    if (FLT_EXPBIT0_WORD < NWORDS / 2)
      m.word[FLT_EXPBIT0_WORD + 1] |= (unsigned int) 1 << FLT_EXPBIT0_BIT;
    else
      m.word[0] |= (unsigned int) 1;
    ASSERT (!isfinite (m.value));
    #undef NWORDS
  }
#endif
}

static void
test_isfinited ()
{
  /* Zero. */
  ASSERT (isfinite (0.0));
  /* Subnormal values. */
  ASSERT (isfinite (DBL_MIN / 2));
  ASSERT (isfinite (-DBL_MIN / 2));
  /* Finite values. */
  ASSERT (isfinite (3.141));
  ASSERT (isfinite (3.141e30));
  ASSERT (isfinite (3.141e-30));
  ASSERT (isfinite (-2.718));
  ASSERT (isfinite (-2.718e30));
  ASSERT (isfinite (-2.718e-30));
  /* Infinite values.  */
  ASSERT (!isfinite (1.0 / 0.0));
  ASSERT (!isfinite (-1.0 / 0.0));
  /* Quiet NaN.  */
  ASSERT (!isfinite (zerod / zerod));
#if defined DBL_EXPBIT0_WORD && defined DBL_EXPBIT0_BIT
  /* Signalling NaN.  */
  {
    #define NWORDS \
      ((sizeof (double) + sizeof (unsigned int) - 1) / sizeof (unsigned int))
    typedef union { double value; unsigned int word[NWORDS]; } memory_double;
    memory_double m;
    m.value = zerod / zerod;
# if DBL_EXPBIT0_BIT > 0
    m.word[DBL_EXPBIT0_WORD] ^= (unsigned int) 1 << (DBL_EXPBIT0_BIT - 1);
# else
    m.word[DBL_EXPBIT0_WORD + (DBL_EXPBIT0_WORD < NWORDS / 2 ? 1 : - 1)]
      ^= (unsigned int) 1 << (sizeof (unsigned int) * CHAR_BIT - 1);
# endif
    m.word[DBL_EXPBIT0_WORD + (DBL_EXPBIT0_WORD < NWORDS / 2 ? 1 : - 1)]
      |= (unsigned int) 1 << DBL_EXPBIT0_BIT;
    ASSERT (!isfinite (m.value));
    #undef NWORDS
  }
#endif
}

static void
test_isfinitel ()
{
  /* Zero. */
  ASSERT (isfinite (0.0L));
  /* Subnormal values. */
  ASSERT (isfinite (LDBL_MIN / 2));
  ASSERT (isfinite (-LDBL_MIN / 2));
  /* Finite values. */
  ASSERT (isfinite (3.141L));
  ASSERT (isfinite (3.141e30L));
  ASSERT (isfinite (3.141e-30L));
  ASSERT (isfinite (-2.718L));
  ASSERT (isfinite (-2.718e30L));
  ASSERT (isfinite (-2.718e-30L));
  /* Infinite values.  */
  ASSERT (!isfinite (1.0L / 0.0L));
  ASSERT (!isfinite (-1.0L / 0.0L));
  /* Quiet NaN.  */
  ASSERT (!isfinite (zerol / zerol));
#if defined LDBL_EXPBIT0_WORD && defined LDBL_EXPBIT0_BIT
  /* A bit pattern that is different from a Quiet NaN.  With a bit of luck,
     it's a Signalling NaN.  */
  {
    #define NWORDS \
      ((sizeof (long double) + sizeof (unsigned int) - 1) / sizeof (unsigned int))
    typedef union { unsigned int word[NWORDS]; long double value; }
          memory_long_double;
    memory_long_double m;
    m.value = zerol / zerol;
# if LDBL_EXPBIT0_BIT > 0
    m.word[LDBL_EXPBIT0_WORD] ^= (unsigned int) 1 << (LDBL_EXPBIT0_BIT - 1);
# else
    m.word[LDBL_EXPBIT0_WORD + (LDBL_EXPBIT0_WORD < NWORDS / 2 ? 1 : - 1)]
      ^= (unsigned int) 1 << (sizeof (unsigned int) * CHAR_BIT - 1);
# endif
    m.word[LDBL_EXPBIT0_WORD + (LDBL_EXPBIT0_WORD < NWORDS / 2 ? 1 : - 1)]
      |= (unsigned int) 1 << LDBL_EXPBIT0_BIT;
    ASSERT (!isfinite (m.value));
    #undef NWORDS
  }
#endif
}

int
main ()
{
  test_isfinitef ();
  test_isfinited ();
  test_isfinitel ();
  return 0;
}
