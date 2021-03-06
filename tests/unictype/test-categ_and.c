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
  uc_general_category_t ct =
    uc_general_category_and (
      uc_general_category_or (UC_LETTER, UC_DECIMAL_DIGIT_NUMBER),
      uc_general_category_or (UC_UPPERCASE_LETTER, UC_NUMBER));
  
  ASSERT (uc_is_general_category ('A', ct));
  ASSERT (!uc_is_general_category ('a', ct));
  ASSERT (uc_is_general_category ('7', ct));
  ASSERT (!uc_is_general_category (0x00B2, ct));

  return 0;
}
