#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include "../utils/lwutils.h"

typedef struct list_elem
{
	void *data;
	struct list_elem *next;
	struct list_elem *prev;
} list_elem;

extern list_elem *list_add_elem(list_elem *list, void *data) __attribute__ ((warn_unused_result));
extern list_elem *list_map(list_elem *list, int(func)(void*, va_list), ...) __attribute__ ((warn_unused_result));
extern void list_clear(list_elem *list);

typedef struct tree_node
{
	list_elem *list;
	struct tree_node *left;
	struct tree_node *right;
} tree_node;

extern tree_node *tree_add_elem(tree_node *root_node, void *data, int(cmp_func)(void*,void*));
extern tree_node *tree_rem_elem(tree_node *root_node, void *data, int(cmp_func)(void*,void*)) __attribute__ ((warn_unused_result));
extern void tree_clear(tree_node *active_node);

#endif
