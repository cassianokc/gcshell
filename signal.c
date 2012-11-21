#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "common.h"
#include "extern.h"

void set_signals(void) {
    struct sigaction sigint;
    if (memset(&sigint, 0, sizeof (struct sigaction)) == NULL)
        fatal();
    sigint.sa_handler = redirect_sigint;
    sigaction(SIGINT, &sigint, NULL);

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

void redirect_sigint(int p) {

}

void redirect_sigtstp(int p) {

}