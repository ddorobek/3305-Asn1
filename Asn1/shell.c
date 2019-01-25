/******************************************************************************
* 
* Name: 	Zaid Albirawi
* Email: 	zalbiraw@uwo.ca
*
* shell.c processes the users input, passes it to a tokenizer, and then passes
* the tokens obtained to the process_commands function which will process and
* execute the commands.
*
******************************************************************************/

#include "shell.h"

/******************************************************************************
* Processes the input and determine whether it is a user interface operation 
* or a set of commands that will need to be executed.
******************************************************************************/
void shell(char* filename)
{

	/**************************************************************************
	* short			special_char 	determines whether the character to be 
	*								processed is special or not.
	* int 			len 			keeps track of the current line length
	* char 			line 			holds current line
	**************************************************************************/
	short special_char = FALSE;
	int status, len = 0;
	char ch, *line = (char*)malloc(MAX_LEN);
	FILE *fp = NULL;

	if (filename != NULL)
	{
		fp = fopen(filename, READ);

		if (fp == NULL) printf("Unable to open %s\n", filename);

	}

	/**************************************************************************
	* Loops until the user exits the program.
	**************************************************************************/
	print_user();
	while(TRUE)
	{

		ch = getch(fp);
			
		if (special_char)
		{
			special_char = FALSE;
			ch = switch_keypad(ch);
		}

		/**********************************************************************
		* switchs arround possible cases depending on the read character
		**********************************************************************/
		switch(ch)
		{
			/******************************************************************
			* handles the ascii translation of arrow characters
			******************************************************************/
			case '\033':
				getch(NULL);
				special_char = TRUE;
				continue;
				break;

			/******************************************************************
			* ignore arrow characters and tab
			******************************************************************/
			case KEYLEFT:
			case KEYRIGHT:
			case KEYUP:
			case KEYDOWN:
			case '\t':
				break;

			/******************************************************************
			* handles backspacing
			******************************************************************/
			case DELETE:
			{
				if (len > 0) 
					delete(--len, line);
				break;
			}

			/******************************************************************
			* if the maximum line length is not exceeded the program will print
			* the character. if the character is not a new line then continue. 
			* Else, terminate line, pass it to the execute_commands methond,
			* allocate a new line, and reset the line length. 
			******************************************************************/
			default:
			{
				if (len < MAX_LEN)
				{

					if (ch != '\n')
					{
						printf("%c", ch);
						line[len++] = ch;
					}

					else if (ch == '\n' && len > 0)
					{
						printf("%c", ch);
						line[len] = '\0';
						status = execute_commands(line);
						
						free(line);
						if (status == TERMINATE) exit(SUCCESS);

						line = (char*)malloc(MAX_LEN);
						len = 0;

						print_user();
					}
				}
				break;
			}
		}
	}

	fclose(fp);
}

/******************************************************************************
* execute_commands will process and execute the commands in the variable line.
******************************************************************************/
short execute_commands(char* line)
{
	short status;
	char* input = line;
	int maxCapacity = 100;
	char* lineArray[maxCapacity];
	int tokenCount = 0, pipeCount = 0, optionCount = 0, inCount = 0, outCount = 0;
	
	//If user enters "exit" program will exit.
	if(strcmp(line,"exit") == 0) {
		exit(1);
	}
	
	//Tokenizes line by space delimeter. 
	for (char * token = strtok(input, " "); token; token = strtok(NULL, " ")) {
		lineArray[tokenCount] = token; //Token is added to lineArray
		
		if(strcmp(lineArray[tokenCount], "|") == 0) //If element is |, pipeCount is incremented.
			pipeCount++;
		
		/* if(strstr(lineArray[tokenCount], "-")) //If element is -, optionCount is incremented.
			optionCount++; */
		
		if(strstr(lineArray[tokenCount], "<")) //If element is <, inCount is incremented.
			inCount++;
		
		if(strstr(lineArray[tokenCount], ">")) //If element is >, outCount is incremented.
			outCount++;
		
		tokenCount++;
	}
	
	int pipePositions[pipeCount], optionPositions[optionCount], inPositions[inCount], outPositions[outCount];
	int pipes = 0, option = 0, in = 0, out = 0;
	
	//Iterates through lineArray and adds each pipe/input/output index to their respective arrays.
	for(int i=0; i<tokenCount; i++) {
		if(strcmp(lineArray[i], "|") == 0)
			pipePositions[pipes++] = i;
			
		/* if(strcmp(lineArray[i], "-") == 0)
			optionPositions[option++] = i; */
			
		if(strcmp(lineArray[i], "<") == 0)
			inPositions[in++] = i;

		if(strcmp(lineArray[i], ">") == 0)
			outPositions[out++] = i;
	}
		
	pid_t pid;
	pid = fork(); //Forks new process.

	//If an error occurs, it is printed and program exits.
	if(pid < 0) {
		perror("Problem forking.");
		exit(1);
	}
	
	//Parent process
	if(pid > 0) {
		int status;
		wait(NULL);
		//printf("child complete");
	}
	
	//Child process
	else if(pid == 0){
		
		//If there is an "<" in the command and no pipes, this block of code is executed.
		if(inCount > 0 && pipeCount == 0) {
			check_input(lineArray, inPositions, inCount, tokenCount); //Input is processed in check_input function.
			if(outCount > 0 && outPositions[0] != -1) {
				outPositions[0] -= 2; //If there is also a ">" in the command, the output position needs to be moved forward by 2 indexes.
			} 
		}
		
		//If there is an ">" in the command and no pipes, this block of code is executed.
		if(outCount > 0 && pipeCount == 0) {
			check_output(lineArray, outPositions, outCount, tokenCount); //Output is processed in check_output function.
		}
		
		//If there are no pipes, this block of code is executed.
		if(pipeCount == 0) {
			lineArray[tokenCount] = NULL; //Last index of lineArray is set to NULL for exec() function.
			status = execvp(lineArray[0], lineArray); //Command is executed.
			
			//If an error occurs, it is printed and program exits.
			if(status < 0) {
				perror("exec problem (child)\n");
				exit(1);				
			}
			
			exit(1);
		} 
		 		
		//Otherwise if there are pipes, this block of code is executed.
		else {	
			for(int i=0; i<tokenCount; i++) {
				if(strcmp(lineArray[i], "|") == 0) {
					lineArray[i] = NULL; //Sets each index in lineArray with a pipe to NULL.
				}
			} 
			
			command_has_pipe(lineArray, pipePositions, pipeCount, tokenCount, inPositions, inCount, outPositions, outCount); //Calls command_has_pipe to process command.
		}
	}
	
	return status;
}

