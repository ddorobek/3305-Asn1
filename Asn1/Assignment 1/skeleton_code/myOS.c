/******************************************************************************
* 
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* myOS.c is the entry point to tHE mini OS that you'll be developing throughout
* CS3305.
*
******************************************************************************/

#include "myOS.h"

/******************************************************************************
* Main function.
******************************************************************************/
int main(int argc, char** argv)
{
	if (argc > 1 && !strcmp(argv[1], SHELL))
	{
		shell(argv[2]);
	}

	else 
	{
		printf("\nUsage:\t./myOS COMMAND\n\nAvailable commands:\n\n\t1. shell\n\n");
	}

	return SUCCESS;
}