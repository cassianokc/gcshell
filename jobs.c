#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

/* Allocates space for in *job and then parses in_string
 to create for each word a string. A NULL pointer 
 is put in the final position because execvp needs it. 
 in_string is coppied to string to make sure in_string is left
 untouched.*/
int string_to_job(char *in_string, struct JOB **job) {

    int count1 = 0;
    char *string;
    char *aux;
    string = malloc(strlen(in_string) * sizeof (char));
    *job = malloc(sizeof (struct JOB));
    if (string == NULL || job == NULL) // Check for memory leak
        return FAILURE;
    (*job)->num_args = 2;
    strcpy(string, in_string);
    while (string[count1] != '\0') { //Calculates the number of spaces in string
        if (string[count1] == ' ')
            (*job)->num_args++;
        count1++;
    }
    (*job)->arg_strings = malloc((*job)->num_args * sizeof (char *));
    if ((*job)->arg_strings == NULL) // Check for memory leak
        return FAILURE;
    count1 = 0;
    aux = strtok(string, " "); // Tokenize string spaces into \0 then runs through it.
    while (aux != NULL) {
        (*job)->arg_strings[count1] = malloc(strlen(aux) * sizeof (char));
        if ((*job)->arg_strings[count1] == NULL)
            return FAILURE;
        strcpy((*job)->arg_strings[count1], aux);
        aux = strtok(NULL, " ");
        count1++;
    }
    free(string);
    (*job)->arg_strings[(*job)->num_args - 1] = NULL;
    (*job)->IS_FOREGROUND_FLAG = TRUE;
    (*job)->IS_RUNNING_FLAG = TRUE;
    (*job)->next_job = NULL;
    return SUCCESS;
}

/* Returns TRUE if job j is running, FALSE otherwise.*/
int is_running(struct JOB *j) {
    return j->IS_RUNNING_FLAG;
}

/* Returns TRUE if job j is in foreground, FALSE otherwise(background).*/
int is_foreground(struct JOB *j) {
    return j->IS_FOREGROUND_FLAG;
}

/* Given a new_job pointer, appends it in j strucutre. Then makes j point to
 * this new job.*/
int add_job(struct JOB **j, struct JOB *new_job) {
    int count1;
    struct JOB *new_job_copy;
    new_job_copy = malloc(sizeof (struct JOB));
    if (new_job_copy == NULL)
        return FAILURE;
    *new_job_copy = *new_job;
    new_job_copy->arg_strings = malloc(new_job_copy->num_args * sizeof (char *));
    for (count1 = 0; count1 < new_job_copy->num_args; count1++) {
        if (new_job->arg_strings[count1] != NULL) {
            new_job_copy->arg_strings[count1] = malloc(strlen(new_job->arg_strings[count1]) * sizeof (char));
            if (new_job_copy->arg_strings[count1] == NULL)
                return FAILURE;
            strcpy(new_job_copy->arg_strings[count1], new_job->arg_strings[count1]);
        } else
            new_job_copy->arg_strings[count1] = new_job->arg_strings[count1];
    }
    new_job_copy->next_job = *j;
    *j = new_job_copy;
    return SUCCESS;
}

/* Deletes safely all jobs in a job structure. The *j pointer will point to NULL
 *  afterwise.*/
void delete_jobs(struct JOB **j) {
    struct JOB *aux1, *aux2;
    int count1;
    aux1 = *j;
    *j = NULL;
    if (aux1 != NULL)
        return;
    while ((aux2 = aux1->next_job) != NULL) { // Run through the structure
        for (count1 = 0; count1 < aux1->num_args; count1++) { //Frees the strings
            if (aux1->arg_strings[count1] != NULL) // Make sure dont free any NULL pointer
                free(aux1->arg_strings[count1]);
        }
        free(aux1->arg_strings); // Free the strings pointer
        free(aux1); // Free the structure itself.
        aux1 = aux2;
    }
    // Now free the structure node.
    for (count1 = 0; count1 < aux1->num_args; count1++) {
        if (aux1->arg_strings[count1] != NULL)
            free(aux1->arg_strings[count1]);
    }
    free(aux1->arg_strings);
    free(aux1);
    return;
}

/* Prints all jobs in a job structure.*/
void print_jobs(struct JOB *j) {
    int count1;
    struct JOB *aux;
    count1 = 1;
    aux = j;
    while (aux != NULL) {
        printf("[%d]\t", aux->pid);
        for (count1 = 0; count1 < aux->num_args - 1; count1++)
            if (aux->arg_strings[count1] != NULL)
                printf(" %s", aux->arg_strings[count1]);
        printf(" || Status: ");

        if (aux->IS_RUNNING_FLAG == TRUE)
            printf(" running");
        else printf("stopped");
        putchar('\n');
        aux = aux->next_job;
    }
}

struct JOB *wake_job(struct JOB **j, int foreground) {
    struct JOB *aux;
    aux = *j;
    *j = aux->next_job;
    aux->IS_RUNNING_FLAG = TRUE;
    aux->IS_FOREGROUND_FLAG = foreground;
    return aux;
}

int job_exists(struct JOB *list, struct JOB *j) {
    struct JOB *aux;
    aux = list;
    while (aux != NULL) {
        if (aux->pid == j->pid)
            return TRUE;
    }
    return FALSE;
}