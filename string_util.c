#include "common.h"

int parse_path(char *path_piece)
/* Separates the $PATH enviroment variable. */
{
    static int count1 = 0;
    int count2=0;
    while (PATH[count1] != ':' && count2<MAX_STRING)
    {
        if (PATH[count1] == '\0')
        {
            count1 = 0;
            path_piece[count2] = '\0';
            return FAILURE;
        }
        path_piece[count2] = PATH[count1];
        count1++;
        count2++;
    }
    count1++;
    path_piece[count2] = '\0';
    return SUCESS;

}