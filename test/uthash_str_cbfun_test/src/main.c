/*--------------------------------------------------------------------------------INCLUDE--------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "calback.h"

/*--------------------------------------------------------------------------------DEFINES--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------VARIABLES--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------STATIC FUNCTIONS--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------PUBLIC FUNCTIONS--------------------------------------------------------------------------------*/

int main(int argc, char **argv)
{
   int ret;
   int a = 3;
   int b = 2;
   callbackInit();
   ret = callback("add", a, b);
   printf("add ret: %d\n", ret);
   ret = callback("substract", a, b);
   printf("substract ret: %d\n", ret);
   return ret;
}