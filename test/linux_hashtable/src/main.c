/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtable.h"
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
typedef struct _hash_test {
	int key;
	int value;
	struct hlist_node hnode; /* 哈希表节点 */
} HASHTEST;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
DEFINE_HASHTABLE(g_hashtble, 8); /* 定义哈希表，8表示哈希表的大小, 1<<8 */
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{	
	HASHTEST *s;
	HASHTEST *found;
	struct hlist_node *tmp;
	int key_to_find = 42;
	int i = 0;
	int bkt;

	for (i = 0; i < 100; i++)
	{
		s = (HASHTEST *)malloc(sizeof(HASHTEST));
		s->key = i;
		s->value = i + 100;
		hash_add(g_hashtble, &s->hnode, s->key);
	}
	
	hash_for_each_possible(g_hashtble, found, hnode, key_to_find)
	{
		if (found->key == key_to_find)
		{
			printf("Key found: key=%d, value=%d\n", found->key, found->value);
			printf("======================================================\n");
			break;
		}
	}

	hash_for_each_safe(g_hashtble, bkt, tmp, s, hnode)
	{
		if (NULL != s)
		{
			printf("key=%d, value=%d\n", s->key, s->value);
			hash_del(&s->hnode);
			free(s);
		}
	}

	return 0;
}
