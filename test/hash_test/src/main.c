#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 20

struct DataItem {
   int key;
   int data;
};

struct DataItem *hashArray[SIZE];

int hashCode(int key)
{
   return key % SIZE;
}

struct DataItem *search(int key)
{
   //获取哈希值
   int hashIndex = hashCode(key);

   //在哈希表中进行线性查找
   while (hashArray[hashIndex] != NULL)
   {
      if (hashArray[hashIndex]->key == key)
      {
         return hashArray[hashIndex];
      }
      //在哈希冲突时，通过线性探测向前移动
      ++hashIndex;

      //循环查找整个哈希表
      hashIndex %=SIZE;
   }
   return NULL;
}

void insert(int key, int data)
{
   struct DataItem *item = (struct DataItem*)malloc(sizeof(struct DataItem));
   item->data = data;
   item->key = key;

   //获取哈希值
   int hashIndex = hashCode(key);

   //处理哈希冲突
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1)
   {
      //在哈希冲突时，通过线性探测向前移动
      ++hashIndex;

      //循环查找整个哈希表
      hashIndex %= SIZE;
   }
   hashArray[hashIndex] = item;
}

void delete(struct DataItem *item)
{
   int key = item->key;

   //获取哈希值
   int hashIndex = hashCode(key);

   //在哈希表中进行线性查找
   while (hashArray[hashIndex] != NULL)
   {
      if (hashArray[hashIndex]->key == key)
      {
         free(hashArray[hashIndex]);
         //标记该位置为空
         hashArray[hashIndex] = NULL;
      }

      //在哈希冲突时，通过线性探测向前移动
      ++hashIndex;

      //循环整个哈希表
      hashIndex %= SIZE;
   }
}

void display()
{
   int i = 0;

   for (i = 0; i < SIZE; i++)
   {
      if (hashArray[i] != NULL)
      {
         printf(" (%d, %d)", hashArray[i]->key, hashArray[i]->data);
      }
      else
      {
         printf(" (~, ~) ");
      }
   }
   printf("\n");
}

int main()
{
   struct DataItem *item;
   insert(1, 20);
   insert(2, 70);
   insert(42, 80);
   insert(4, 25);
   insert(12, 44);
   insert(14, 32);
   insert(17, 11);
   insert(13, 78);
   insert(37, 97);

   display();

   item = search(37);

   if (item != NULL)
   {
      printf("search(37), find the element: (%d, %d)\n", item->key, item->data);
   }
   else
   {
      printf("search(37), not find the element\n");
   }

   delete(item);
   display();
   item = search(37);

   if (item != NULL)
   {
      printf("after delete item, search(37), find the element: (%d, %d)\n", item->key, item->data);
   }
   else
   {
      printf("after delete item, search(37), not find the element\n");
   }
}