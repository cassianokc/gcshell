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

int main(int argc, char **argv) {
    int count1;
    char *input_string;
    size_t input_lenght;
    struct JOB *cur_job;
    input_string = NULL;
    cur_job = NULL;
    for (count1 = 0; count1 < argc; count1++) { // Checks for shell call parameters.
        if (strcmp(argv[count1], "--help") == 0) {
            help();
        }
    }
    printf("$ ");
    while (getline(&input_string, &input_lenght, stdin) != FAILURE) {
        input_string[strlen(input_string) - 1] = '\0'; //Takes the \n from the string end. 
        string_to_job(input_string, &cur_job); // Converts string to a job structure
        process_job(cur_job); // Process the input job.
        delete_jobs(&cur_job); // Deletes it
        printf("$ "); // Prompts user for another input.

    }
    free(input_string);
    return EXIT_SUCCESS;
}

/* Forks the shell process and makes it wait while the child process executes the
job passed. */
void process_job(struct JOB *j) {
    int pid;
    pid = 0;
    // Check for built-in commands, if found, execute it, frees input_arg and return to main loop
    if (check_builtin(j) == SUCCESS)
        return;
    pid = fork();
    if (pid == -1)
        fatal();
    if (pid != 0) { // If parent
        int status;
        wait(&status);
        return;
    } else {
        execute(j->arg_strings, j->num_args);
    }
}

/* Searches for redirections and pipes in the arg list, treats them and then executes
the prompt. */
void execute(char **arg, int words) {
    int count1;
    for (count1 = 0; count1 < words - 1; count1++) {
        if (strcmp(arg[count1], "|") == 0) { // Case found a pipe
            int pid;
            int pipefd[2];
            free(arg[count1]);
            arg[count1] = NULL;
            count1++;
            if (pipe(pipefd) < 0)
                fatal();
            pid = fork(); // Fork
            if (pid == -1)
                fatal();
            if (pid == 0) { //If child
                close(STDOUT_DESC); // Redirect output and breaks to execute the first command
                dup(pipefd[1]);
                close(pipefd[1]);
                close(pipefd[0]);
                break;

            } else { // If parent.
                close(STDIN_DESC); // Redirects input and execute the command after pipe
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
            close(STDOUT_DESC);
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
            close(STDIN_DESC);
            fd = open(arg[count1], O_RDWR, S_IRUSR | S_IWUSR);
            if (fd < 0) {
                fatal();
            }

        }
        if (strcmp(arg[count1], ">>") == 0) { 
            int fd;
            arg[count1] = NULL;
            count1++;
            close(STDOUT_DESC);
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

