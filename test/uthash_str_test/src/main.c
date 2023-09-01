#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uthash.h>

typedef struct{
   char name[10];
   int id;
   UT_hash_handle hh;
} HASHTEST;

int main(int argc, char **argv)
{
   const char *names[] = {"joe", "bob", "betty", NULL};
   HASHTEST *s, *tmp, *users = NULL;
   for (int i = 0; names[i]; i++)
   {
      s = (HASHTEST *)malloc(sizeof(*s));
      strcpy(s->name, names[i]);
      s->id = i;
      HASH_ADD_STR(users, name, s);
   }

   HASH_FIND_STR(users, "betty", s);
   if (s != NULL)
   {
      printf("betty's id is: %d\n", s->id);
   }

   HASH_ITER(hh, users, s, tmp)
   {
      HASH_DEL(users, s);
      free(s);      
   }
   return 0;
}