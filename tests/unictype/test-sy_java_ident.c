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
  ASSERT (uc_java_ident_category ('0') == UC_IDENTIFIER_VALID);
  ASSERT (uc_java_ident_category ('X') == UC_IDENTIFIER_START);
  ASSERT (uc_java_ident_category ('+') == UC_IDENTIFIER_INVALID);
  ASSERT (uc_java_ident_category (0x00B2) == UC_IDENTIFIER_INVALID);
  ASSERT (uc_java_ident_category (0x05DA) == UC_IDENTIFIER_START);

  ASSERT (uc_java_ident_category (0x001B) == UC_IDENTIFIER_IGNORABLE);
  ASSERT (uc_java_ident_category (0xE0001) == UC_IDENTIFIER_IGNORABLE);

  return 0;
}
