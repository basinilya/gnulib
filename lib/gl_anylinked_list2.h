/* Sequential list data type implemented by a linked list.
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

/* Common code of gl_linked_list.c and gl_linkedhash_list.c.  */

/* -------------------------- gl_list_t Data Type -------------------------- */

static gl_list_t
gl_linked_create_empty (gl_list_implementation_t implementation,
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
#if WITH_HASHTABLE
  list->table_size = 11;
  list->table =
    (gl_hash_entry_t *) xzalloc (list->table_size * sizeof (gl_hash_entry_t));
#endif
  list->root.next = &list->root;
  list->root.prev = &list->root;
  list->count = 0;

  return list;
}

static gl_list_t
gl_linked_create (gl_list_implementation_t implementation,
		  gl_listelement_equals_fn equals_fn,
		  gl_listelement_hashcode_fn hashcode_fn,
		  bool allow_duplicates,
		  size_t count, const void **contents)
{
  struct gl_list_impl *list =
    (struct gl_list_impl *) xmalloc (sizeof (struct gl_list_impl));
  gl_list_node_t tail;

  list->base.vtable = implementation;
  list->base.equals_fn = equals_fn;
  list->base.hashcode_fn = hashcode_fn;
  list->base.allow_duplicates = allow_duplicates;
#if WITH_HASHTABLE
  {
    size_t estimate = xsum (count, count / 2); /* 1.5 * count */
    if (estimate < 10)
      estimate = 10;
    list->table_size = next_prime (estimate);
    list->table =
      (gl_hash_entry_t *) xzalloc (list->table_size * sizeof (gl_hash_entry_t));
  }
#endif
  list->count = count;
  tail = &list->root;
  for (; count > 0; contents++, count--)
    {
      gl_list_node_t node =
	(struct gl_list_node_impl *)
	xmalloc (sizeof (struct gl_list_node_impl));

      node->value = *contents;
#if WITH_HASHTABLE
      node->h.hashcode =
	(list->base.hashcode_fn != NULL
	 ? list->base.hashcode_fn (node->value)
	 : (size_t)(uintptr_t) node->value);

      /* Add node to the hash table.  */
      add_to_bucket (list, node);
#endif

      /* Add node to the list.  */
      node->prev = tail;
      tail->next = node;
      tail = node;
    }
  tail->next = &list->root;
  list->root.prev = tail;

  return list;
}

static size_t
gl_linked_size (gl_list_t list)
{
  return list->count;
}

static const void *
gl_linked_node_value (gl_list_t list, gl_list_node_t node)
{
  return node->value;
}

static gl_list_node_t
gl_linked_next_node (gl_list_t list, gl_list_node_t node)
{
  return (node->next != &list->root ? node->next : NULL);
}

static gl_list_node_t
gl_linked_previous_node (gl_list_t list, gl_list_node_t node)
{
  return (node->prev != &list->root ? node->prev : NULL);
}

static const void *
gl_linked_get_at (gl_list_t list, size_t position)
{
  size_t count = list->count;
  gl_list_node_t node;

  if (!(position < count))
    /* Invalid argument.  */
    abort ();
  /* Here we know count > 0.  */
  if (position <= ((count - 1) / 2))
    {
      node = list->root.next;
      for (; position > 0; position--)
	node = node->next;
    }
  else
    {
      position = count - 1 - position;
      node = list->root.prev;
      for (; position > 0; position--)
	node = node->prev;
    }
  return node->value;
}

static gl_list_node_t
gl_linked_set_at (gl_list_t list, size_t position, const void *elt)
{
  size_t count = list->count;
  gl_list_node_t node;

  if (!(position < count))
    /* Invalid argument.  */
    abort ();
  /* Here we know count > 0.  */
  if (position <= ((count - 1) / 2))
    {
      node = list->root.next;
      for (; position > 0; position--)
	node = node->next;
    }
  else
    {
      position = count - 1 - position;
      node = list->root.prev;
      for (; position > 0; position--)
	node = node->prev;
    }
#if WITH_HASHTABLE
  if (elt != node->value)
    {
      size_t new_hashcode =
	(list->base.hashcode_fn != NULL
	 ? list->base.hashcode_fn (elt)
	 : (size_t)(uintptr_t) elt);

      if (new_hashcode != node->h.hashcode)
	{
	  remove_from_bucket (list, node);
	  node->value = elt;
	  node->h.hashcode = new_hashcode;
	  add_to_bucket (list, node);
	}
      else
	node->value = elt;
    }
#else
  node->value = elt;
#endif
  return node;
}

