#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char *word, *help, *guessedword;
    char variant, guess;
    int attempt, len, win;

    help = getinput();

    word = getinput();

    win = 0;
    len = getstrlen(word);

    guessedword = (char*)malloc(sizeof(char) * (len + 1));

    for (int i = 0; i < len; i++)
        *(guessedword + i) = '*';
    
    *(guessedword + len) = '\0';

    for (int i = 0; i < 25; i++)
        printf(" \n");

    attempt = 0;

    while (attempt < 10 && win == 0) {

        printf("\n");
        printstr(help);
        printstr(guessedword);

        printf("letter (0) or word (1)?\n");
        variant = getchar();

        while(variant == '\n')
            variant = getchar();

        if (variant == '0') {

            guess = getchar();
            while(guess == '\n')
                guess = getchar();

            for (int i = 0; i < len; i++) {
                if (*(word + i) == guess)
                    *(guessedword + i) = *(word + i);
            }

            if (comparestr(guessedword, word))
                win = 1;

        }

        else if (variant == '1') {

            free(guessedword);

            guessedword = getinput();

            while (getstrlen(guessedword) == 0)
                guessedword = getinput();

            if (comparestr(guessedword, word))
                win = 1;
            else
                win = -1;
        }

        attempt++;
    }

    if (win == 1)
        printf("win!");
    else
        printf("loose!");
    
    free(guessedword);
    free(word);
    free(help);

    return 0;
}
