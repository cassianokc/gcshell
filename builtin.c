#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

/* Checks job->arg_stringss for builtin functions and executes them. */
int check_builtin(struct JOB *job)
{
    if (strcmp(job->arg_strings[0], "cd") == 0 && job->num_args >= 2) {
	chdir(job->arg_strings[1]);
	delete_jobs(&job);
	return SUCCESS;
    }
    if (strcmp(job->arg_strings[0], "exit") == 0) {
	delete_jobs(&job);
	exit(EXIT_SUCCESS);
    }
    if (strcmp(job->arg_strings[0], "help") == 0) {
	help();
	delete_jobs(&job);
	return SUCCESS;
    }
    if (strcmp(job->arg_strings[0], "jobs") == 0) {
	help();
	delete_jobs(&job);
	return SUCCESS;
    }
    if (strcmp(job->arg_strings[0], "bg") == 0) {
	help();
	delete_jobs(&job);
	return SUCCESS;
    }
    if (strcmp(job->arg_strings[0], "fg") == 0) {
	help();
	delete_jobs(&job);
	return SUCCESS;
    }
    return FAILURE;
}

/* Opens the README file and prints it on the screen.*/
void help(void)
{
    FILE *readme = fopen("README", "r");
    char *read_string = NULL;
    size_t input_lenght;
    if (readme != NULL) {
	while (getline(&read_string, &input_lenght, readme) != FAILURE)
	    printf("%s", read_string);
	fclose(readme);
    } else
	printf("Couldn't find README.\n");
    return;
}