/**********************************************************************************
* command_has_pipe will process and execute the piped commands in the variable line.
**********************************************************************************/
void command_has_pipe(char **lineArray, int *pipePositions, int pipeCount, int tokenCount, int *inPositions, int inCount, int *outPositions, int outCount) {
	int fds[2];
	pid_t pid;
	int status;
	
	//Creates the pipe. If a piping error occurs, it is printed and program exits.
	if(pipe(fds) < 0) {
		perror("fatal error: piping");
		exit(1);
	}
	
	pid = fork(); //Forks new process.
	
	//Parent process
	if(pid > 0) {
		close(fds[0]); //Close read end.
		
		//Redirect standard output to shared memory.
		if(dup2(fds[1], STDOUT_FILENO) < 0) {
			perror("can't dup");
			exit(1);
		}
		
		int tempOutPos[tokenCount], tempInPos[tokenCount]; //Declare temporary arrays for input and output positions.
		int tempOutCount = 0, tempInCount = 0; //Set temporary input and output counts to 0.
		char *temp[pipePositions[pipeCount]+1]; //Declare temporary array.
		int i;
		
		//Loop until first pipe.
		for(i=0; i<pipePositions[0]; i++) {
			temp[i] = lineArray[i];
			
			if(strcmp(lineArray[i], ">") == 0)
				tempOutPos[tempOutCount++] = i; //If element in lineArray is ">", set temporary output position to index.
			
			if(strcmp(lineArray[i], "<") == 0)
				tempInPos[tempInCount++] = i; //If element in lineArray is ">", set temporary input position to index.
		}
		temp[i] = NULL; //Set last element in temp[] to NULL.

		check_input(temp, tempInPos, tempInCount, pipePositions[pipeCount]+1); //temp[] input is processed in check_input function.
		
		if(tempInCount > 0 && tempOutCount > 0 && tempOutPos[0] != -1)
			tempOutPos[0] -= 2; //If there is also a ">" in temp[], the output position needs to be moved forward by 2 indexes.		

		check_output(temp, tempOutPos, tempOutCount, pipePositions[pipeCount]+1); //temp[] output is processed in check_output function.
		
		status = execvp(temp[0], temp); //Command in temp[] is executed.
		
		//If an error occurs, it is printed and program exits.
		if(status < 0) {
			perror("exec problem (parent)\n");
			exit(1);				
		}
		
		wait(NULL);			
		
	}
	
	//Child process
	else if(pid == 0){
		close(fds[1]); //Close standard output and reconnect to the writing end of the pipe.
		if(dup2(fds[0], STDIN_FILENO) < 0) {
			perror("can't dup");
			exit(1);
		}
				
		//If there are more pipes in the command, this block of code is executed.
		if(pipePositions[0] != -1) {
			char *temp[tokenCount - pipePositions[0]]; //Declare temporary array
			int tempOutPos[tokenCount], tempInPos[tokenCount]; //Declare temporary arrays for input and output positions.
			int tempOutCount = 0, tempInCount = 0; //Set temporary input and output counts to 0.
			int j=0;
			
			//Loop from first pipe in pipePositions until end of lineArray.
			for(int i=(pipePositions[0] + 1); i < tokenCount; i++) {
				temp[j] = lineArray[i];
				
				if(strcmp(lineArray[i], ">") == 0) { 
					tempOutPos[tempOutCount++] = j; //If element in lineArray is ">", set temporary output position to index.
				}
				
				if(strcmp(lineArray[i], "<") == 0) {
					tempInPos[tempInCount++] = j; //If element in lineArray is ">", set temporary output position to index.
				}
				j++;
			}
			temp[j] = NULL; //Set last element in temp[] to NULL.		
			
			shift_positions(pipePositions, pipeCount); //Shifts each pipe in pipePositions forward by 1.
			
			check_input(temp, tempInPos, tempInCount, j); //temp[] input is processed in check_input function.
			if(tempInCount > 0 && tempOutCount > 0 && tempOutPos[0] != -1) {
				tempOutPos[0] -= 2; //If there is also a ">" in temp[], the output position needs to be moved forward by 2 indexes.		
			} 			

			check_output(temp, tempOutPos, tempOutCount, j); //temp[] output is processed in check_output function.
			
			//If there are no more pipes, this code is executed.
			if(pipePositions[0] == -1) {
				status = execvp(temp[0], temp); //Command in temp[] is executed.
				
				//If an error occurs, it is printed and program exits.
				if(status < 0) {
					perror("exec problem (child)\n");
					exit(1);				
				}
				
				exit(1);
			}
			
			else {
				//If there are still pipes remaining, this function is called again recursively.
				command_has_pipe(lineArray, pipePositions, pipeCount, tokenCount, inPositions, inCount, outPositions, outCount);
			}
		}
		
		exit(1);
	}
}

