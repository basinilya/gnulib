/* Sequential list data type implemented by an array.
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
#include "gl_array_list.h"

#include <stdlib.h>
/* Get memcpy.  */
#include <string.h>

#include "xalloc.h"

/* Checked size_t computations.  */
#include "xsize.h"

#ifndef uintptr_t
# define uintptr_t unsigned long
#endif

/* -------------------------- gl_list_t Data Type -------------------------- */

/* Concrete gl_list_impl type, valid for this file only.  */
struct gl_list_impl
{
  struct gl_list_impl_base base;
  /* An array of ALLOCATED elements, of which the first COUNT are used.
     0 <= COUNT <= ALLOCATED.  */
  const void **elements;
  size_t count;
  size_t allocated;
};

/* struct gl_list_node_impl doesn't exist here.  The pointers are actually
   indices + 1.  */
#define INDEX_TO_NODE(index) (gl_list_node_t)(uintptr_t)(size_t)((index) + 1)
#define NODE_TO_INDEX(node) ((uintptr_t)(node) - 1)

static gl_list_t
gl_array_create_empty (gl_list_implementation_t implementation,
		       gl_listelement_equals_fn equals_fn,
		       gl_listelement_hashcode_fn hashcode_fn,
		       bool allow_duplicates)
{
  struct gl_list_impl *list =
    (struct gl_list_impl *) xmalloc (sizeof (struct gl_list_impl));

  list->base.vtable = implementation;
  list->base.equals_fn = equals_fn;
  list->base.hashcode_fn = hashcode_fn;
  list->base.allow_duplicates = allow_duplicates;
  list->elements = NULL;
  list->count = 0;
  list->allocated = 0;

  return list;
}

static gl_list_t
gl_array_create (gl_list_implementation_t implementation,
		 gl_listelement_equals_fn equals_fn,
		 gl_listelement_hashcode_fn hashcode_fn,
		 bool allow_duplicates,
		 size_t count, const void **contents)
{
  struct gl_list_impl *list =
    (struct gl_list_impl *) xmalloc (sizeof (struct gl_list_impl));

  list->base.vtable = implementation;
  list->base.equals_fn = equals_fn;
  list->base.hashcode_fn = hashcode_fn;
  list->base.allow_duplicates = allow_duplicates;
  if (count > 0)
    {
      list->elements =
	(const void **) xmalloc (count * sizeof (const void *));
      memcpy (list->elements, contents, count * sizeof (const void *));
    }
  else
    list->elements = NULL;
  list->count = count;
  list->allocated = count;

  return list;
}

static size_t
gl_array_size (gl_list_t list)
{
  return list->count;
}

static const void *
gl_array_node_value (gl_list_t list, gl_list_node_t node)
{
  uintptr_t index = NODE_TO_INDEX (node);
  if (!(index < list->count))
    /* Invalid argument.  */
    abort ();
  return list->elements[index];
}

static gl_list_node_t
gl_array_next_node (gl_list_t list, gl_list_node_t node)
{
  uintptr_t index = NODE_TO_INDEX (node);
  if (!(index < list->count))
    /* Invalid argument.  */
    abort ();
  index++;
  if (index < list->count)
    return INDEX_TO_NODE (index);
  else
    return NULL;
}

static gl_list_node_t
gl_array_previous_node (gl_list_t list, gl_list_node_t node)
{
  uintptr_t index = NODE_TO_INDEX (node);
  if (!(index < list->count))
    /* Invalid argument.  */
    abort ();
  if (index > 0)
    return INDEX_TO_NODE (index - 1);
  else
    return NULL;
}

static const void *
gl_array_get_at (gl_list_t list, size_t position)
{
  size_t count = list->count;

  if (!(position < count))
    /* Invalid argument.  */
    abort ();
  return list->elements[position];
}

static gl_list_node_t
gl_array_set_at (gl_list_t list, size_t position, const void *elt)
{
  size_t count = list->count;

  if (!(position < count))
    /* Invalid argument.  */
    abort ();
  list->elements[position] = elt;
  return INDEX_TO_NODE (position);
}

static size_t
gl_array_indexof (gl_list_t list, const void *elt)
{
  size_t count = list->count;
  if (count > 0)
    {
      gl_listelement_equals_fn equals = list->base.equals_fn;
      if (equals != NULL)
	{
	  size_t i;

	  for (i = 0;;)
	    {
	      if (equals (elt, list->elements[i]))
		return i;
	      i++;
	      if (i == count)
		break;
	    }
	}
      else
	{
	  size_t i;

	  for (i = 0;;)
	    {
	      if (elt == list->elements[i])
		return i;
	      i++;
	      if (i == count)
		break;
	    }
	}
    }
  return (size_t)(-1);
}

static gl_list_node_t
gl_array_search (gl_list_t list, const void *elt)
{
  size_t index = gl_array_indexof (list, elt);
  return INDEX_TO_NODE (index);
}

