/* Sequential list data type implemented by a hash table with a binary tree.
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

/* Common code of gl_avltreehash_list.c and gl_rbtreehash_list.c.  */

/* Hash table entry representing the same value at more than one position.  */
struct gl_multiple_nodes
{
  struct gl_hash_entry h;           /* hash table entry fields; must be first */
  void *magic;                      /* used to distinguish from single node */
  gl_oset_t nodes;                  /* set of nodes, sorted by position */
};
/* A value that cannot occur at the corresponding field (->left) in
   gl_list_node_impl.  */
#define MULTIPLE_NODES_MAGIC  (void *) -1

/* Resize the hash table if needed, after list->count was incremented.  */
static inline void
hash_resize_after_add (gl_list_t list)
{
  size_t count = (list->root != 0 ? list->root->branch_size : 0);
  size_t estimate = xsum (count, count / 2); /* 1.5 * count */
  if (estimate > list->table_size)
    hash_resize (list, estimate);
}

/* Return the position of the given node in the tree.  */
static size_t
node_position (gl_list_node_t node)
{
  size_t position = 0;

  if (node->left != NULL)
    position += node->left->branch_size;
  for (;;)
    {
      gl_list_node_t parent = node->parent;

      if (parent == NULL)
	return position;
      /* position is now relative to the subtree of node.  */
      if (parent->right == node)
	{
	  position += 1;
	  if (parent->left != NULL)
	    position += parent->left->branch_size;
	}
      /* position is now relative to the subtree of parent.  */
      node = parent;
    }
}

/* Compares two nodes by their position in the tree.  */
static int
compare_by_position (const void *x1, const void *x2)
{
  gl_list_node_t node1 = (gl_list_node_t) x1;
  gl_list_node_t node2 = (gl_list_node_t) x2;
  size_t position1 = node_position (node1);
  size_t position2 = node_position (node2);

  return (position1 > position2 ? 1 :
	  position1 < position2 ? -1 : 0);
}

/* Return the first element of a non-empty ordered set of nodes.  */
static inline gl_list_node_t
gl_oset_first (gl_oset_t set)
{
  gl_oset_iterator_t iter = gl_oset_iterator (set);
  const void *first;

  if (!gl_oset_iterator_next (&iter, &first))
    abort ();
  gl_oset_iterator_free (&iter);
  return (gl_list_node_t) first;
}

/* Add a node to the hash table structure.
   If duplicates are allowed, this function performs in average time
   O((log n)^2): gl_oset_add may need to add an element to an ordered set of
   size O(n), needing O(log n) comparison function calls.  The comparison
   function is compare_by_position, which is O(log n) worst-case.
   If duplicates are forbidden, this function is O(1).  */
static void
add_to_bucket (gl_list_t list, gl_list_node_t new_node)
{
  size_t index = new_node->h.hashcode % list->table_size;

  /* If no duplicates are allowed, multiple nodes are not needed.  */
  if (list->base.allow_duplicates)
    {
      size_t hashcode = new_node->h.hashcode;
      const void *value = new_node->value;
      gl_listelement_equals_fn equals = list->base.equals_fn;
      gl_hash_entry_t *entryp;

      for (entryp = &list->table[index]; *entryp != NULL; entryp = &(*entryp)->hash_next)
	{
	  gl_hash_entry_t entry = *entryp;

	  if (entry->hashcode == hashcode)
	    {
	      if (((struct gl_multiple_nodes *) entry)->magic == MULTIPLE_NODES_MAGIC)
		{
		  /* An entry representing multiple nodes.  */
		  gl_oset_t nodes = ((struct gl_multiple_nodes *) entry)->nodes;
		  /* Only the first node is interesting.  */
		  gl_list_node_t node = gl_oset_first (nodes);
		  if (equals != NULL ? equals (value, node->value) : value == node->value)
		    {
		      /* Found already multiple nodes with the same value.
			 Add the new_node to it.  */
		      gl_oset_add (nodes, new_node);
		      return;
		    }
		}
	      else
		{
		  /* An entry representing a single node.  */
		  gl_list_node_t node = (struct gl_list_node_impl *) entry;
		  if (equals != NULL ? equals (value, node->value) : value == node->value)
		    {
		      /* Found already a node with the same value.  Turn it
			 into an ordered set, and add new_node to it.  */
		      gl_oset_t nodes;
		      struct gl_multiple_nodes *multi_entry;

		      nodes =
			gl_oset_create_empty (OSET_TREE_FLAVOR,
					      compare_by_position);

		      gl_oset_add (nodes, node);
		      gl_oset_add (nodes, new_node);

		      multi_entry =
			(struct gl_multiple_nodes *) xmalloc (sizeof (struct gl_multiple_nodes));
		      multi_entry->h.hash_next = entry->hash_next;
		      multi_entry->h.hashcode = entry->hashcode;
		      multi_entry->magic = MULTIPLE_NODES_MAGIC;
		      multi_entry->nodes = nodes;
		      *entryp = &multi_entry->h;
		      return;
		    }
		}
	    }
	}
    }
  /* If no duplicates are allowed, multiple nodes are not needed.  */
  new_node->h.hash_next = list->table[index];
  list->table[index] = &new_node->h;
}

