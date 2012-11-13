#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "common.h"

/* Checks args for builtin functions and executes them. */
int check_builtin(struct JOB *job)
{
    /*if (strcmp(arg[0], "cd") == 0 && size >= 2) {
	chdir(arg[1]);
	free_pointer_to_pointers(arg, size);
	return SUCCESS;
    }
    if (strcmp(arg[0], "exit") == 0) {
	free_pointer_to_pointers(arg, size);
	exit(EXIT_SUCCESS);
    }
    if (strcmp(arg[0], "help") == 0) {
	help();
	free_pointer_to_pointers(arg, size);
	return SUCCESS;
    }
    if (strcmp(arg[0], "jobs") == 0) {
	help();
	free_pointer_to_pointers(arg, size);
	return SUCCESS;
    }
    if (strcmp(arg[0], "bg") == 0) {
	help();
	free_pointer_to_pointers(arg, size);
	return SUCCESS;
    }
    if (strcmp(arg[0], "fg") == 0) {
	help();
	free_pointer_to_pointers(arg, size);
	return SUCCESS;
    }*/
    return FAILURE;
}

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
