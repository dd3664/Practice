/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <uthash.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
typedef struct {
	int key;
	int value;
	UT_hash_handle hh;
} HASHITEM;
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
HASHITEM *g_hashtable = NULL;
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
void insert_item(HASHITEM **tbl, int key, int value)
{
	HASHITEM *item;
	HASH_FIND_INT(*tbl, &key, item);
	if (NULL == item)
	{
		item = (HASHITEM *)malloc(sizeof *item);
		item->key = key;
		HASH_ADD_INT(*tbl, key, item);
	}
	item->value = value;
}

HASHITEM *find_item(HASHITEM **tbl, int key)
{
	HASHITEM *item = NULL;;
	HASH_FIND_INT(*tbl, &key, item);
	return item;
}

void delete_item(HASHITEM **tbl, int key)
{
	HASHITEM *item;
	HASH_FIND_INT(*tbl, &key, item);
	if (NULL != item)
	{
		HASH_DEL(*tbl, item);
		free(item);
	}
}

void destroy_hashtbl(HASHITEM **tbl)
{
	HASHITEM *cur_item;
	HASHITEM *tmp;

	if (NULL == *tbl)
	{
		return;
	}
	
	HASH_ITER(hh, *tbl, cur_item, tmp)
	{
		HASH_DEL(*tbl, cur_item);
		free(cur_item);
	}

	*tbl = NULL;
}

int lengthOfLongestSubstring(char* s)
{
    int len = 0;
	int right = 0;
	int left = 0;
	int ret = 0;
	HASHITEM *item;

	len = strlen(s);
	for (right = 0; right < len; right++)
	{
		item = find_item(&g_hashtable, (int)s[right]);
		if (NULL == item)
		{
			insert_item(&g_hashtable, (int)s[right], right);
		}
		else
		{
			if (item->value < left)
			{
				item->value = right;
				continue;
			}
			else
			{
				ret = (right - left) > ret ? (right - left) : ret;
				left = item->value + 1;
				item->value = right;
			}
			/* 上述代码流程更易理解，但可简化为以下代码 */
			/* ret = (right - left) > ret ? (right - left) : ret;
			left = (item->value + 1) > left ? (item->value + 1) : left;
			item->value = right; */
		}
	}
	ret = (right - left) > ret ? (right - left) : ret;
	return ret;
}

int main(int argc, char *argv[])
{
	char *s = argv[1];
	int ret = lengthOfLongestSubstring(s);
	printf("ret=%d\n", ret);
	return 0;
}
