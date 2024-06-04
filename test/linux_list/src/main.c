/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"
#include "list_sort.h"
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
typedef struct _list_node
{
	int data;
	struct list_head list;
} LISTNODE;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
LIST_HEAD(g_test_list);
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/
static int descending_sort(void *priv, struct list_head *a, struct list_head *b)
{
	LISTNODE *list_node_a = list_entry(a, LISTNODE, list);
	LISTNODE *list_node_b = list_entry(b, LISTNODE, list);
	return (list_node_b->data - list_node_a->data);
}

static int ascending_sort(void *priv, struct list_head *a, struct list_head *b)
{
	LISTNODE *list_node_a = list_entry(a, LISTNODE, list);
	LISTNODE *list_node_b = list_entry(b, LISTNODE, list);
	return (list_node_a->data - list_node_b->data);
}

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{	
	LISTNODE *node;
	LISTNODE *tmp;
	int test[] = {9, 1, 8, 4, 7};
	int i;

	for (i = 0; i < 5; i++)
	{
		node = malloc(sizeof(LISTNODE));
		if (NULL == node)
		{
			break;
		}
		memset(node, 0, sizeof(LISTNODE));
		node->data = test[i];
		INIT_LIST_HEAD(&node->list);
		list_add_tail(&node->list, &g_test_list);
	}

	list_for_each_entry(node, &g_test_list, list)
	{
		printf("before sort, Data=%d\n", node->data);
	}

	list_sort(NULL, &g_test_list, descending_sort);

	list_for_each_entry(node, &g_test_list, list)
	{
		printf("descending sort, Data=%d\n", node->data);
	}

	list_sort(NULL, &g_test_list, ascending_sort);
	
	list_for_each_entry_safe(node, tmp, &g_test_list, list)
	{
		printf("ascending sort, Data=%d \n", node->data);
		list_del(&node->list);
		free(node);
	}

	return 0;
}
