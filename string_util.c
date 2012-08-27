#include "common.h"

int separate_string(const char *string_full, char *string_piece, int max, char delim)
{
    static int count1 = 0;
    int count2=0;
    while (string_full[count1] != delim && count2<max)
    {
        if (string_full[count1] == '\0')
        {
            count1 = 0;
            string_piece[count2] = '\0';
            return FAILURE;
        }
        string_piece[count2] = string_full[count1];
        count1++;
        count2++;
    }
    count1++;
    string_piece[count2] = '\0';
    return SUCESS;

}
