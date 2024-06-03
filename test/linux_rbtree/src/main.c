/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rbtree.h"
#include "type.h"
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
typedef struct _rbtree_entry {
	int key;
	struct rb_node node;
} RBTREE_ENTRY;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
struct rb_root entry_root = RB_ROOT;
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/
static void entry_insert(struct rb_root *root, RBTREE_ENTRY *entry)
{
	struct rb_node **p, *parent = NULL;
	RBTREE_ENTRY *cur = NULL;

	p = &root->rb_node;
	while (*p)
	{
		parent = *p;
		cur = rb_entry(*p, RBTREE_ENTRY, node);		
		if (entry->key > cur->key)
		{
			p = &(*p)->rb_right;
		}
		else
		{
			p = &(*p)->rb_left;
		}
	}
	rb_link_node(&entry->node, parent, p);
	rb_insert_color(&entry->node, root);
}

static RBTREE_ENTRY *entry_search(struct rb_root *root, int key)
{
	struct rb_node *p = root->rb_node;
	RBTREE_ENTRY *cur = NULL;

	while(p)
	{
		cur = rb_entry(p, RBTREE_ENTRY, node);
		if (key > cur->key)
		{
			p = p->rb_right;
		}
		else if (key < cur->key)
		{
			p = p->rb_left;
		}
		else
		{
			return cur;
		}
	}
	return NULL;
}

static void entry_remove(struct rb_root *root, int key)
{
	RBTREE_ENTRY *entry = entry_search(root, key);
	if (NULL == entry)
	{
		return;
	}
	rb_erase(&entry->node, root);
	free(entry);
	entry = NULL;
	return;
}
/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{
	int i = 0;
	int keys[] = {5, 3, 7, 1, 4, 6, 8};
	int remove_key = 5;
	RBTREE_ENTRY *entry = NULL;
	struct rb_node *cur;

	for (i = 0; i < ARRAY_SIZE(keys); i++)
	{
		entry = malloc(sizeof(RBTREE_ENTRY));
		memset(entry, 0, sizeof(RBTREE_ENTRY));
		entry->key = keys[i];
		entry_insert(&entry_root, entry);
	}

	for (i = 0; i < ARRAY_SIZE(keys); i++)
	{
		entry = entry_search(&entry_root, keys[i]);
		if (NULL == entry)
		{
			continue;
		}
		printf("key[%d]=%d\n", i, entry->key);
	}

	printf("remove entry which key=%d\n", remove_key);
	entry_remove(&entry_root, remove_key);

	for (cur = rb_first(&entry_root); cur; cur = rb_next(cur))
	{
		entry = rb_entry(cur, RBTREE_ENTRY, node);
		printf("remove from root and free, entry->key=%d\n", entry->key);
		rb_erase(cur, &entry_root);
		free(entry);
		entry = NULL;
	}

	return 0;
}
