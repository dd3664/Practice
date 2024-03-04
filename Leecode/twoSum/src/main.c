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

int *twoSum(int* nums, int numsSize, int target, int* returnSize) 
{
    int i = 0;
	HASHITEM *item = NULL;
	int *ret = NULL;
	for (i = 0; i < numsSize; i++)
	{
		item = find_item(&g_hashtable, target - nums[i]);
		if (NULL == item)
		{
			insert_item(&g_hashtable, nums[i], i);
		}
		else
		{
			ret = (int *)malloc(2 * sizeof(int));
			ret[0] = i;
			ret[1] = item->value;
			break;
		}
	}

	*returnSize = NULL == ret ? 0 : 2;

	destroy_hashtbl(&g_hashtable);

	return ret;
}

int main(int argc, char *argv[])
{
	int nums[3] = {3, 2, 4};
	int *ret;
	int ret_size;
	ret = twoSum(nums, 3, 6, &ret_size);
	printf("ret_size=%d\n", ret_size);
	if (NULL != ret)
	{
		printf("ret[0] = %d, ret[1] = %d\n", ret[0], ret[1]);
		free(ret);
	}
	return 0;
}
