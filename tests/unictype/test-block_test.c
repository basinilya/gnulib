/* Test the Unicode character type functions.
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

#include <config.h>

#include "unictype.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int
main ()
{
  uc_block_t b;

  b.start = 0x20000;
  b.end = 0x2A6D6;
  b.name = "CJK Supplemental Area";

  ASSERT (!uc_is_block (0x5678, &b));
  ASSERT (!uc_is_block (0x1FFFF, &b));
  ASSERT (uc_is_block (0x20000, &b));
  ASSERT (uc_is_block (0x25678, &b));
  ASSERT (uc_is_block (0x2A6D6, &b));
  ASSERT (!uc_is_block (0x2A6D7, &b));

  return 0;
}
