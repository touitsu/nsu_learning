#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* str;
    char** keywords;
    int res;

    str = getinput();

    res = 1;

    if (cntunqchrs(str, ' ', '/') || cntunqchrs(str, ':', '@') || 
        cntunqchrs(str, '[', '^') || cntunqchrs(str, '`', '`') || cntunqchrs(str, '{', '~'))
        res = 0;

    if (*str >= '0' && *str <= '9')
        res = 0;

    keywords = pythonkeywords();

    for (int i = 0; i < 33; i++)
        if (comparestr(str, keywords[i]))
            res = 0;

    if (res)
        printf("Can be used.");
    else
        printf("Can not be used.");

    freematrix(keywords);
    free(str);

    return 0;
}
