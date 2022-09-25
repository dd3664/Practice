#include <stdio.h>

void num_add()
{
    static int i = 0;
    int b = 0;
    i = i + 1;
    b = b + 1;
    printf("i= %d, b= %d \n", i, b);
}

int main()
{
    num_add();
    num_add();
    num_add();
    getchar();
    return 0;
}
