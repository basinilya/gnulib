/*
 * Copyright (C) 2004, 2007 Free Software Foundation
 * Written by Simon Josefsson
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#include <config.h>

#include <stdio.h>
#include <string.h>
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

int
main (int argc, char *argv[])
{
  {
    const char input[] = "foo";
    const char *result = memmem (input, strlen (input), "", 0);
    ASSERT (result == input);
  }

  {
    const char input[] = "foo";
    const char *result = memmem (input, strlen (input), "o", 1);
    ASSERT (result == input + 1);
  }

  {
    const char input[] = "ABC ABCDAB ABCDABCDABDE";
    const char *result = memmem (input, strlen (input), "ABCDABD", 7);
    ASSERT (result == input + 15);
  }

  {
    const char input[] = "ABC ABCDAB ABCDABCDABDE";
    const char *result = memmem (input, strlen (input), "ABCDABE", 7);
    ASSERT (result == NULL);
  }

  /* Check that length 0 does not dereference NULL.  */
  {
    const char *result = memmem (NULL, 0, "foo", 3);
    ASSERT (result == NULL);
  }

  {
    const char input[] = "foo";
    const char *result = memmem (input, strlen (input), NULL, 0);
    ASSERT (result == input);
  }

  /* Check that a very long haystack is handled quickly if the needle is
     short and occurs near the beginning.  */
  {
    size_t repeat = 10000;
    size_t m = 1000000;
    char *needle =
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"
      "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
    size_t n = strlen (needle);
    char *haystack = (char *) malloc (m + 1);
    if (haystack != NULL)
      {
	memset (haystack, 'A', m);
	haystack[0] = 'B';

	for (; repeat > 0; repeat--)
	  {
	    ASSERT (memmem (haystack, m, needle, n) == haystack + 1);
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
    size_t n = strlen (haystack);
    char *needle = (char *) malloc (m + 1);
    if (needle != NULL)
      {
	memset (needle, 'A', m);

	for (; repeat > 0; repeat--)
	  {
	    ASSERT (memmem (haystack, n, needle, m) == NULL);
	  }

	free (needle);
      }
  }

  /* Check that the asymptotic worst-case complexity is not quadratic.  */
  {
    size_t m = 1000000;
    char *haystack = (char *) malloc (2 * m + 1);
    char *needle = (char *) malloc (m + 1);
    if (haystack != NULL && needle != NULL)
      {
	const char *result;

	memset (haystack, 'A', 2 * m);
	haystack[2 * m] = 'B';

	memset (needle, 'A', m);
	needle[m] = 'B';

	result = memmem (haystack, 2 * m + 1, needle, m + 1);
	ASSERT (result == haystack + m);
      }
    if (needle != NULL)
      free (needle);
    if (haystack != NULL)
      free (haystack);
  }

  return 0;
}
