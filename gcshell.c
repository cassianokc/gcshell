/**
 * Students:
 * Cassiano Kleinert Casagrande 7152819
 * Gustavo Livrare Martins 7277687
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>

#include "common.h"

int
main(int argc, char **argv) {
    char *input_string = NULL;

    size_t input_lenght, count1;
    for (count1 = 0; count1 < argc; count1++) { /* Checks for shell call parameters. */
        if (strcmp(argv[count1], "--help") == 0) {
            FILE *readme = fopen("README", "r");
            char *read_string = NULL;
            size_t input_lenght;
            if (readme != NULL) {
                while (getline(&read_string, &input_lenght, readme) != FAILURE)
                    printf("%s", read_string);
                fclose(readme);
            } else
                printf("Couldn't find README.\n");
        }
    }
    printf("$ ");
    while (getline(&input_string, &input_lenght, stdin) != FAILURE) { /* Reads user input from stdin. */
        process_string(input_string);
        printf("$ ");

    }
    free(input_string);
    return EXIT_SUCCESS;
}

void
process_string(char *input_string)
/* Counts number of words in input_string, then allocates **argv, after that, forks 
 * , tests the args for i/o redirection and execute the program called. */ {
    char **input_arg = NULL;
    size_t input_words, count1;
    int pid = 0;
    count1 = 0;
    input_string[strlen(input_string) - 1] = '\0'; /* Takes the \n from the string. */
    input_words = count_words(input_string, ' ');
    input_arg = (char **) malloc((input_words + 1) * sizeof (char *));
    if (input_arg == NULL) {
        fatal();
    }
    input_arg[input_words] = NULL;
    while (read_word(input_string, &input_arg[count1], ' ') == SUCESS) {

        count1++;
    }
    /* Check for built-in commands, if found, execute it, frees input_arg and return to main loop. */
    if (strcmp(input_arg[0], "cd") == 0 && input_words >= 2) {
        chdir(input_arg[1]);
        free_pointer_to_pointers(input_arg, input_words);
        return;
    }
    if (strcmp(input_arg[0], "exit") == 0) {
        free_pointer_to_pointers(input_arg, input_words);
        free(input_string);
        exit(EXIT_SUCCESS);
    }
    if (strcmp(input_arg[0], "help") == 0) {
        FILE *readme = fopen("README", "r");
        char *read_string = NULL;
        size_t input_lenght;
        if (readme != NULL) {
            while (getline(&read_string, &input_lenght, readme) != FAILURE)
                printf("%s", read_string);
            fclose(readme);
            free_pointer_to_pointers(input_arg, input_words);
        } else
            printf("Couldn't find README.\n");
        return;
    }
    pid = fork();
    if (pid == -1)
        fatal();
    if (pid != 0) { /* If parent. */
        int status;
        wait(&status);
        free_pointer_to_pointers(input_arg, input_words);
        return;
    } else {
        execute(input_arg, input_words);
    }
}

void
execute(char **arg, size_t words) {
    /* Searches for redirections and pipes in the arg list, treats them and then executes
     the prompt. */
    int count1;
    for (count1 = 0; count1 < words; count1++) {
        if (strcmp(arg[count1], "|") == 0) { /* Case found a pipe. */
            int pid;
            int pipefd[2];
            arg[count1] = NULL;
            count1++;
            if (pipe(pipefd) < 0)
                fatal();
            pid = fork(); /* Fork. */
            if (pid == -1)
                fatal();
            if (pid == 0) { /* If child. */
                close(STDOUT_DESC); /* Redirect input and breaks to execute the first command. */
                dup(pipefd[1]);
                close(pipefd[1]);
                close(pipefd[0]);
                break;

            } else { /* If parent. */
                int status;
                close(STDIN_DESC); /* Redirects output and execute the command after pipe. */
                dup(pipefd[0]);
                close(pipefd[0]);
                close(pipefd[1]);
                execute(arg + count1, words - count1);
                return;
            }
        }
        if (strcmp(arg[count1], ">") == 0) {
            int fd;
            arg[count1] = NULL;
            count1++;
            close(1);
            fd = open(arg[count1], O_CREAT | O_TRUNC | O_RDWR,
                    S_IRUSR | S_IWUSR);
            if (fd < 0) {
                fatal();
            }
        }
        if (strcmp(arg[count1], "<") == 0) {
            int fd;
            arg[count1] = NULL;
            count1++;
            close(0);
            fd = open(arg[count1], O_RDWR, S_IRUSR | S_IWUSR);
            if (fd < 0) {
                fatal();
            }

        }
        if (strcmp(arg[count1], ">>") == 0) {
            int fd;
            arg[count1] = NULL;
            count1++;
            close(1);
            fd = open(arg[count1], O_CREAT | O_APPEND | O_RDWR,
                    S_IRUSR | S_IWUSR);
            if (fd < 0) {
                fatal();
            }

        }
    }
    execvp(arg[0], arg);
    printf("Command not found: %s\n", arg[0]); /* Execvp didn't executed correctly. */
    exit(EXIT_FAILURE);
}

void
free_pointer_to_pointers(char **p, int n)
/* Frees all pointers in p[0], p[1]..., p[n-1], then frees p. */ {
    size_t count1;
    for (count1 = 0; count1 < n; count1++) {
        free(p[count1]);
    }
    free(p);
}