/* Ensure that list->allocated > list->count.  */
static void
grow (gl_list_t list)
{
  size_t new_allocated;
  size_t memory_size;
  const void **memory;

  new_allocated = xtimes (list->allocated, 2);
  new_allocated = xsum (new_allocated, 1);
  memory_size = xtimes (new_allocated, sizeof (const void *));
  if (size_overflow_p (memory_size))
    /* Overflow, would lead to out of memory.  */
    xalloc_die ();
  memory = (const void **) xrealloc (list->elements, memory_size);
  if (memory == NULL)
    /* Out of memory.  */
    xalloc_die ();
  list->elements = memory;
  list->allocated = new_allocated;
}

static gl_list_node_t
gl_array_add_first (gl_list_t list, const void *elt)
{
  size_t count = list->count;
  const void **elements;
  size_t i;

  if (count == list->allocated)
    grow (list);
  elements = list->elements;
  for (i = count; i > 0; i--)
    elements[i] = elements[i - 1];
  elements[0] = elt;
  list->count = count + 1;
  return INDEX_TO_NODE (0);
}

static gl_list_node_t
gl_array_add_last (gl_list_t list, const void *elt)
{
  size_t count = list->count;

  if (count == list->allocated)
    grow (list);
  list->elements[count] = elt;
  list->count = count + 1;
  return INDEX_TO_NODE (count);
}

static gl_list_node_t
gl_array_add_before (gl_list_t list, gl_list_node_t node, const void *elt)
{
  size_t count = list->count;
  uintptr_t index = NODE_TO_INDEX (node);
  size_t position;
  const void **elements;
  size_t i;

  if (!(index < count))
    /* Invalid argument.  */
    abort ();
  position = index;
  if (count == list->allocated)
    grow (list);
  elements = list->elements;
  for (i = count; i > position; i--)
    elements[i] = elements[i - 1];
  elements[position] = elt;
  list->count = count + 1;
  return INDEX_TO_NODE (position);
}

static gl_list_node_t
gl_array_add_after (gl_list_t list, gl_list_node_t node, const void *elt)
{
  size_t count = list->count;
  uintptr_t index = NODE_TO_INDEX (node);
  size_t position;
  const void **elements;
  size_t i;

  if (!(index < count))
    /* Invalid argument.  */
    abort ();
  position = index + 1;
  if (count == list->allocated)
    grow (list);
  elements = list->elements;
  for (i = count; i > position; i--)
    elements[i] = elements[i - 1];
  elements[position] = elt;
  list->count = count + 1;
  return INDEX_TO_NODE (position);
}

static gl_list_node_t
gl_array_add_at (gl_list_t list, size_t position, const void *elt)
{
  size_t count = list->count;
  const void **elements;
  size_t i;

  if (!(position <= count))
    /* Invalid argument.  */
    abort ();
  if (count == list->allocated)
    grow (list);
  elements = list->elements;
  for (i = count; i > position; i--)
    elements[i] = elements[i - 1];
  elements[position] = elt;
  list->count = count + 1;
  return INDEX_TO_NODE (position);
}

static bool
gl_array_remove_node (gl_list_t list, gl_list_node_t node)
{
  size_t count = list->count;
  uintptr_t index = NODE_TO_INDEX (node);
  size_t position;
  const void **elements;
  size_t i;

  if (!(index < count))
    /* Invalid argument.  */
    abort ();
  position = index;
  elements = list->elements;
  for (i = position + 1; i < count; i++)
    elements[i - 1] = elements[i];
  list->count = count - 1;
  return true;
}

static bool
gl_array_remove_at (gl_list_t list, size_t position)
{
  size_t count = list->count;
  const void **elements;
  size_t i;

  if (!(position < count))
    /* Invalid argument.  */
    abort ();
  elements = list->elements;
  for (i = position + 1; i < count; i++)
    elements[i - 1] = elements[i];
  list->count = count - 1;
  return true;
}

static bool
gl_array_remove (gl_list_t list, const void *elt)
{
  size_t position = gl_array_indexof (list, elt);
  if (position == (size_t)(-1))
    return false;
  else
    return gl_array_remove_at (list, position);
}

static void
gl_array_list_free (gl_list_t list)
{
  if (list->elements != NULL)
    free (list->elements);
  free (list);
}

/* --------------------- gl_list_iterator_t Data Type --------------------- */

static gl_list_iterator_t
gl_array_iterator (gl_list_t list)
{
  gl_list_iterator_t result;

  result.vtable = list->base.vtable;
  result.list = list;
  result.count = list->count;
  result.p = list->elements + 0;
  result.q = list->elements + list->count;

  return result;
}