static gl_list_node_t
gl_linked_search (gl_list_t list, const void *elt)
{
#if WITH_HASHTABLE
  size_t hashcode =
    (list->base.hashcode_fn != NULL
     ? list->base.hashcode_fn (elt)
     : (size_t)(uintptr_t) elt);
  size_t index = hashcode % list->table_size;
  gl_listelement_equals_fn equals = list->base.equals_fn;

  if (!list->base.allow_duplicates)
    {
      /* Look for the first match in the hash bucket.  */
      gl_list_node_t node;

      for (node = (gl_list_node_t) list->table[index];
	   node != NULL;
	   node = (gl_list_node_t) node->h.hash_next)
	if (node->h.hashcode == hashcode
	    && (equals != NULL
		? equals (elt, node->value)
		: elt == node->value))
	  return node;
      return NULL;
    }
  else
    {
      /* Look whether there is more than one match in the hash bucket.  */
      bool multiple_matches = false;
      gl_list_node_t first_match = NULL;
      gl_list_node_t node;

      for (node = (gl_list_node_t) list->table[index];
	   node != NULL;
	   node = (gl_list_node_t) node->h.hash_next)
	if (node->h.hashcode == hashcode
	    && (equals != NULL
		? equals (elt, node->value)
		: elt == node->value))
	  {
	    if (first_match == NULL)
	      first_match = node;
	    else
	      {
		multiple_matches = true;
		break;
	      }
	  }
      if (!multiple_matches)
	return first_match;
      else
	{
	  /* We need the match with the smallest index.  But we don't have
	     a fast mapping node -> index.  So we have to walk the list.  */
	  for (node = list->root.next; node != &list->root; node = node->next)
	    if (node->h.hashcode == hashcode
		&& (equals != NULL
		    ? equals (elt, node->value)
		    : elt == node->value))
	      return node;
	  /* We know there are at least two matches.  */
	  abort ();
	}
    }
#else
  gl_listelement_equals_fn equals = list->base.equals_fn;
  gl_list_node_t node;

  if (equals != NULL)
    {
      for (node = list->root.next; node != &list->root; node = node->next)
	if (equals (elt, node->value))
	  return node;
    }
  else
    {
      for (node = list->root.next; node != &list->root; node = node->next)
	if (elt == node->value)
	  return node;
    }
  return NULL;
#endif
}

static size_t
gl_linked_indexof (gl_list_t list, const void *elt)
{
#if WITH_HASHTABLE
  /* Here the hash table doesn't help much.  It only allows us to minimize
     the number of equals() calls, by looking up first the node and then
     its index.  */
  size_t hashcode =
    (list->base.hashcode_fn != NULL
     ? list->base.hashcode_fn (elt)
     : (size_t)(uintptr_t) elt);
  size_t index = hashcode % list->table_size;
  gl_listelement_equals_fn equals = list->base.equals_fn;
  gl_list_node_t node;

  /* First step: Look up the node.  */
  if (!list->base.allow_duplicates)
    {
      /* Look for the first match in the hash bucket.  */
      for (node = (gl_list_node_t) list->table[index];
	   node != NULL;
	   node = (gl_list_node_t) node->h.hash_next)
	if (node->h.hashcode == hashcode
	    && (equals != NULL
		? equals (elt, node->value)
		: elt == node->value))
	  break;
    }
  else
    {
      /* Look whether there is more than one match in the hash bucket.  */
      bool multiple_matches = false;
      gl_list_node_t first_match = NULL;

      for (node = (gl_list_node_t) list->table[index];
	   node != NULL;
	   node = (gl_list_node_t) node->h.hash_next)
	if (node->h.hashcode == hashcode
	    && (equals != NULL
		? equals (elt, node->value)
		: elt == node->value))
	  {
	    if (first_match == NULL)
	      first_match = node;
	    else
	      {
		multiple_matches = true;
		break;
	      }
	  }
      if (multiple_matches)
	{
	  /* We need the match with the smallest index.  But we don't have
	     a fast mapping node -> index.  So we have to walk the list.  */
	  size_t index;

	  for (node = list->root.next, index = 0;
	       node != &list->root;
	       node = node->next, index++)
	    if (node->h.hashcode == hashcode
		&& (equals != NULL
		    ? equals (elt, node->value)
		    : elt == node->value))
	      return index;
	  /* We know there are at least two matches.  */
	  abort ();
	}
      node = first_match;
    }

  /* Second step: Look up the index of the node.  */
  if (node == NULL)
    return (size_t)(-1);
  else
    {
      size_t index = 0;

      for (; node->prev != &list->root; node = node->prev)
	index++;

      return index;
    }
#else
  gl_listelement_equals_fn equals = list->base.equals_fn;
  gl_list_node_t node;

  if (equals != NULL)
    {
      size_t index;
      for (node = list->root.next, index = 0;
	   node != &list->root;
	   node = node->next, index++)
	if (equals (elt, node->value))
	  return index;
    }
  else
    {
      size_t index;
      for (node = list->root.next, index = 0;
	   node != &list->root;
	   node = node->next, index++)
	if (elt == node->value)
	  return index;
    }
  return (size_t)(-1);
#endif
}

