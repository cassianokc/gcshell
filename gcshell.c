#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "common.h"

int main(int argc, char **argv)
{
    char *input_string = NULL;
    size_t input_lenght, count1;
    for (count1=0; count1<argc; count1++) /* Checks for shell call parameters. */
    {
        if (strcmp(argv[count1], "--help") == 0)
        {
            printf("%s %s %s\n %s\n", "A simple shell made by Gustavo Livrare Martins(my colleague)",
                "and Cassiano Kleinert Casagrande(me! =D) for the Operating Systems II course,",
                "in Universidade de São Paulo.",
                "Built-in commands are exit and cd.");
        }
    }
    printf("$ ");
   
    while (getline(&input_string, &input_lenght, stdin) != FAILURE) /* Reads user input from stdin. */
    {        process_string(input_string); 
        printf("$ ");

    }
    free(input_string);
    return EXIT_SUCCESS;
}

void process_string(char *input_string)
/* Counts number of words in input_string, then allocates **argv, after that, forks 
 * , tests the args for i/o redirection and execute the program called. */
{
    char **input_arg = NULL;
    size_t input_words, count1;
    int pid = 0;
    count1 = 0;
    input_string[strlen(input_string) - 1] = '\0'; /* Takes the \n from the string. */
    input_words = count_words(input_string, ' ');
    input_arg = (char **) malloc((input_words + 1) * sizeof (char *));
    input_arg[input_words] = NULL;
    while (read_word(input_string, &input_arg[count1], ' ') == SUCESS)
    {

        count1++;
    }
    /* Check for built-in commands, if found, execute it, frees input_arg and return to main loop.*/
    if (strcmp(input_arg[count1], "cd") == 0 && input_words >= 2)
    {
        chdir(input_arg[1]);
        free_input_args(input_arg, input_words);
        return;
    }
    if (strcmp(input_arg[count1], "exit") == 0)
    {
        free_input_args(input_arg, input_words);
        free(input_string);
        exit(EXIT_SUCCESS);
    }
    pid = fork();
    if (pid == -1)
        fatal();
    if (pid != 0)
    {
        wait();
        free_input_args(input_arg, input_words);
        return;
    }
    else
    {
        for (count1 = 0; count1 < input_words; count1++)
        {
            if (strcmp(input_arg[count1], ">") == 0)
            {
                  int fd;
                  input_arg[count1] =  NULL;
                  count1++;
                  close(1);
                  fd = open(input_arg[count1], O_CREAT | O_TRUNC | O_RDWR,  S_IRUSR | S_IWUSR);
                  if (fd < 0)
                  {
                      fatal();
                  }
            }
            if (strcmp(input_arg[count1], "<") == 0)
            {
                  int fd;
                  input_arg[count1] =  NULL;
                  count1++;
                  close(0);
                  fd = open(input_arg[count1], O_CREAT | O_TRUNC | O_RDWR,  S_IRUSR | S_IWUSR);
                  if (fd < 0)
                  {
                      fatal();
                  }

            }
            if (strcmp(input_arg[count1], ">>") == 0)
            {
                  int fd;
                  input_arg[count1] =  NULL;
                  count1++;
                  close(1);
                  fd = open(input_arg[count1], O_CREAT | O_APPEND | O_RDWR,  S_IRUSR | S_IWUSR);
                  if (fd < 0)
                  {
                      fatal();
                  }

            }
        }

        execvp(input_arg[0], input_arg);
    }
}

void free_input_args(char **arg, int n)
/**/
{
    size_t count1;
    for (count1 = 0; count1 < n; count1++)
    {
        free(arg[count1]);
    }
    free(arg);
}
