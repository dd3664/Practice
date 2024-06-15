/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "type.h"
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
typedef struct _avl_entry {
	int key;
	struct avl_node avl;
} AVL_ENTRY;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
struct avl_tree g_avltree;
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/
static int entry_cmp(const void *k1, const void *k2, void *ptr)
{
	const AVL_ENTRY *entry1 = k1;
	const AVL_ENTRY *entry2 = k2;
	return entry1->key - entry2->key;
}
/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{
	int i = 0;
	int keys[] = {5, 3, 7, 1, 4, 6, 8};
	AVL_ENTRY *entry = NULL;
	AVL_ENTRY *tmp = NULL;
	AVL_ENTRY entry_tosearch = {0};

	//初始化AVL树
	avl_init(&g_avltree, entry_cmp, false, NULL);

	//插入节点
	for (i = 0; i < ARRAY_SIZE(keys); i++)
	{
		entry = malloc(sizeof(AVL_ENTRY));
		memset(entry, 0, sizeof(AVL_ENTRY));
		entry->key = keys[i];
		entry->avl.key = entry;
		avl_insert(&g_avltree, &entry->avl);
	}

	//查找节点
	entry_tosearch.key = 5;
	tmp = avl_find_element(&g_avltree, &entry_tosearch, tmp, avl);
	if (NULL != tmp)
	{
		printf("find entry with key:%d\n", tmp->key);
	}

	//遍历并释放节点
	avl_for_each_element_safe(&g_avltree, tmp, avl, entry)
	{
		printf("%d\n", tmp->key);
		avl_delete(&g_avltree, &tmp->avl);
		free(tmp);
	}

	return 0;
}
