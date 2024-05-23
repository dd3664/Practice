/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
/****************************************************************************************************/
/*                                           DEFINES                                                */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                           VARIABLES                                              */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       STATIC FUNCTIONS                                           */
/****************************************************************************************************/

/****************************************************************************************************/
/*                                       PUBLIC FUNCTIONS                                           */
/****************************************************************************************************/
int main(int argc, char *argv[])
{	
	int id = 0;
	int a = 0;
	int b = 0;
	long double c = 0.000001;
	unsigned int print_count = 0;

	if (NULL == argv[1])
	{
		printf("Please input an param as id\n");
		return 0;
	}
	id = atoi(argv[1]);
	printf("============Program %d Start============\n", id);
	while(1)
	{
		c = c * c + 1;
		a++;
		if (a == 1000)
		{
			//printf("a = %d\n", a);
			b++;
			a = 0;
		}
		if (b == 10000)
		{
			print_count++;
			printf("id = %d, print_count = %d (x10^7)\n",id,  print_count);
			b = 0;
		}
	}
	return 0;
}