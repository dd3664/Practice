/****************************************************************************************************/
/*                                           INCLUDE                                                */
/****************************************************************************************************/
#include <stdio.h>
#include "logTool.h"
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
	int n = 100;
	while (n--)
	{
		TPLOG("This is a test This is a test This is a test This is a test This is a test This is a test This is a test ");
		TPLOG("This is a test This is a test This is a test This is a test This is a test This is a test This is a test ");
	}
	return 0;
}
