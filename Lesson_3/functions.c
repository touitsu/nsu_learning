#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>


int maximum(int a, int b){
    if (a>b)
        return a;
    return b;
}

char* getInput(){
    char* string = (char*)malloc(sizeof(char) * 2);
    char c = getchar();
    int i = 0;
    while(c != '\n'){
        *(string + i) = c;
        c = getchar();
        i ++;
        string = (char*)realloc(string, sizeof(char) * (i+2));
        *(string + i + 1) = '\0';
    }
    return string;
}

void printstr(char* string){
    int i = 0;
    while (*(string + i) != '\0'){
        printf("%c", *(string + i));
        i++;
    }
    printf("\n");
}

int countmaxspecconrepeats(char* string, char repeat){ //count maximum amount of specified contionous repeats
    int res = 0, curres = 0, i = 0;
    while(*(string + i) != '\0'){
        if (repeat == *(string + i))
            curres ++;
        else{
            res = maximum(res,curres);
            curres = 0;
        }
        i++;
    }
    res = maximum(res,curres);
    return res;
}

int countmaxconrepeats(char* string){
    int res = 0, curres = 0, i = 0, lastc = -1;
    while(*(string + i) != '\0'){
        if(*(string + i) == lastc)
            curres ++;

        else{
            res = maximum(curres,res);
            curres = 1;
            lastc = *(string + i);
        }
        i++;
    }
    res = maximum(curres,res);
    return res;
}

int isinstr(char c, char* string){
    int i = 0;
    while (*(string + i)){
        if(*(string + i) == c)
            return 1;

    }
    return 0;
}

int cntunqchrs(char* string){                          // count unique chars
    int res = 0, i = 0;
    char* strofunqchars = (char*)malloc(sizeof(char) * 2);
    return res;
}

