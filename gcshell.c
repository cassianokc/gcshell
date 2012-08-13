#include <stdio.h>

#include "common.h"

int main(int argc, char **argv)
{
    char *input_string;
    size_t input_lenght;
    if (argc != 1)
        ;/* TODO: Process input, should be something like gcshell -option1 -option2... -optionx.*/
    while (getline(&input_string, &input_lenght, stdin) != FAILURE)
    {
        printf("%s", input_string);
    }
}