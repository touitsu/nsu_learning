#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include "functions.h"


int main() {
    char *word, *guessedword;
    int attempt, bulls, cows;

    word = getinput();

    while(cntunqchrs(word, '0', '9') != 4 && getstrlen(word) != 4)
        word = getinput();

    for (int i = 0; i < 25; i++)
        printf(" \n");

    bulls = 0;
    cows = 0;
    attempt = 0;

    while (attempt < 10 && bulls != 4) {

        bulls = 0;
        cows = 0;

        guessedword = getinput();
        while (getstrlen(guessedword) == 0)
            guessedword = getinput();

        for (int i = 0; i < 4; i++) {
            if (*(guessedword + i) == *(word + i))
                bulls += 1;
            else if (*(guessedword + i) != *(word + i) && isinstr(*(guessedword + i), word))
                cows += 1;

        }
        
        printf("Bulls: %d Cows: %d\n", bulls, cows);
 
        free(guessedword);
        attempt++;
    }

    if (bulls == 4)
        printf("Win!");
    else
        printf("Loose!");

    free(word);

    return 0;
}

