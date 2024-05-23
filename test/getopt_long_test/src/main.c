/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
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
	int opt = 0;
	int option_index = 0;
	int flag = 0;
	const char opt_set[] = "ad:";
	struct option long_opt_set[] = {
		{"add", no_argument, 0, 'a'},
		{"delete", required_argument, 0, 'd'},
		{"file", required_argument, 0, 'f'},
		{"log", required_argument, &flag, 1},
		{0, 0, 0, 0} //结束标志
	};
	while ((opt = getopt_long(argc, argv, opt_set, long_opt_set, &option_index)) != -1)
	{
		switch (opt)
		{
			case 'a':
				printf("Option --add or -a\n");
				break;
			case 'd':
				printf("Option --delete or -d, with value: %s\n", optarg);
				break;
			case 'f':
				printf("Option --file with value: %s\n", optarg);
				break;
			case 0:
				printf("Option flag = %d, value=%s\n", flag, optarg);
				break;
			default:
				printf("Usage: %s [-a] [-d value] [--add] [--delete value] [--file value]\n", argv[0]);
				break;
		}
	}
	return 0;
}
