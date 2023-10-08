#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* str = getinput();
    char* str1 = getinput();
    char* str2 = getinput();
    int* res = sumarr(sumarr(cntamunqchrs(str), cntamunqchrs(str1)), cntamunqchrs(str2));
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
