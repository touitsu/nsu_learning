#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char* word;
    char variant, guess;
    int attempt;


    word = getinput();

    for (int i = 0; i < 25; i++)
        printf(" \n");

    attempt = 0;

    while (attempt < 10) {
        printf("letter (0) or word (1)?\n");
        variant = getchar();

        if (variant == '0') {
            guess = getchar();

        }

        else if (variant == '1') {

        }

        attempt++;
    }

    free(word);

    return 0;
}
