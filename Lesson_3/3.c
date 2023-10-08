#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "fucntions.h"


int main() {
    char* str = getinput();
    printf("%d", cntunqchrs(str, 'A', 'z'));
    free(str);
    return 0;
}
