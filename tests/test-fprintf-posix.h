/* Test of POSIX compatible vsprintf() and sprintf() functions.
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

static void
test_function (int (*my_fprintf) (FILE *, const char *, ...))
{
  /* Here we don't test output that may be platform dependent.
     The bulk of the tests is done as part of the 'vasnprintf-posix' module.  */

  /* Test support of size specifiers as in C99.  */

  my_fprintf (stdout, "%ju %d\n", (uintmax_t) 12345671, 33, 44, 55);

  my_fprintf (stdout, "%zu %d\n", (size_t) 12345672, 33, 44, 55);

  my_fprintf (stdout, "%tu %d\n", (ptrdiff_t) 12345673, 33, 44, 55);

  /* Test the support of the 'a' and 'A' conversion specifier for hexadecimal
     output of floating-point numbers.  */

  /* Positive zero.  */
  my_fprintf (stdout, "%a %d\n", 0.0, 33, 44, 55);

  /* Negative zero.  */
  my_fprintf (stdout, "%a %d\n", -0.0, 33, 44, 55);

  /* Positive infinity.  */
  my_fprintf (stdout, "%a %d\n", 1.0 / 0.0, 33, 44, 55);

  /* Negative infinity.  */
  my_fprintf (stdout, "%a %d\n", -1.0 / 0.0, 33, 44, 55);

  /* NaN.  */
  my_fprintf (stdout, "%a %d\n", 0.0 / 0.0, 33, 44, 55);

  /* FLAG_ZERO with infinite number.  */
  my_fprintf (stdout, "%010a %d\n", 1.0 / 0.0, 33, 44, 55);

  /* FLAG_ZERO with NaN.  */
  my_fprintf (stdout, "%010a %d\n", 0.0 / 0.0, 33, 44, 55);

  /* Test the support of the POSIX/XSI format strings with positions.  */

  my_fprintf (stdout, "%2$d %1$d\n", 33, 55);
}