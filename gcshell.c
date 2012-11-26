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

struct JOB *jobs_list;
struct JOB *cur_job;

int main(int argc, char **argv) {
    int count1, result;
    char *input_string;
    size_t input_lenght;
    input_string = NULL;
    cur_job = NULL;
    set_signals();
    setbuf(stdin, NULL); // Make stdin/out streams unbuffereds.
    setbuf(stdout, NULL);
    for (count1 = 0; count1 < argc; count1++) { //Check for shell call parameters
        if (strcmp(argv[count1], "--help") == 0) {
            help();
        }
    }
    printf("$ ");
    while ((result = read_string(&input_string, &input_lenght, stdin)) != 0) {
        if (result == -1 || strcmp(input_string, "") == 0) {
            printf("\n$ ");
            continue;
        }
        
        string_to_job(input_string, &cur_job); //Convert string to a job structure
        process_job(); // Process the input job.
        delete_jobs(&cur_job); // Delete it
        printf("$ ");
    }
    free(input_string);
    return EXIT_SUCCESS;
}

/* Forks the shell process and makes it wait while the child process executes the
job passed. */
void process_job() {
    int pid;
    pid = 0;
    //Check for built-in commands, if found, execute it, frees input_arg and return to main loop
    if (check_builtin() == SUCCESS)
        return;
    pid = fork();
    if (pid == -1)
        fatal();
    if (pid != 0) { //If parent
        int status;
        cur_job->pid = pid;
        if (strcmp(cur_job->arg_strings[cur_job->num_args - 2], "&") == 0) {
            cur_job->IS_FOREGROUND_FLAG = FALSE;
            free(cur_job->arg_strings[cur_job->num_args - 2]);
            cur_job->arg_strings[cur_job->num_args - 2] = NULL;
            cur_job->num_args--;
            add_job(&jobs_list, cur_job);
        }
        if (is_foreground(cur_job))
            waitpid(cur_job->pid, &status, 0);

    } else {
        cur_job->pid = getpid();
        if (strcmp(cur_job->arg_strings[cur_job->num_args - 2], "&") == 0) {
            cur_job->IS_FOREGROUND_FLAG = FALSE;
            free(cur_job->arg_strings[cur_job->num_args - 2]);
            cur_job->arg_strings[cur_job->num_args - 2] = NULL;
            cur_job->num_args--;
        }
        execute(cur_job->arg_strings, cur_job->num_args);
    }
}

/* Searches for redirections and pipes in the arg list, treats them and then executes
the prompt.*/
void execute(char **arg, int words) {
    int count1;
    for (count1 = 0; count1 < words - 1; count1++) {
        if (strcmp(arg[count1], "|") == 0) { //Case found a pipe
            int pid;
            int pipefd[2];
            free(arg[count1]);
            arg[count1] = NULL;
            count1++;
            if (pipe(pipefd) < 0)
                fatal();
            pid = fork(); //Fork
            if (pid == -1)
                fatal();
            if (pid == 0) { //If child
                close(STDOUT_DESC); //Redirect output and breaks to execute the first command
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
    printf("Command not found: %s\n", arg[0]); //Execvp didn't executed correctly
    exit(EXIT_FAILURE);
}

int read_string(char **string, size_t *n, FILE *file) {
    int count1;
    char *s;
    ssize_t result;
    size_t cur_size;
    count1 = 0;
    if (*string == NULL) {
        cur_size = 50;
        s = malloc(cur_size * sizeof (char));
    } else {
        s = *string;
        cur_size = *n;
    }
    while (1) {
        result = read(fileno(file), s + count1, 1);
        if (result == -1) {
            if (ferror(file) == EINTR || ECHILD)
                clearerr(file);
            return -1;

        }
        if (s[count1] == '\n' || result == 0) {
            s[count1] = '\0';
            *n = cur_size;
            *string = s;
            return count1 + result;
        }
        count1++;
        if (count1 == cur_size) {
            cur_size = cur_size + 50;
            s = realloc(s, cur_size);
            if (s == NULL)
                return 0;
        }
    }
    clearerr(stderr);
}
