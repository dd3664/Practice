/*--------------------------------------------------------------------------------INCLUDE--------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "uthash.h"
#include "registerCore.h"
/*--------------------------------------------------------------------------------DEFINES--------------------------------------------------------------------------------*/
typedef struct __CALLBACK
{
    char key[10]; /* key of the callback */
    func cb; /* function ptr of the callback*/
    UT_hash_handle hh;
} CALLBACK;
/*--------------------------------------------------------------------------------VARIABLES--------------------------------------------------------------------------------*/
CALLBACK *cbFuncs = NULL;

/*--------------------------------------------------------------------------------STATIC FUNCTIONS--------------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------------PUBLIC FUNCTIONS--------------------------------------------------------------------------------*/
void regCallback(char *key, func cb)
{
   CALLBACK *s;
   HASH_FIND_STR(cbFuncs, key, s);
   if (s != NULL)
   {
      printf("Reg failed due to the repeated key!\n");
      return;
   }
   s = (CALLBACK *)malloc(sizeof(*s));
   strcpy(s->key, key);
   s->cb = cb;
   HASH_ADD_STR(cbFuncs, key, s);
}

int actCallback(char *key, int param1, int param2)
{
   CALLBACK *s;
   int ret = 0;
   HASH_FIND_STR(cbFuncs, key, s);
   if (s == NULL)
   {
      printf("Callback failed due to the key not exist!\n");
      return ret;
   }
   ret = s->cb(param1, param2);
   return ret;

}