static gl_list_node_t
gl_linked_add_first (gl_list_t list, const void *elt)
{
  gl_list_node_t node =
    (struct gl_list_node_impl *) xmalloc (sizeof (struct gl_list_node_impl));

  node->value = elt;
#if WITH_HASHTABLE
  node->h.hashcode =
    (list->base.hashcode_fn != NULL
     ? list->base.hashcode_fn (node->value)
     : (size_t)(uintptr_t) node->value);

  /* Add node to the hash table.  */
  add_to_bucket (list, node);
#endif

  /* Add node to the list.  */
  node->prev = &list->root;
  node->next = list->root.next;
  node->next->prev = node;
  list->root.next = node;
  list->count++;

#if WITH_HASHTABLE
  hash_resize_after_add (list);
#endif

  return node;
}

static gl_list_node_t
gl_linked_add_last (gl_list_t list, const void *elt)
{
  gl_list_node_t node =
    (struct gl_list_node_impl *) xmalloc (sizeof (struct gl_list_node_impl));

  node->value = elt;
#if WITH_HASHTABLE
  node->h.hashcode =
    (list->base.hashcode_fn != NULL
     ? list->base.hashcode_fn (node->value)
     : (size_t)(uintptr_t) node->value);

  /* Add node to the hash table.  */
  add_to_bucket (list, node);
#endif

  /* Add node to the list.  */
  node->next = &list->root;
  node->prev = list->root.prev;
  node->prev->next = node;
  list->root.prev = node;
  list->count++;

#if WITH_HASHTABLE
  hash_resize_after_add (list);
#endif

  return node;
}

static gl_list_node_t
gl_linked_add_before (gl_list_t list, gl_list_node_t node, const void *elt)
{
  gl_list_node_t new_node =
    (struct gl_list_node_impl *) xmalloc (sizeof (struct gl_list_node_impl));

  new_node->value = elt;
#if WITH_HASHTABLE
  new_node->h.hashcode =
    (list->base.hashcode_fn != NULL
     ? list->base.hashcode_fn (new_node->value)
     : (size_t)(uintptr_t) new_node->value);

  /* Add new_node to the hash table.  */
  add_to_bucket (list, new_node);
#endif

  /* Add new_node to the list.  */
  new_node->next = node;
  new_node->prev = node->prev;
  new_node->prev->next = new_node;
  node->prev = new_node;
  list->count++;

#if WITH_HASHTABLE
  hash_resize_after_add (list);
#endif

  return new_node;
}