/**********************************************************************************
* shift_positions will shift the elements of the positions array forward by 1.
**********************************************************************************/
void shift_positions(int *positions, int count) {
	if(count == 1) {
		positions[0] = -1; //If there is only 1 index in the array, positions[0] is set to -1.
	}
	else {
		int i;
		for(i=0; i<(count-1); i++) {
			positions[i] = positions[i+1]; //Each element is shifted forward.
		}
		positions[i] = -1; //Last element is set to -1.
	}
	count--;
}


/**********************************************************************************
* check_output will write the contents in standard output to the file passed to it. 
***********************************************************************************/
void check_output(char **lineArray, int *outPositions, int outCount, int tokenCount) {
	if(outCount > 0) {
		FILE *outputFile = fopen(lineArray[outPositions[0]+1], "w"); //Output file is created and opened.
		int outputFD = fileno(outputFile);
		dup2(outputFD, STDOUT_FILENO); //Standard output is duplicated to the file.
		close(outputFD); //File is closed.

		//Shifts each element in lineArray forward by 2 to delete ">" and "filename" from lineArray.
		for(int i=outPositions[0]; i<=tokenCount; i++) {
			if(i == tokenCount) {
				//Last 2 elements are set as null.
				lineArray[i-1] = NULL;
				lineArray[i-2] = NULL;
				break;
			}
			lineArray[i] = lineArray[i+2];
		}
		
		shift_positions(outPositions, outCount); //Shifts each output index in outPositions forward by 1.
		
		if(outPositions[0] > 0) {
			outPositions[0] -= 2; //Shifts output position back by 2 to account for deletion of ">" and "filename".
		}
		
		outCount -= 1;
	} 
}

/********************************************************************************************
* check_output will read the contents of file passed to it and duplicate it to standard input. 
*********************************************************************************************/
void check_input(char **lineArray, int *inPositions, int inCount, int tokenCount) {
	if(inCount > 0) {
		FILE *inputFile = fopen(lineArray[inPositions[0]+1], "r"); //Input file is created and opened.
		int inputFD = fileno(inputFile);
		dup2(inputFD, STDIN_FILENO); //File contents are duplicated to standard input.
		close(inputFD); //File is closed.
		
		//Shifts each element in lineArray forward by 2 to delete "<" and "filename" from lineArray.
		for(int i=inPositions[0]; i<=tokenCount; i++) {
			if(i == tokenCount) {
				//Last 2 elements are set as null.
				lineArray[i-1] = NULL;
				lineArray[i-2] = NULL;
				break;
			}
			lineArray[i] = lineArray[i+2];
		}
		
		shift_positions(inPositions, inCount); //Shifts each input index in outPositions forward by 1.
		
		if(inPositions[0] > 0) {
			inPositions[0] -= 2; //Shifts input position back by 2 to account for deletion of "<" and "filename".
		}
		
		inCount -= 1;
	}
}