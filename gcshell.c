#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

int main(int argc, char **argv, char **env)
{
    char *input_string, input_piece[MAX_STRING];
    size_t input_lenght;
				    int pid = 0;
    printf("$ ");
    PATH = getenv("PATH");
    while (getline(&input_string, &input_lenght, stdin) != FAILURE) 
	{
		input_string[strlen(input_string) - 1] = '\0';	/* Takes the \n from the string. */
		char input_arg[50][50];
		int count1=0;
		while (separate_string(input_string, input_piece, MAX_STRING, ' ') == SUCESS)
		{
					strcpy(input_arg[count1], input_piece);
					count1++;
					printf("%s\n", input_piece);


		}
if (pid == -1)
						exit(EXIT_FAILURE);
		  		    if (pid != 0)
						wait();
				    else 
					{
						execvp(argv[0], argv);
				    }
		printf("$ ");
    }
    return EXIT_SUCCESS;
}

int file_exist(char *filename)
{
    FILE *f = fopen(filename, "r");
    if (!f)
	return FALSE;
    fclose(f);
    return TRUE;
}
