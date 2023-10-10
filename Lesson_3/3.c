#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* str;

    str = getinput();

    printf("%d", cntunqchrs(str, 'A', 'Z') + cntunqchrs(str, 'a', 'z'));
    
    free(str);
    
    return 0;
}
