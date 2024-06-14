/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "cJSON.h"

/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/
/*
{
    "name": "Jane",
    "age": 25,
    "is_student": true,
    "scores": [95, 92, 88],
    "address": {
        "city": "Los Angeles",
        "zipcode": "90001"
    }
}
*/
static void create_json()
{
	cJSON *root = NULL;
	cJSON *scores = NULL;
	cJSON *address = NULL;
	char *jstr = NULL;

	printf("==========create_json==========\n");

	//创建JSON对象
	root = cJSON_CreateObject();

	//添加数据
	cJSON_AddStringToObject(root, "name", "Jane");
	cJSON_AddNumberToObject(root, "age", 25);
	cJSON_AddBoolToObject(root, "is_student", cJSON_True);

	//创建并添加数组
	scores = cJSON_CreateArray();
	cJSON_AddItemToArray(scores, cJSON_CreateNumber(95));
	cJSON_AddItemToArray(scores, cJSON_CreateNumber(92));
	cJSON_AddItemToArray(scores, cJSON_CreateNumber(88));
	cJSON_AddItemToObject(root, "scores", scores);

	//创建并添加嵌套对象
	address = cJSON_CreateObject();
	cJSON_AddStringToObject(address, "city", "Los Angeles");
	cJSON_AddStringToObject(address, "zipcode", "90001");
	cJSON_AddItemToObject(root, "address", address);

	//打印JSON字符串
	jstr = cJSON_Print(root);
	if (NULL == jstr)
	{
		printf("cJSON_Print failed\n");
		goto leave;
	}
	printf("jstr=%s\n", jstr);

leave:
	if (root)
	{
		cJSON_Delete(root);
	}
	if (jstr)
	{
		free(jstr);
	}
	return;
}

static void parse_json()
{
	char *jstr = "{\"name\":\"John\",\"age\":30,\"is_student\":false,\"scores\":[90,85,88],\"address\":{\"city\":\"New York\",\"zipcode\":\"10001\"}}";
	cJSON *root = NULL;
	cJSON *item = NULL;
	cJSON *inner_item = NULL;
	int array_size = 0;
	int i = 0;

	printf("==========parse_json==========\n");

	//解析json字符串
	root = cJSON_Parse(jstr);
	if (NULL == root)
	{
		printf("cJSON_Parse failed\n");
		goto leave;
	}

	//提取数据
	item = cJSON_GetObjectItem(root, "name");
	if (NULL != item && cJSON_String == item->type && NULL != item->valuestring)
	{
		printf("name: %s\n", item->valuestring);
	}

	item = cJSON_GetObjectItem(root, "age");
	if (NULL != item && cJSON_Number == item->type)
	{
		printf("age: %d\n", item->valueint);
	}

	item = cJSON_GetObjectItem(root, "is_student");
	if (NULL != item && (cJSON_True == item->type || cJSON_False == item->type))
	{
		printf("is_student: %s\n", cJSON_True == item->type ? "true" : "false");
	}

	item = cJSON_GetObjectItem(root, "scores");
	if (NULL != item && cJSON_Array == item->type)
	{
		array_size = cJSON_GetArraySize(item);
		printf("scores: ");
		for (i = 0; i < array_size; i++)
		{
			inner_item = cJSON_GetArrayItem(item, i);
			if (NULL != inner_item && cJSON_Number == inner_item->type)
			{
				printf("%d ", inner_item->valueint);
			}
		}
		printf("\n");
	}

	item = cJSON_GetObjectItem(root, "address");
	if (NULL != item && cJSON_Object == item->type)
	{
		inner_item = cJSON_GetObjectItem(item, "city");
		if (NULL != inner_item && cJSON_String == inner_item->type && NULL != inner_item->valuestring)
		{
			printf("city: %s\n", inner_item->valuestring);
		}

		inner_item = cJSON_GetObjectItem(item, "zipcode");
		if (NULL != inner_item && cJSON_String == inner_item->type && NULL != inner_item->valuestring)
		{
			printf("zipcode: %s\n", inner_item->valuestring);
		}
	}
leave:
	if (root)
	{
		cJSON_Delete(root);
	}
	return;
}
/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{
	create_json();
	parse_json();
	return 0;
}
