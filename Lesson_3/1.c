#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char * str = getinput();
    printf("%d", countmaxspecconrepeats(str, ' '));
    free(str);
    return 0;
}
