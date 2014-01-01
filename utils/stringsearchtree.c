#include "stringsearchtree.h"

inline char init_node(node *parent, int depth)
{
	// Should always be != NULL
	if(parent == NULL)
	{
		return 1;
	}

	// Allocate node a or fail!
	parent->a = calloc(1, sizeof(node));
	if(parent->a == NULL)
	{
		return 1;
	}

	// Allocate node b or fail!
	parent->b = calloc(1, sizeof(node));
	if(parent->b == NULL)
	{
		nfree(parent->a);
		return 1;
	}

	// Initialize concatenated lists
	if(depth == 4)
	{
		return 0;
	}


	// Try to initialize node a or fail!
	if(init_node(parent->a, depth+1) != 0)
	{
		nfree(parent->a);
		return 1;
	}

	// Try to initialize node b or fail!
	if(init_node(parent->b, depth+1) != 0)
	{
		nfree(parent->a);
		nfree(parent->b);

		return 1;
	}

	return 0;
}

node *init_searchtree(void)
{
	node *root;

	// Allocate root or fail!
	root = calloc(1, sizeof(node));
	if(root == NULL)
		return NULL;

	if(init_node(root, 0) != 0)
	{
		nfree(root);
		return NULL;
	}

	return root;
}

inline node *get_list_from_key(node *tree, unsigned char *key)
{
	// The 5 integers are equal to first 20 bytes of the (hashed) string.
	unsigned int v[5];
	memcpy(v, key, 20);

	// Binary search inside of the tree
	node *node_ptr = tree;

	int depth;
	for(depth=0; depth<5; depth++)
	{
		// Direction in the tree: right or left?
		if(v[depth] < UINT_MAX/2)
		{
			node_ptr = node_ptr->a;
		}
		else
		{
			node_ptr = node_ptr->b;
		}
	}

	return node_ptr;
}

char add_string(node *tree, unsigned char *key, unsigned char *value)
{
	// Get the list where the value should be stored
	node *node_ptr = get_list_from_key(tree, key);

	if(node_ptr == NULL)
	{
		return 1;
	}


	// Allocate list the first time
	if(node_ptr->a == NULL)
	{
		node_ptr->a = calloc(1, sizeof(list));
		node_ptr->b = node_ptr->a;
	}
	else
	{
		// Allocate next element or fail!
		((list*)node_ptr->b)->next = calloc(1, sizeof(list));
		if(((list*)node_ptr->b)->next == NULL)
		{
			return 1;
		}
		// Set b to the new end
		node_ptr->b = (node *)((list*)node_ptr->b)->next;
	}

	// Point to the end of the list!
	list *list_ptr = (list *)node_ptr->b;

	// Allocate key or fail!
	list_ptr->key = (unsigned char *)calloc(1, 20);
	if(list_ptr->key == NULL)
		return 1;

	// Copy key into list
	if(memcpy((char *)list_ptr->key, (char *)key, 20) == NULL)
	{
		nfree(list_ptr->key);

		return 1;
	}

	// Allocate string or fail!
	list_ptr->string = (unsigned char *)calloc(1, strlen((char *)value)+1);
	if(list_ptr->string == NULL)
	{
		nfree(list_ptr->key);

		return 1;
	}

	// Copy string into list
	if(strncpy((char *)list_ptr->string, (char *)value, strlen((char *)value)) == NULL)
	{
		nfree(list_ptr->key);
		nfree(list_ptr->string);

		return 1;
	}

	return 0;
}

int check_string(node *tree, unsigned char *key, char *value)
{
	// Get the list where the value should be stored
	node *node_ptr = get_list_from_key(tree, key);
	if(node_ptr == NULL)
	{
		return 1;
	}

	// Point to the beginning of the list
	list *list_ptr = (list *)node_ptr->a;

	// Linear search on list
	while(list_ptr != NULL)
	{
		if(memcmp((char *)list_ptr->key, (char *)key, 20) == 0)
		{
			if(strcmp((char *)list_ptr->string, value)==0)
			{
				return 0;
			}
		}

		list_ptr = list_ptr->next;
	}

	// We didn't find the key
	return 1;
}


inline void free_list(list *list_ptr)
{
	unsigned int c=0;
	while(list_ptr != NULL)
	{
		++c;

		list *list_old = list_ptr;
		list_ptr = list_ptr->next;

		nfree(list_old->key);
		nfree(list_old->string);
		nfree(list_old);
	}

	log_write("stringsearchtree: Freed %u elements", LOG_DBG, 1, c);
}

inline void free_node(node *node, int depth)
{
	if(node == NULL)
		return;

	// Free concatenated lists
	if(depth == 5)
	{
		// Start freeing from beginning of the list
		free_list((list *)node->a);
		nfree(node);
		return;
	}

	if(node->a != NULL)
		free_node(node->a, depth+1);
	if(node->b != NULL)
		free_node(node->b, depth+1);

	nfree(node);
}

void free_searchtree(node *tree)
{
	free_node(tree, 0);
}

