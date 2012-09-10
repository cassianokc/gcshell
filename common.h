#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#define SUCESS 0
#define FAILURE -1
#define TRUE 1
#define FALSE 0
#define MAX_STRING 250
#define fatal()                                          \
        do { fprintf (stderr, "Error: %s: %d: %s: %s\n", \
         __FILE__, __LINE__,                             \
        __PRETTY_FUNCTION__, strerror (errno));          \
        exit (EXIT_FAILURE);} while (0)



const char *PATH;


int read_word(const char *, char **, char);
int count_words(const char *, char);

int file_exist(char *);


#endif

