#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"

int main(int argc, char **argv, char **env)
{
    char *input_string = NULL, **input_arg = NULL;
    size_t input_lenght = 0, input_words, count1;
    int pid = 0;
    printf("$ ");

    while (getline(&input_string, &input_lenght, stdin) != FAILURE)
    {
        count1 = 0;
        input_string[strlen(input_string) - 1] = '\0'; /* Takes the \n from the string. */
        input_words = count_words(input_string, ' ');
        input_arg = (char **) malloc((input_words + 1) * sizeof (char *));
        input_arg[input_words] = NULL;
        while (read_word(input_string, &input_arg[count1], ' ') == SUCESS)
        {
            
            count1++;
        }

        pid = fork();
        if (pid == -1)
            fatal();
        if (pid != 0)
        {
            wait();
            for (count1 = 0; count1 < input_words; count1++)
                free(input_arg[count1]);
            free(input_arg);
        }
        else
        {
            execvp(input_arg[0], input_arg);
        }
        printf("$ ");
    }
    free(input_string);
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