static gl_list_iterator_t
gl_array_iterator_from_to (gl_list_t list, size_t start_index, size_t end_index)
{
  gl_list_iterator_t result;

  if (!(start_index <= end_index && end_index <= list->count))
    /* Invalid arguments.  */
    abort ();
  result.vtable = list->base.vtable;
  result.list = list;
  result.count = list->count;
  result.p = list->elements + start_index;
  result.q = list->elements + end_index;

  return result;
}

static bool
gl_array_iterator_next (gl_list_iterator_t *iterator,
			const void **eltp, gl_list_node_t *nodep)
{
  gl_list_t list = iterator->list;
  if (iterator->count != list->count)
    {
      if (iterator->count != list->count + 1)
	/* Concurrent modifications were done on the list.  */
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
      if (nodep != NULL)
	*nodep = INDEX_TO_NODE (p - list->elements);
      iterator->p = p + 1;
      return true;
    }
  else
    return false;
}

static void
gl_array_iterator_free (gl_list_iterator_t *iterator)
{
}

/* ---------------------- Sorted gl_list_t Data Type ---------------------- */

static size_t
gl_array_sortedlist_indexof (gl_list_t list, gl_listelement_compar_fn compar,
			     const void *elt)
{
  size_t count = list->count;

  if (count > 0)
    {
      size_t low = 0;
      size_t high = count;

      /* At each loop iteration, low < high; for indices < low the values
	 are smaller than ELT; for indices >= high the values are greater
	 than ELT.  So, if the element occurs in the list, is at
	 low <= position < high.  */
      do
	{
	  size_t mid = low + (high - low) / 2; /* low <= mid < high */
	  int cmp = compar (list->elements[mid], elt);

	  if (cmp < 0)
	    low = mid + 1;
	  else if (cmp > 0)
	    high = mid;
	  else /* cmp == 0 */
	    {
	      /* We have an element equal to ELT at index MID.  But we need
		 the minimal such index.  */
	      high = mid;
	      /* At each loop iteration, low <= high and
		   compar (list->elements[high], elt) == 0,
		 and we know that the first occurrence of the element is at
		 low <= position <= high.  */
	      while (low < high)
		{
		  size_t mid2 = low + (high - low) / 2; /* low <= mid < high */
		  int cmp2 = compar (list->elements[mid2], elt);

		  if (cmp2 < 0)
		    low = mid2 + 1;
		  else if (cmp2 > 0)
		    /* The list was not sorted.  */
		    abort ();
		  else /* cmp2 == 0 */
		    {
		      if (mid2 == low)
			break;
		      high = mid2 - 1;
		    }
		}
	      return low;
	    }
	}
      while (low < high);
      /* Here low == high.  */
    }
  return (size_t)(-1);
}

static gl_list_node_t
gl_array_sortedlist_search (gl_list_t list, gl_listelement_compar_fn compar,
			    const void *elt)
{
  size_t index = gl_array_sortedlist_indexof (list, compar, elt);
  return INDEX_TO_NODE (index);
}

static gl_list_node_t
gl_array_sortedlist_add (gl_list_t list, gl_listelement_compar_fn compar,
			 const void *elt)
{
  size_t count = list->count;
  size_t low = 0;
  size_t high = count;

  /* At each loop iteration, low <= high; for indices < low the values are
     smaller than ELT; for indices >= high the values are greater than ELT.  */
  while (low < high)
    {
      size_t mid = low + (high - low) / 2; /* low <= mid < high */
      int cmp = compar (list->elements[mid], elt);

      if (cmp < 0)
	low = mid + 1;
      else if (cmp > 0)
	high = mid;
      else /* cmp == 0 */
	{
	  low = mid;
	  break;
	}
    }
  return gl_array_add_at (list, low, elt);
}

static bool
gl_array_sortedlist_remove (gl_list_t list, gl_listelement_compar_fn compar,
			    const void *elt)
{
  size_t index = gl_array_sortedlist_indexof (list, compar, elt);
  if (index == (size_t)(-1))
    return false;
  else
    return gl_array_remove_at (list, index);
}


const struct gl_list_implementation gl_array_list_implementation =
  {
    gl_array_create_empty,
    gl_array_create,
    gl_array_size,
    gl_array_node_value,
    gl_array_next_node,
    gl_array_previous_node,
    gl_array_get_at,
    gl_array_set_at,
    gl_array_search,
    gl_array_indexof,
    gl_array_add_first,
    gl_array_add_last,
    gl_array_add_before,
    gl_array_add_after,
    gl_array_add_at,
    gl_array_remove_node,
    gl_array_remove_at,
    gl_array_remove,
    gl_array_list_free,
    gl_array_iterator,
    gl_array_iterator_from_to,
    gl_array_iterator_next,
    gl_array_iterator_free,
    gl_array_sortedlist_search,
    gl_array_sortedlist_indexof,
    gl_array_sortedlist_add,
    gl_array_sortedlist_remove
  };