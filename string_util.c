#include "common.h"

int read_word(const char *string_full, char **string_piece, char delim)
{
    static int count1 = 0;
    int count2 = 0, word_lenght = 0;
    while (string_full[word_lenght] != delim && string_full[word_lenght] != '\0')
    {
        word_lenght++;
    }
    *string_piece = malloc(word_lenght * sizeof (char));
    if ((*string_piece) == NULL)
        printf("NULL POINTER EXCEPTION\n");
    while (string_full[count1] != delim)
    {
        if (string_full[count1] == '\0')
        {
            count1 = 0;
            (*string_piece)[count2] = '\0';
            return FAILURE;
        }
        (*string_piece)[count2] = string_full[count1];
        count1++;
        count2++;
    }
    count1++;
    (*string_piece)[count2] = '\0';
    return SUCESS;

}

int count_words(const char *string, char delim)
{
    int count1 = 0;
    int delim_count = 1;
    while (string[count1] != '\0')
    {
        if (string[count1] == delim)
        {
            if (count1 != 0)
                if (string[count1-1] != delim)
                    delim_count++;
        }
        count1++;
    }
    return delim_count;
}

