#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* str;
    int* res;

    str = getinput();
    res = cntamunqchrs(str);

    printf("%c", findinarr(res, 3) + ' ' - 1);

    free(str);
    free(res);

    return 0;
}
