#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* str;
    char** strarr;
    int len, winner, i;

    str = getinput();
    strarr = sprt(str, ' ');

    winner = -1;
    len = getstrarrlen(strarr);

    for (i = 0; i < len - 1 && winner == -1; i++) {
        if (*(*(strarr + i) + getstrlen(*(strarr + i)) - 1) != *(*(strarr + i + 1)))
            winner = i % 2;
    }

    if (winner == -1)
        winner = i % 2;

    if (winner == 0)
        printf("Petya won!");
    else
        printf("Vasya won!");


    freematrix(strarr);
    free(str);

    return 0;
}
