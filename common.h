#ifndef COMMON_H
#define COMMON_H

#include <stdlib.h>

#define SUCESS 0
#define FAILURE -1
#define TRUE 1
#define FALSE 0

#define MAX_STRING 250

const char *PATH;



int parse_path(char *);
int file_exist(char *);


#endif