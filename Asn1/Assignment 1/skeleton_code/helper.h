/******************************************************************************
* 
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* helper.h
*
******************************************************************************/

#ifndef HELPER_H
#define HELPER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LEN 1024

#define TERMINATE -1
#define FAILURE 1
#define SUCCESS 0

#define FALSE 0
#define TRUE 1
#define DELETE 127
#define KEYLEFT 4
#define KEYRIGHT 5
#define KEYUP 3
#define KEYDOWN 2

void print_user();
char getch(FILE*);
void delete(int, char*);
char switch_keypad(char);

#endif