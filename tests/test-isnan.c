/* Test of isnan() substitute.
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

#include "isnan.h"

#include <limits.h>
#include <stdlib.h>

#define ASSERT(expr) if (!(expr)) abort ();

int
main ()
{
  /* Finite values.  */
  ASSERT (!isnan (3.141));
  ASSERT (!isnan (3.141e30));
  ASSERT (!isnan (3.141e-30));
  ASSERT (!isnan (-2.718));
  ASSERT (!isnan (-2.718e30));
  ASSERT (!isnan (-2.718e-30));
  /* Infinite values.  */
  ASSERT (!isnan (1.0 / 0.0));
  ASSERT (!isnan (-1.0 / 0.0));
  /* Quiet NaN.  */
  ASSERT (isnan (0.0 / 0.0));
#if defined DBL_EXPBIT0_WORD && defined DBL_EXPBIT0_BIT
  /* Signalling NaN.  */
  {
    #define NWORDS \
      ((sizeof (double) + sizeof (unsigned int) - 1) / sizeof (unsigned int))
    typedef union { double value; unsigned int word[NWORDS]; } memory_double;
    memory_double m;
    m.value = 0.0 / 0.0;
# if DBL_EXPBIT0_BIT > 0
    m.word[DBL_EXPBIT0_WORD] ^= (unsigned int) 1 << (DBL_EXPBIT0_BIT - 1);
# else
    m.word[DBL_EXPBIT0_WORD + (DBL_EXPBIT0_WORD < NWORDS / 2 ? 1 : - 1)]
      ^= (unsigned int) 1 << (sizeof (unsigned int) * CHAR_BIT - 1);
# endif
    m.word[DBL_EXPBIT0_WORD + (DBL_EXPBIT0_WORD < NWORDS / 2 ? 1 : - 1)]
      |= (unsigned int) 1 << DBL_EXPBIT0_BIT;
    ASSERT (isnan (m.value));
  }
#endif
  return 0;
}