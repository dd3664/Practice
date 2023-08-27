#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <uthash.h>

struct my_struct {
   int id;
   char name[10];
   UT_hash_handle hh;
};

struct my_struct *g_users = NULL;

void addUser(int user_id, char *name)
{
   struct my_struct *s;
   HASH_FIND_INT(g_users, &user_id, s); /* 插入前先查看key值是否已经在hash表g_users里面了 */
   if (s == NULL)
   {
      s = (struct my_struct *)malloc(sizeof *s);
      s->id = user_id;
      HASH_ADD_INT(g_users, id, s); /* 第二个参数为hash结构里面，hash key值的变量名 */
   }
   strcpy(s->name, name);
}

void deleteUser(int user_id)
{
   struct my_struct *s =NULL;
   HASH_FIND_INT(g_users, &user_id, s);
   if (s != NULL)
   {
      HASH_DEL(g_users, s);
      free(s);
   }
}

struct my_struct *findUser(int user_id)
{
   struct my_struct *s;
   HASH_FIND_INT(g_users, &user_id, s);
   return s;
}

void deleteAll()
{
   struct my_struct *current_user;
   struct my_struct *tmp;
   
   HASH_ITER(hh, g_users, current_user, tmp)
   {
      HASH_DEL(g_users, current_user);
      free(current_user);
   }
}

void printUsers()
{
   struct my_struct *s;
   for (s = g_users; s != NULL; s = (struct my_struct *)(s->hh.next))
   {
      printf("userid: %d; name: %s\n", s->id, s->name);
   }
}

int by_name(const struct my_struct *a, const struct my_struct *b)
{
   return strcmp(a->name, b->name);
}

int by_id(const struct my_struct *a, const struct my_struct *b)
{
   return (a->id - b->id);
}

const char *getl(const char *prompt)
{
   static char buf[21];
   char *p;
   printf("%s?", prompt);
   fflush(stdout);
   p = fgets(buf, sizeof(buf), stdin);
   if (p == NULL || (p = strchr(buf, '\n')) == NULL)
   {
      puts("Invalid input!");
      exit(EXIT_FAILURE);
   }
   *p = '\0';
   return buf;
}

int main()
{
   int id = 1;
   int running = 1;
   struct my_struct *s;
   int temp;

   while(running)
   {
      printf("1. add user\n");
      printf("1.add or rename user by id\n");
      printf(" 3. find user\n");
      printf(" 4. delete user\n");
      printf(" 5. delete all users\n");
      printf(" 6. sort items by name\n");
      printf(" 7. sort items by id\n");
      printf(" 8. print users\n");
      printf(" 9. count users\n");
      printf("10. quit\n");

      switch(atoi(getl("Command")))
      {
         case 1:
            addUser(id++, getl("Name (20 char max)"));
            break;
         case 2:
            temp = atoi(getl("ID"));
            addUser(temp, getl("Name (20 char max)"));
         case 3:
            s = findUser(atoi(getl("ID to find")));
            printf("user: %s\n", s ? s->name : "unknown");
            break;
         case 4:
            deleteUser(atoi(getl("ID to delete")));
            break;
         case 5:
            deleteAll();
            break;
         case 6:
            HASH_SORT(g_users, by_name);
            break;
         case 7:
            HASH_SORT(g_users, by_id);
            break;
         case 8:
            printUsers();
            break;
         case 9:
            temp = HASH_COUNT(g_users);
            printf("there ar %d users\n", temp);
            break;
         case 10:
            running = 0;
            break;
      }
   }
   deleteAll();
   return 0;
}