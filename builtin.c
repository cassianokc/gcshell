#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"
#include "extern.h"

/* Checks job->arg_stringss for builtin functions and executes them. */
int check_builtin()
{
    if (strcmp(cur_job->arg_strings[0], "cd") == 0 && cur_job->num_args >= 2) {
	chdir(cur_job->arg_strings[1]);
	delete_jobs(&cur_job);
	return SUCCESS;
    }
    if (strcmp(cur_job->arg_strings[0], "exit") == 0) {
	delete_jobs(&cur_job);
	exit(EXIT_SUCCESS);
    }
    if (strcmp(cur_job->arg_strings[0], "help") == 0) {
	help();
	delete_jobs(&cur_job);
	return SUCCESS;
    }
    if (strcmp(cur_job->arg_strings[0], "cur_jobs") == 0) {
        print_jobs(jobs_list);
	delete_jobs(&cur_job);
	return SUCCESS;
    }
    if (strcmp(cur_job->arg_strings[0], "bg") == 0) {
	help();
	delete_jobs(&cur_job);
	return SUCCESS;
    }
    if (strcmp(cur_job->arg_strings[0], "fg") == 0) {
	help();
	delete_jobs(&cur_job);
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
