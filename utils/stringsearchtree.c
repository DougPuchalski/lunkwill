#include "stringsearchtree.h"

inline char init_node(node *parent, int depth)
{
	// Should always be != NULL
	if(parent == NULL)
		return 1;

	// Initialize concatenated lists
	if(depth == 4)
	{
		// Allocate list a or fail!
		parent->a = (list *)calloc(1, sizeof(list));
		if(parent->a == NULL)
			return 1;

		// b points to the end of list a
		parent->b = parent->a;

		return 0;
	}

	// Allocate node a or fail!
	parent->a = calloc(1, sizeof(node));
	if(parent->a == NULL)
		return 1;

	// Try to initialize node a or fail!
	if(init_node(parent->a, depth+1) != 0)
	{
		free(parent->a);
		return 1;
	}

	// Allocate node b or fail!
	parent->b = calloc(1, sizeof(node));
	if(parent->b == NULL)
	{
		free(parent->a);
		return 1;
	}

	// Try to initialize node b or fail!
	if(init_node(parent->b, depth+1) != 0)
	{
		free(parent->a);
		free(parent->b);

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
		free(root);
		return NULL;
	}

	return root;
}

inline node *get_list_from_key(node *tree, unsigned char *key)
{
	// Our key needs to be at least 20 bytes long
	if(strlen((char *)key) < 20)
		return NULL;

	// The 5 integers are equal to first 20 bytes of the (hashed) string.
	unsigned int v[5];
	memcpy(v, key, 20);


	// Binary search inside of the tree
	node *node_ptr = tree;

	int depth;
	for(depth=0; depth<5; depth++)
	{
		// Direction in the tree: right or left?
		if(v[depth] < UINT_MAX)
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
		return 1;


	// Allocate list the first time
	if(node_ptr->a == NULL)
	{
		node_ptr->a = calloc(1, sizeof(node));
		node_ptr->b = node_ptr->a;
	}

	// Point to the end of the list!
	list *list_ptr = (list *)node_ptr->b;

	// Allocate key or fail!
	list_ptr->key = (unsigned char *)calloc(1, strlen((char *)key)+1);
	if(list_ptr->key == NULL)
		return 1;

	// Copy key into list
	if(strcpy((char *)list_ptr->key, (char *)key) == NULL)
	{
		free(list_ptr->key);

		return 1;
	}

	// Allocate string or fail!
	list_ptr->string = (unsigned char *)calloc(1, strlen((char *)value)+1);
	if(list_ptr->string == NULL)
	{
		free(list_ptr->key);

		return 1;
	}

	// Copy string into list
	if(strcpy((char *)list_ptr->string, (char *)value) == NULL)
	{
		free(list_ptr->key);
		free(list_ptr->string);

		return 1;
	}

	// Allocate next element or fail!
	list_ptr->next = calloc(1, sizeof(list));
	if(list_ptr->next == NULL)
	{
		free(list_ptr->key);
		free(list_ptr->string);

		return 1;
	}

	// Set b to the new end
	node_ptr->b = (node *)list_ptr->next;

	return 0;
}

unsigned char *search_string(node *tree, unsigned char *key)
{
	// Get the list where the value should be stored
	node *node_ptr = get_list_from_key(tree, key);
	if(node_ptr == NULL)
		return NULL;

	// Point to the beginning of the list
	list *list_ptr = (list *)node_ptr->a;
	// Linear search on list
	while(list_ptr != NULL)
	{
		if(strcmp((char *)list_ptr->key, (char *)key) == 0)
			return list_ptr->string;

		list_ptr = list_ptr->next;
	}

	// We didn't find the key
	return NULL;
}


inline void free_list(list *list_ptr)
{
	unsigned int c=0;
	while(list_ptr != NULL)
	{
		++c;

		list *list_old = list_ptr;
		list_ptr = list_ptr->next;

		free(list_old->key);
		free(list_old->string);
		free(list_old);
	}

	log_write("stringsearchtree: Freed %u elements", LOG_DBG, 1, c);
}

inline void free_node(node *node, int depth)
{
	// Free concatenated lists
	if(depth == 4)
	{
		// Start freeing from beginning of the list
		free_list((list *)node->a);
		free(node->a);
		free(node);
		
		return;
	}

	free_node(node->a, depth+1);
	free_node(node->b, depth+1);
	free(node);
}

void free_searchtree(node *tree)
{
	free_node(tree, 0);
}