static gl_list_node_t
gl_linked_add_after (gl_list_t list, gl_list_node_t node, const void *elt)
{
  gl_list_node_t new_node =
    (struct gl_list_node_impl *) xmalloc (sizeof (struct gl_list_node_impl));

  new_node->value = elt;
#if WITH_HASHTABLE
  new_node->h.hashcode =
    (list->base.hashcode_fn != NULL
     ? list->base.hashcode_fn (new_node->value)
     : (size_t)(uintptr_t) new_node->value);

  /* Add new_node to the hash table.  */
  add_to_bucket (list, new_node);
#endif

  /* Add new_node to the list.  */
  new_node->prev = node;
  new_node->next = node->next;
  new_node->next->prev = new_node;
  node->next = new_node;
  list->count++;

#if WITH_HASHTABLE
  hash_resize_after_add (list);
#endif

  return new_node;
}

static gl_list_node_t
gl_linked_add_at (gl_list_t list, size_t position, const void *elt)
{
  size_t count = list->count;
  gl_list_node_t new_node;

  if (!(position <= count))
    /* Invalid argument.  */
    abort ();

  new_node =
    (struct gl_list_node_impl *) xmalloc (sizeof (struct gl_list_node_impl));
  new_node->value = elt;
#if WITH_HASHTABLE
  new_node->h.hashcode =
    (list->base.hashcode_fn != NULL
     ? list->base.hashcode_fn (new_node->value)
     : (size_t)(uintptr_t) new_node->value);

  /* Add new_node to the hash table.  */
  add_to_bucket (list, new_node);
#endif

  /* Add new_node to the list.  */
  if (position <= (count / 2))
    {
      gl_list_node_t node;

      node = &list->root;
      for (; position > 0; position--)
	node = node->next;
      new_node->prev = node;
      new_node->next = node->next;
      new_node->next->prev = new_node;
      node->next = new_node;
    }
  else
    {
      gl_list_node_t node;

      position = count - position;
      node = &list->root;
      for (; position > 0; position--)
	node = node->prev;
      new_node->next = node;
      new_node->prev = node->prev;
      new_node->prev->next = new_node;
      node->prev = new_node;
    }
  list->count++;

#if WITH_HASHTABLE
  hash_resize_after_add (list);
#endif

  return new_node;
}

static bool
gl_linked_remove_node (gl_list_t list, gl_list_node_t node)
{
  gl_list_node_t prev;
  gl_list_node_t next;

#if WITH_HASHTABLE
  /* Remove node from the hash table.  */
  remove_from_bucket (list, node);
#endif

  /* Remove node from the list.  */
  prev = node->prev;
  next = node->next;

  prev->next = next;
  next->prev = prev;
  list->count--;

  free (node);
  return true;
}

static bool
gl_linked_remove_at (gl_list_t list, size_t position)
{
  size_t count = list->count;
  gl_list_node_t removed_node;

  if (!(position < count))
    /* Invalid argument.  */
    abort ();
  /* Here we know count > 0.  */
  if (position <= ((count - 1) / 2))
    {
      gl_list_node_t node;
      gl_list_node_t after_removed;

      node = &list->root;
      for (; position > 0; position--)
	node = node->next;
      removed_node = node->next;
      after_removed = node->next->next;
      node->next = after_removed;
      after_removed->prev = node;
    }
  else
    {
      gl_list_node_t node;
      gl_list_node_t before_removed;

      position = count - 1 - position;
      node = &list->root;
      for (; position > 0; position--)
	node = node->prev;
      removed_node = node->prev;
      before_removed = node->prev->prev;
      node->prev = before_removed;
      before_removed->next = node;
    }
#if WITH_HASHTABLE
  remove_from_bucket (list, removed_node);
#endif
  list->count--;

  free (removed_node);
  return true;
}

static bool
gl_linked_remove (gl_list_t list, const void *elt)
{
  gl_list_node_t node = gl_linked_search (list, elt);

  if (node != NULL)
    return gl_linked_remove_node (list, node);
  else
    return false;
}

static void
gl_linked_list_free (gl_list_t list)
{
  gl_list_node_t node;

  for (node = list->root.next; node != &list->root; )
    {
      gl_list_node_t next = node->next;
      free (node);
      node = next;
    }
#if WITH_HASHTABLE
  free (list->table);
#endif
  free (list);
}

/* --------------------- gl_list_iterator_t Data Type --------------------- */

