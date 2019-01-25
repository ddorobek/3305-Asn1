/******************************************************************************
* 
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* shell.h
*
******************************************************************************/

#ifndef SHELL_H
#define SHELL_H
	
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdbool.h>

#include "helper.h"

#define READ "r"

void shell(char*);
short execute_commands(char*);
void shift_positions(int *positions, int count);
void command_has_pipe(char **lineArray, int *pipePositions, int pipeCount, int tokenCount, int *inPositions, int inCount, int *outPositions, int outCount);
void check_output(char **lineArray, int *outPositions, int outCount, int tokenCount);
void check_input(char **lineArray, int *inPositions, int inCount, int tokenCount);

#endif