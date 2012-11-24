#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "common.h"
#include "extern.h"

/* Checks job->arg_stringss for builtin functions and executes them. */
int check_builtin() {
    if (strcmp(cur_job->arg_strings[0], "cd") == 0 && cur_job->num_args >= 2) {
        chdir(cur_job->arg_strings[1]);
        return SUCCESS;
    }
    if (strcmp(cur_job->arg_strings[0], "exit") == 0) {
        exit(EXIT_SUCCESS);
    }
    if (strcmp(cur_job->arg_strings[0], "help") == 0) {
        help();
        return SUCCESS;
    }
    if (strcmp(cur_job->arg_strings[0], "jobs") == 0) {
        print_jobs(jobs_list);
        return SUCCESS;
    }
    if (strcmp(cur_job->arg_strings[0], "bg") == 0) {
        if (jobs_list != NULL) {
            delete_jobs(&cur_job);
            cur_job = wake_job(&jobs_list, FALSE);
            if (cur_job != NULL) {
                kill(cur_job->pid, SIGCONT);
            }
        }
        return SUCCESS;
    }
    if (strcmp(cur_job->arg_strings[0], "fg") == 0) {
        if (jobs_list != NULL) {
            int status;
            delete_jobs(&cur_job);
            cur_job = wake_job(&jobs_list, TRUE);
            if (cur_job != NULL)
                wait(&status);
        }
        return SUCCESS;
    }
    return FAILURE;
}

/* Opens the README file and prints it on the screen.*/
void help(void) {
    FILE *readme = fopen("README", "r");
    char c;
    if (readme != NULL) {
        while ((c = getc(readme)) != EOF)
            putchar(c);
        fclose(readme);
    } else
        printf("Couldn't find README.\n");
    return;
}
