/* Ordered set data type implemented by an array.
   Copyright (C) 2006 Free Software Foundation, Inc.
   Written by Bruno Haible <bruno@clisp.org>, 2006.

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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

/* Specification.  */
#include "gl_array_oset.h"

#include <stdlib.h>

#include "xalloc.h"

/* Checked size_t computations.  */
#include "xsize.h"

/* -------------------------- gl_oset_t Data Type -------------------------- */

/* Concrete gl_oset_impl type, valid for this file only.  */
struct gl_oset_impl
{
  struct gl_oset_impl_base base;
  /* An array of ALLOCATED elements, of which the first COUNT are used.
     0 <= COUNT <= ALLOCATED.  */
  const void **elements;
  size_t count;
  size_t allocated;
};

static gl_oset_t
gl_array_create_empty (gl_oset_implementation_t implementation,
		       gl_setelement_compar_fn compar_fn)
{
  struct gl_oset_impl *set =
    (struct gl_oset_impl *) xmalloc (sizeof (struct gl_oset_impl));

  set->base.vtable = implementation;
  set->base.compar_fn = compar_fn;
  set->elements = NULL;
  set->count = 0;
  set->allocated = 0;

  return set;
}

static size_t
gl_array_size (gl_oset_t set)
{
  return set->count;
}

static size_t
gl_array_indexof (gl_oset_t set, const void *elt)
{
  size_t count = set->count;

  if (count > 0)
    {
      gl_setelement_compar_fn compar = set->base.compar_fn;
      size_t low = 0;
      size_t high = count;

      /* At each loop iteration, low < high; for indices < low the values
	 are smaller than ELT; for indices >= high the values are greater
	 than ELT.  So, if the element occurs in the list, is at
	 low <= position < high.  */
      do
	{
	  size_t mid = low + (high - low) / 2; /* low <= mid < high */
	  int cmp = (compar != NULL
		     ? compar (set->elements[mid], elt)
		     : (set->elements[mid] > elt ? 1 :
		        set->elements[mid] < elt ? -1 : 0));

	  if (cmp < 0)
	    low = mid + 1;
	  else if (cmp > 0)
	    high = mid;
	  else /* cmp == 0 */
	    /* We have an element equal to ELT at index MID.  */
	    return mid;
	}
      while (low < high);
    }
  return (size_t)(-1);
}

static bool
gl_array_search (gl_oset_t set, const void *elt)
{
  return gl_array_indexof (set, elt) != (size_t)(-1);
}

/* Ensure that set->allocated > set->count.  */
static void
grow (gl_oset_t set)
{
  size_t new_allocated;
  size_t memory_size;
  const void **memory;

  new_allocated = xtimes (set->allocated, 2);
  new_allocated = xsum (new_allocated, 1);
  memory_size = xtimes (new_allocated, sizeof (const void *));
  if (size_overflow_p (memory_size))
    /* Overflow, would lead to out of memory.  */
    xalloc_die ();
  memory = (const void **) xrealloc (set->elements, memory_size);
  if (memory == NULL)
    /* Out of memory.  */
    xalloc_die ();
  set->elements = memory;
  set->allocated = new_allocated;
}

/* Add the given element ELT at the given position,
   0 <= position <= gl_oset_size (set).  */
static inline void
gl_array_add_at (gl_oset_t set, size_t position, const void *elt)
{
  size_t count = set->count;
  const void **elements;
  size_t i;

  if (count == set->allocated)
    grow (set);
  elements = set->elements;
  for (i = count; i > position; i--)
    elements[i] = elements[i - 1];
  elements[position] = elt;
  set->count = count + 1;
}

/* Remove the element at the given position,
   0 <= position < gl_oset_size (set).  */
static inline void
gl_array_remove_at (gl_oset_t set, size_t position)
{
  size_t count = set->count;
  const void **elements;
  size_t i;

  elements = set->elements;
  for (i = position + 1; i < count; i++)
    elements[i - 1] = elements[i];
  set->count = count - 1;
}

static bool
gl_array_add (gl_oset_t set, const void *elt)
{
  size_t count = set->count;
  size_t low = 0;

  if (count > 0)
    {
      gl_setelement_compar_fn compar = set->base.compar_fn;
      size_t high = count;

      /* At each loop iteration, low < high; for indices < low the values
	 are smaller than ELT; for indices >= high the values are greater
	 than ELT.  So, if the element occurs in the list, is at
	 low <= position < high.  */
      do
	{
	  size_t mid = low + (high - low) / 2; /* low <= mid < high */
	  int cmp = (compar != NULL
		     ? compar (set->elements[mid], elt)
		     : (set->elements[mid] > elt ? 1 :
			set->elements[mid] < elt ? -1 : 0));

	  if (cmp < 0)
	    low = mid + 1;
	  else if (cmp > 0)
	    high = mid;
	  else /* cmp == 0 */
	    return false;
	}
      while (low < high);
    }
  gl_array_add_at (set, low, elt);
  return true;
}

static bool
gl_array_remove (gl_oset_t set, const void *elt)
{
  size_t index = gl_array_indexof (set, elt);
  if (index != (size_t)(-1))
    {
      gl_array_remove_at (set, index);
      return true;
    }
  else
    return false;
}

static void
gl_array_free (gl_oset_t set)
{
  if (set->elements != NULL)
    free (set->elements);
  free (set);
}

/* --------------------- gl_oset_iterator_t Data Type --------------------- */

static gl_oset_iterator_t
gl_array_iterator (gl_oset_t set)
{
  gl_oset_iterator_t result;

  result.vtable = set->base.vtable;
  result.set = set;
  result.count = set->count;
  result.p = set->elements + 0;
  result.q = set->elements + set->count;

  return result;
}

static bool
gl_array_iterator_next (gl_oset_iterator_t *iterator, const void **eltp)
{
  gl_oset_t set = iterator->set;
  if (iterator->count != set->count)
    {
      if (iterator->count != set->count + 1)
	/* Concurrent modifications were done on the set.  */
	abort ();
      /* The last returned element was removed.  */
      iterator->count--;
      iterator->p--;
      iterator->q--;
    }
  if (iterator->p < iterator->q)
    {
      const void **p = (const void **) iterator->p;
      *eltp = *p;
      iterator->p = p + 1;
      return true;
    }
  else
    return false;
}

static void
gl_array_iterator_free (gl_oset_iterator_t *iterator)
{
}


const struct gl_oset_implementation gl_array_oset_implementation =
  {
    gl_array_create_empty,
    gl_array_size,
    gl_array_search,
    gl_array_add,
    gl_array_remove,
    gl_array_free,
    gl_array_iterator,
    gl_array_iterator_next,
    gl_array_iterator_free
  };