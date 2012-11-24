#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include "common.h"
#include "extern.h"

void set_signals(void) {
    struct sigaction sigint, sigtstp, sigchld;
    if (memset(&sigint, 0, sizeof (struct sigaction)) == NULL ||
            memset(&sigtstp, 0, sizeof (struct sigaction)) == NULL ||
            memset(&sigchld, 0, sizeof (struct sigaction)) == NULL)
        fatal();
    sigint.sa_handler = handle_sigint;
    sigtstp.sa_handler = handle_sigtstp;
    sigchld.sa_flags = SA_SIGINFO;
    sigchld.sa_sigaction = handle_sigchld;
    sigaction(SIGINT, &sigint, NULL);
    sigaction(SIGTSTP, &sigtstp, NULL);
    sigaction(SIGCHLD, &sigchld, NULL);

}

void dfl_signals(void) {
    struct sigaction sigint, sigtstp;
    if (memset(&sigint, 0, sizeof (struct sigaction)) == NULL)
        fatal();
    if (memset(&sigtstp, 0, sizeof (struct sigaction)) == NULL)
        fatal();
    sigint.sa_handler = SIG_DFL;
    sigtstp.sa_handler = SIG_DFL;
    sigaction(SIGINT, &sigint, NULL);
    sigaction(SIGTSTP, &sigtstp, NULL);

}

void handle_sigint(int s) {
}

/* Sends sigcont to all processes that were stopped and shouldnt(were running in background).
 and adds cur_job to stopped list. */
void handle_sigtstp(int s) {
    struct JOB *aux;
    aux = jobs_list;
    while (aux != NULL) {
        if (is_running(aux))
            kill(aux->pid, SIGCONT);
        aux = aux->next_job;
    }
    if (cur_job != NULL && !job_exists(jobs_list, cur_job)) {
        cur_job->IS_RUNNING_FLAG = FALSE;
        cur_job->IS_FOREGROUND_FLAG = FALSE;
        add_job(&jobs_list, cur_job);
    }
}

void handle_sigchld(int s, siginfo_t *info, void *context) {
    struct JOB *aux1, *aux2;
    int count1;
    pid_t pid;
    pid = info->si_pid;
    aux1 = jobs_list;
    if (info->si_code == CLD_STOPPED || info->si_code == CLD_CONTINUED)
    {
        printf("blaah\n");
        return;
    }
    if (aux1 == NULL)
        return;
    if (aux1->next_job == NULL) {
        if (aux1->pid == pid) {
            printf("[%d]\t", aux1->pid);
            for (count1 = 0; count1 < aux1->num_args - 1; count1++)
                if (aux1->arg_strings[count1] != NULL)
                    printf(" %s", aux1->arg_strings[count1]);
            printf(" || Status: Terminated\n");
            free(aux1);
            jobs_list = NULL;
        }
        return;
    }
    while (aux1->next_job != NULL) {
        if (aux1->next_job->pid == pid) {
            aux2 = aux1->next_job;
            aux1->next_job = aux2->next_job;
            printf("[%d]\t", aux2->pid);
            for (count1 = 0; count1 < aux2->num_args - 1; count1++)
                if (aux2->arg_strings[count1] != NULL)
                    printf(" %s", aux2->arg_strings[count1]);
            printf(" || Status: Terminated\n");
            free(aux2);
        }
        aux1 = aux1->next_job;
    }
    return;
}