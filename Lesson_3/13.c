#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* str;
    int* num;
    int res, sum1, sum2;

    res = 0;

    while (res < 1) {
        str = getinput();
        
        num = fromstrtointarr(str);

        sum1 = 0;
        sum2 = 0;

        if (*num % 2 == 1) {

            for (int i = 1; i < (*num - 1) / 2 + 1; i++)
                sum1 += *(num + i);

            for (int i = (*num - 1) / 2 + 1; i < *num; i++)
                sum2 += *(num + i);

            if(sum1 == sum2)
                res = 1;

        }

        free(str);
        free(num);
    }

    return 0;
}
