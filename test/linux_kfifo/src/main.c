/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "kfifo.h"
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/
//定义fifo的元素
struct demo_element {
    int data1;
    int data2;
    char data3[8];
};

#define DM_FIFO_ELEMENT_MAX     2 /* 定义fifo最大保存的元素个数, 需为2的整数次幂 */
/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/
DECLARE_KFIFO(g_demo_fifo, struct demo_element, DM_FIFO_ELEMENT_MAX);
/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/
static void demo_enqueue(void)
{
   int ret = 0;
    struct demo_element element = {0};
 
    element.data1= 10;
    element.data2 = 20;
    strcpy(element.data3, "Test");

    ret = kfifo_put(&g_demo_fifo, element);
    if (!ret) 
    {
        printf("kfifo_put fail, fifo is full\n");
    }

    ret = kfifo_put(&g_demo_fifo, element);
    if (!ret) 
    {
        printf("kfifo_put fail, fifo is full\n");
    }

    ret = kfifo_put(&g_demo_fifo, element);
    if (!ret) 
    {
        printf("kfifo_put fail, fifo is full\n");
    }

    return;
}

static void demo_dequeue(void)
{
    int ret = 0;
    struct demo_element element = {0};

    ret = kfifo_get(&g_demo_fifo, &element);
    if (ret) 
    {
        printf("kfifo_get element.data1=%d, element.data2=%d, element.data3=%s\n", element.data1, element.data2, element.data3);
    } 
    else 
    {
        printf("kfifo_get fail, fifo is empty\n");
    }

    ret = kfifo_get(&g_demo_fifo, &element);
    if (ret) 
    {
        printf("kfifo_get element.data1=%d, element.data2=%d, element.data3=%s\n", element.data1, element.data2, element.data3);
    } 
    else 
    {
        printf("kfifo_get fail, fifo is empty\n");
    }

    ret = kfifo_get(&g_demo_fifo, &element);
    if (ret) 
    {
        printf("kfifo_get element.data1=%d, element.data2=%d, element.data3=%s\n", element.data1, element.data2, element.data3);
    } 
    else 
    {
        printf("kfifo_get fail, fifo is empty\n");
    }

    return;
}
/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{
    INIT_KFIFO(g_demo_fifo);

    demo_enqueue();
    demo_dequeue();

	return 0;
}
