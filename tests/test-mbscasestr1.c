/* Test of case-insensitive searching in a string.
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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <string.h>

#include <stdlib.h>

#define ASSERT(expr) if (!(expr)) abort ();

int
main ()
{
  /* This test is executed in the C locale.  */

  {
    const char input[] = "foo";
    const char *result = mbscasestr (input, "");
    ASSERT (result == input);
  }

  {
    const char input[] = "foo";
    const char *result = mbscasestr (input, "O");
    ASSERT (result == input + 1);
  }

  {
    const char input[] = "ABC ABCDAB ABCDABCDABDE";
    const char *result = mbscasestr (input, "ABCDaBD");
    ASSERT (result == input + 15);
  }

  {
    const char input[] = "ABC ABCDAB ABCDABCDABDE";
    const char *result = mbscasestr (input, "ABCDaBE");
    ASSERT (result == NULL);
  }

  /* Check that a very long haystack is handled quickly if the needle is
     short and occurs near the beginning.  */
  {
    size_t repeat = 10000;
    size_t m = 1000000;
    char *needle =
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaAaaaaaaAAAAaaaaaaa"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    char *haystack = (char *) malloc (m + 1);
    if (haystack != NULL)
      {
	memset (haystack, 'A', m);
	haystack[0] = 'B';
	haystack[m] = '\0';

	for (; repeat > 0; repeat--)
	  {
	    ASSERT (mbscasestr (haystack, needle) == haystack + 1);
	  }

	free (haystack);
      }
  }

  /* Check that a very long needle is discarded quickly if the haystack is
     short.  */
  {
    size_t repeat = 10000;
    size_t m = 1000000;
    char *haystack =
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "ABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABABAB";
    char *needle = (char *) malloc (m + 1);
    if (needle != NULL)
      {
	memset (needle, 'A', m);
	needle[m] = '\0';

	for (; repeat > 0; repeat--)
	  {
	    ASSERT (mbscasestr (haystack, needle) == NULL);
	  }

	free (needle);
      }
  }

  /* Check that the asymptotic worst-case complexity is not quadratic.  */
  {
    size_t m = 1000000;
    char *haystack = (char *) malloc (2 * m + 2);
    char *needle = (char *) malloc (m + 2);
    if (haystack != NULL && needle != NULL)
      {
	const char *result;

	memset (haystack, 'A', 2 * m);
	haystack[2 * m] = 'B';
	haystack[2 * m + 1] = '\0';

	memset (needle, 'a', m);
	needle[m] = 'B';
	needle[m + 1] = '\0';

	result = mbscasestr (haystack, needle);
	ASSERT (result == haystack + m);
      }
    if (needle != NULL)
      free (needle);
    if (haystack != NULL)
      free (haystack);
  }

  return 0;
}