/* Remove a node from the hash table structure.
   If duplicates are allowed, this function performs in average time
   O((log n)^2): gl_oset_remove may need to remove an element from an ordered
   set of size O(n), needing O(log n) comparison function calls.  The
   comparison function is compare_by_position, which is O(log n) worst-case.
   If duplicates are forbidden, this function is O(1) on average.  */
static void
remove_from_bucket (gl_list_t list, gl_list_node_t old_node)
{
  size_t index = old_node->h.hashcode % list->table_size;

  if (list->base.allow_duplicates)
    {
      size_t hashcode = old_node->h.hashcode;
      const void *value = old_node->value;
      gl_listelement_equals_fn equals = list->base.equals_fn;
      gl_hash_entry_t *entryp;

      for (entryp = &list->table[index]; ; entryp = &(*entryp)->hash_next)
	{
	  gl_hash_entry_t entry = *entryp;

	  if (entry == &old_node->h)
	    {
	      /* Found old_node as a single node in the bucket.  Remove it.  */
	      *entryp = old_node->h.hash_next;
	      break;
	    }
	  if (entry == NULL)
	    /* node is not in the right bucket.  Did the hash codes
	       change inadvertently?  */
	    abort ();
	  if (((struct gl_multiple_nodes *) entry)->magic == MULTIPLE_NODES_MAGIC
	      && entry->hashcode == hashcode)
	    {
	      /* An entry representing multiple nodes.  */
	      gl_oset_t nodes = ((struct gl_multiple_nodes *) entry)->nodes;
	      /* Only the first node is interesting.  */
	      gl_list_node_t node = gl_oset_first (nodes);
	      if (equals != NULL ? equals (value, node->value) : value == node->value)
		{
		  /* Found multiple nodes with the same value.
		     old_node must be one of them.  Remove it.  */
		  if (!gl_oset_remove (nodes, old_node))
		    abort ();
		  if (gl_oset_size (nodes) == 1)
		    {
		      /* Replace a one-element set with a single node.  */
		      node = gl_oset_first (nodes);
		      node->h.hash_next = entry->hash_next;
		      *entryp = &node->h;
		      gl_oset_free (nodes);
		      free (entry);
		    }
		  break;
		}
	    }
	}
    }
  else
    {
      /* If no duplicates are allowed, multiple nodes are not needed.  */
      gl_hash_entry_t *entryp;

      for (entryp = &list->table[index]; ; entryp = &(*entryp)->hash_next)
	{
	  if (*entryp == &old_node->h)
	    {
	      *entryp = old_node->h.hash_next;
	      break;
	    }
	  if (*entryp == NULL)
	    /* node is not in the right bucket.  Did the hash codes
	       change inadvertently?  */
	    abort ();
	}
    }
}

/* Build up the hash table during initialization: Store all the nodes of
   list->root in the hash table.  */
static inline void
add_nodes_to_buckets (gl_list_t list)
{
  /* Iterate across all nodes.  */
  gl_list_node_t node = list->root;
  iterstack_t stack;
  iterstack_item_t *stack_ptr = &stack[0];

  for (;;)
    {
      /* Descend on left branch.  */
      for (;;)
	{
	  if (node == NULL)
	    break;
	  stack_ptr->node = node;
	  stack_ptr->rightp = false;
	  node = node->left;
	  stack_ptr++;
	}
      /* Climb up again.  */
      for (;;)
	{
	  if (stack_ptr == &stack[0])
	    return;
	  stack_ptr--;
	  if (!stack_ptr->rightp)
	    break;
	}
      node = stack_ptr->node;
      /* Add the current node to the hash table.  */
      node->h.hashcode =
	(list->base.hashcode_fn != NULL
	 ? list->base.hashcode_fn (node->value)
	 : (size_t)(uintptr_t) node->value);
      add_to_bucket (list, node);
      /* Descend on right branch.  */
      stack_ptr->rightp = true;
      node = node->right;
      stack_ptr++;
    }
}