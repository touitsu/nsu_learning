#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* str;
    char** strarr;
    int len;

    str = getinput();
    strarr = sprt(str, ' ');

    len = getstrarrlen(strarr);

    for (int i = 1; i < len; i++) {
        if (comparestr(*strarr, *(strarr + i)) == 0 && !hasrepeatables(*(strarr + i)))
            printstr(*(strarr + i));
    }

    freematrix(strarr);
    free(str);

    return 0;
}
