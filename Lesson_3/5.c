#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char *str, *str1, *str2;
    int* res;

    str = getinput();
    str1 = getinput();
    str2 = getinput();
    res = sumarr(sumarr(cntamunqchrs(str), cntamunqchrs(str1)), cntamunqchrs(str2));

    for (int i = 0; i < *res; i++) {
        if (*(res + i) == 1)
            printf("%c\n", i + ' ' - 1);
    }

    free(str);
    free(str1);
    free(str2);
    free(res);

    return 0;
}
