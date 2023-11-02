#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <stdint.h>


void printarr(int32_t* array, uint32_t len) {
    for (uint32_t i = 0; i < len; i++)
        printf("%i ", *(array + i));

    printf("\n");
}


void swap(int32_t* array, uint32_t a, uint32_t b) {
    int32_t tmp;

    tmp = *(array + a);
    *(array + a) = *(array + b);
    *(array + b) = tmp;
}


uint32_t partition(int32_t* array, uint32_t start, uint32_t end) {
    double pivot;

    pivot = (*(array + start) + *(array + end)) / 2.0;

    uint32_t i,j;

    i = start;
    j = end;

    while (1) {
        while (*(array + i) < pivot)
            i++;

        while (*(array + j) > pivot)
            j--;

        if (i >= j)
            return j;

        swap(array, i,j);
        i++;
        j--;
    }
}


void sort(int32_t* array, uint32_t start, uint32_t end) {
    if (start < end) {
        uint32_t p = partition(array, start, end);

        sort(array, start, p);
        sort(array, p+1, end);
    }
}


int32_t main() {
    uint32_t ammount;
    int32_t* array;

    scanf("%u", &ammount);
    if (ammount == 0)
        return 0;

    array = (int32_t*)malloc(sizeof(int32_t) * ammount);
    if (!array)
        return 1;

    for (uint32_t i = 0; i < ammount; i++)
        scanf("%i", array+i);

    sort(array, 0, ammount - 1);

    printarr(array, ammount);

    free(array);
    return 0;
}
