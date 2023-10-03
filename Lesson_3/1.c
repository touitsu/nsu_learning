#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "fucntions.h"


int main() {
    char * str = getInput();
    printf("%d", countmaxspecconrepeats(str, ' '));
    free(str);
    return 0;
}
