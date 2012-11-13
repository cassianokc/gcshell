#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "common.h"

int string_to_job(char *in_string, struct JOB **job) {
    int count1 = 0;
    char *string;
    char *aux;
    string = malloc(strlen(in_string) * sizeof (char));
    *job = malloc(sizeof (struct JOB));
    if (string == NULL || job == NULL)
        return FAILURE;
    (*job)->num_args = 2;
    strcpy(string, in_string);
    while (string[count1] != '\0') {
        if (string[count1] == ' ')
            (*job)->num_args++;
        count1++;
    }
    (*job)->arg_strings = malloc((*job)->num_args * sizeof (char *));
    if ((*job)->arg_strings == NULL)
        return FAILURE;
    count1 = 0;
    aux = strtok(string, " ");
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

int is_running(struct JOB *j) {
    return j->IS_RUNNING_FLAG;
}

int is_foreground(struct JOB *j) {
    return j->IS_FOREGROUND_FLAG;
}

int add_job(struct JOB **j, struct JOB *new_job) {
    struct JOB *new_job_copy;
    new_job_copy = malloc(sizeof (struct JOB));
    if (new_job_copy == NULL)
        return FAILURE;
    *new_job_copy = *new_job;
    new_job_copy->next_job = *j;
    *j = new_job_copy;
    return SUCCESS;
}

void delete_jobs(struct JOB **j) {
    struct JOB *aux1, *aux2;
    int count1;
    aux1 = *j;
    *j = NULL;
    if (aux1 != NULL)
        return;
    while ((aux2 = aux1->next_job) != NULL) {
        for (count1 = 0; count1 < aux1->num_args; count1++) {
            if (aux1->arg_strings[count1] != NULL)
                free(aux1->arg_strings[count1]);
        }
        free(aux1->arg_strings);
        free(aux1);
        aux1 = aux2;
    }
    for (count1 = 0; count1 < aux1->num_args; count1++) {
        if (aux1->arg_strings[count1] != NULL)
            free(aux1->arg_strings[count1]);
    }
    free(aux1->arg_strings);
    free(aux1);
    return;
}

void print_jobs(struct JOB *j) {
    int count1, count2;
    struct JOB *aux;
    count1 = 1;
    aux = j;
    while (aux != NULL) {
        printf("[%d]\t", count1);
        for (count2 = 0; count2 < aux->num_args; count2++)
            printf(" %s", aux->arg_strings[count2]);
        putchar('\n');
        aux = aux->next_job;
        count1++;
    }
}
