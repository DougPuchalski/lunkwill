#ifndef __STRINGSEARCHTREE_H__
#define __STRINGSEARCHTREE_H__

#include "../utils/lwutils.h"

struct node
{
	void *a;
	void *b;
};

struct list
{
	struct list *next;
	unsigned char *key;
	unsigned char *string;
};

typedef struct node node;
typedef struct list list;

/** \brief Add a string to the search tree */
extern char add_string(node *tree, char *key, char *value);

/** \brief Free a previously created tree */
extern void free_searchtree(node *tree);

/** \brief Initialize a search tree */
extern node *init_searchtree(void);

/** \brief Search for a value with the key */
extern char *search_string(node *tree, char *key);

/** \brief Init node and all subnodes */
char init_node(node *parent, int depth);

/** \brief Free node and all subnodes */
void free_node(node *node, int depth);


#endif
