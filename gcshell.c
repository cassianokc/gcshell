#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

int main(int argc, char **argv, char **env)
{
    char *input_string, path_piece[MAX_STRING];
    size_t input_lenght;
int status;
    printf("$ ");
    PATH = getenv("PATH");
    while (getline(&input_string, &input_lenght, stdin) != FAILURE)
    {
        input_string[strlen(input_string)-1] = '\0'; /* Takes the \n from the string. */
        while (separate_string(PATH, path_piece, MAX_STRING, ':') == SUCESS)
        {
            if (strlen(path_piece) + 1 + strlen(input_string) < MAX_STRING)
            {
                strcat(path_piece, "/");
                strcat(path_piece, input_string);
                if (file_exist(path_piece))
                {
                    int pid = 0;
                    pid = fork();
                    if (pid == -1)
                        exit(EXIT_FAILURE);
                    if (pid != 0)
                        wait(&status);
                    else
		    { 
                        execv(path_piece, argv);
		    }
                }
                    
            }
        }
        printf ("$ ");

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