static gl_list_iterator_t
gl_linked_iterator (gl_list_t list)
{
  gl_list_iterator_t result;

  result.vtable = list->base.vtable;
  result.list = list;
  result.p = list->root.next;
  result.q = &list->root;

  return result;
}

static gl_list_iterator_t
gl_linked_iterator_from_to (gl_list_t list,
			    size_t start_index, size_t end_index)
{
  gl_list_iterator_t result;
  size_t n1, n2, n3;

  if (!(start_index <= end_index && end_index <= list->count))
    /* Invalid arguments.  */
    abort ();
  result.vtable = list->base.vtable;
  result.list = list;
  n1 = start_index;
  n2 = end_index - start_index;
  n3 = list->count - end_index;
  /* Find the maximum among n1, n2, n3, so as to reduce the number of
     loop iterations to n1 + n2 + n3 - max(n1,n2,n3).  */
  if (n1 > n2 && n1 > n3)
    {
      /* n1 is the maximum, use n2 and n3.  */
      gl_list_node_t node;
      size_t i;

      node = &list->root;
      for (i = n3; i > 0; i--)
	node = node->prev;
      result.q = node;
      for (i = n2; i > 0; i--)
	node = node->prev;
      result.p = node;
    }
  else if (n2 > n3)
    {
      /* n2 is the maximum, use n1 and n3.  */
      gl_list_node_t node;
      size_t i;

      node = list->root.next;
      for (i = n1; i > 0; i--)
	node = node->next;
      result.p = node;

      node = &list->root;
      for (i = n3; i > 0; i--)
	node = node->prev;
      result.q = node;
    }
  else
    {
      /* n3 is the maximum, use n1 and n2.  */
      gl_list_node_t node;
      size_t i;

      node = list->root.next;
      for (i = n1; i > 0; i--)
	node = node->next;
      result.p = node;
      for (i = n2; i > 0; i--)
	node = node->next;
      result.q = node;
    }

  return result;
}

static bool
gl_linked_iterator_next (gl_list_iterator_t *iterator,
			 const void **eltp, gl_list_node_t *nodep)
{
  if (iterator->p != iterator->q)
    {
      gl_list_node_t node = (gl_list_node_t) iterator->p;
      *eltp = node->value;
      if (nodep != NULL)
	*nodep = node;
      iterator->p = node->next;
      return true;
    }
  else
    return false;
}

static void
gl_linked_iterator_free (gl_list_iterator_t *iterator)
{
}

/* ---------------------- Sorted gl_list_t Data Type ---------------------- */

static gl_list_node_t
gl_linked_sortedlist_search (gl_list_t list, gl_listelement_compar_fn compar,
			     const void *elt)
{
  gl_list_node_t node;

  for (node = list->root.next; node != &list->root; node = node->next)
    {
      int cmp = compar (node->value, elt);

      if (cmp > 0)
	break;
      if (cmp == 0)
	return node;
    }
  return NULL;
}

static size_t
gl_linked_sortedlist_indexof (gl_list_t list, gl_listelement_compar_fn compar,
			      const void *elt)
{
  gl_list_node_t node;
  size_t index;

  for (node = list->root.next, index = 0;
       node != &list->root;
       node = node->next, index++)
    {
      int cmp = compar (node->value, elt);

      if (cmp > 0)
	break;
      if (cmp == 0)
	return index;
    }
  return (size_t)(-1);
}

static gl_list_node_t
gl_linked_sortedlist_add (gl_list_t list, gl_listelement_compar_fn compar,
			  const void *elt)
{
  gl_list_node_t node;

  for (node = list->root.next; node != &list->root; node = node->next)
    if (compar (node->value, elt) >= 0)
      return gl_linked_add_before (list, node, elt);
  return gl_linked_add_last (list, elt);
}

static bool
gl_linked_sortedlist_remove (gl_list_t list, gl_listelement_compar_fn compar,
			     const void *elt)
{
  gl_list_node_t node;

  for (node = list->root.next; node != &list->root; node = node->next)
    {
      int cmp = compar (node->value, elt);

      if (cmp > 0)
	break;
      if (cmp == 0)
	return gl_linked_remove_node (list, node);
    }
  return false;
}