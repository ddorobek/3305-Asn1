/******************************************************************************
* 
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* helper.c contains all the helper methods for the shell
*
******************************************************************************/

#include "helper.h"

/******************************************************************************
* prints the device's username
******************************************************************************/
void print_user()
{
	printf("%s> ", getenv("USER"));
}

/******************************************************************************
* gets character
******************************************************************************/
char getch(FILE *fp)
{
	if (fp != NULL)
	{
		return fgetc(fp);
	}

	system("stty -echo");
	system("stty -icanon");
	char ch = getchar();
	system("stty sane");
  	return ch;
}

/******************************************************************************
* deletes the current character.
******************************************************************************/
void delete(int len, char* line)
{
	line[len] = '\0';
	printf("\b \b");
}

/******************************************************************************
* Determines which special character is processed.
******************************************************************************/
char switch_keypad(char ch)
{
	switch(ch)
	{
		case 'A': return KEYUP;
		case 'B': return KEYDOWN;
		case 'C': return KEYRIGHT;
		case 'D': return KEYLEFT;
		default:  return ch;
	}
}