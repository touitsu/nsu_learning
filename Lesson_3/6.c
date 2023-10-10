#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* str;
    char** strarr;

    str = getinput();
    strarr = sprt(str, ' ');

    printrvrs(strarr);

    freematrix(strarr);
    free(str);

    return 0;
}
