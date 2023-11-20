#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>


typedef struct string{
    char* content;
    int32_t len;
}string;


typedef struct stack{
    int32_t value;
    struct stack* next;
}stack;


void push(stack** top, int32_t data) {
    stack* new;

    new = (stack*)malloc(sizeof(stack));
    new->value = data;

    new->next = *top;
    *top = new;
}


int32_t pop(stack** top) {
    stack* toremove;
    int32_t value;

    toremove = *top;
    value = toremove->value;

    *top = toremove->next;
    free(toremove);

    return value;

}


void freestack(stack** top) {
    while(*top != NULL)
        pop(top);

    free(top);
}


int16_t getp(char operation) {
    switch (operation) {
        case '+': return 1;
        case '-': return 1;
        case '*': return 2;
        case '/': return 2;
        case '(': return 0;
        case '0': return -1;
        case '1': return -1;
        case '2': return -1;
        case '3': return -1;
        case '4': return -1;
        case '5': return -1;
        case '6': return -1;
        case '7': return -1;
        case '8': return -1;
        case '9': return -1;
        case '\n': return -1;
        case ')': return -1;
        default: return -2;
    }
}


void syntaxerror(stack** top, string str) {
    printf("Syntax error\n");
    freestack(top);
    free(str.content);
    exit(0);
}


string concat(string str, char c) {
    str.content = (char*)realloc(str.content, sizeof(char) * (str.len + 2));
    str.content[str.len] = c;
    str.len++;
    return str;
}


string getinp() {
    string str;
    stack** top;
    char c;
    int32_t value, lastisoperation, numcount, opcount;

    top = (stack**)malloc(sizeof(stack*));
    *top = NULL;
    str.content = (char*)malloc(sizeof(char) * (2));
    str.len = 0;
    c = ' ';
    lastisoperation = 0;
    numcount = 0;
    opcount = 0;

    while(c != '\n') {
        c = getchar();

        while (c >= '0' && c <= '9') {
            lastisoperation = 0;
            str = concat(str, c);
            c = getchar();
            if (!(c >= '0' && c <= '9')) {
                str = concat(str, ' ');
                numcount++;
            }
        }

        if (c == ')') {
            if (lastisoperation || *top == NULL)
                syntaxerror(top, str);

            value = pop(top);
            while (value != '(') {
                if (*top == NULL)
                    syntaxerror(top, str);

                str = concat(str, value);
                str = concat(str, ' ');
                value = pop(top);
            }
        }

        if (c == '(') {
            lastisoperation = 1;
            push(top, c);
        }

        if (getp(c) > -1 && c != '(') {
            if (lastisoperation)
                syntaxerror(top, str);

            lastisoperation = 1;
            opcount++;

            if (*top != NULL) {
                while (*top != NULL && getp(c) <= getp((*top)->value)) {
                    value = pop(top);
                    str = concat(str, value);
                    str = concat(str, ' ');
                }
                push(top, c);
            }

            else
                push(top, c);

        }

        if(getp(c) == -2)
            syntaxerror(top, str);

    }

    if((opcount > 0 && numcount < 2) || str.len == 0)
        syntaxerror(top, str);

    while(*top != NULL) {
        value = pop(top);
        str = concat(str, value);
        str = concat(str, ' ');
    }

    return str;
}


int32_t findinstr(string str, int32_t i, char c) {
    for(int32_t j = i; j < str.len; j++)
        if(str.content[j] == c)
            return j;

    return -1;
}


int32_t pow(int32_t a, int32_t b) {
    int32_t res;

    res = 1;

    for (int32_t i = 0; i < b; i++)
        res *= a;

    return res;
}


int32_t getnum(string inp, int32_t i) {
    int32_t res;

    res = 0;

    for (int32_t j = findinstr(inp, i,' ') - 1; j >= i; j--)
        res += (inp.content[j] - '0') * pow(10, findinstr(inp, i,' ') - 1 - j);

    return res;
}


int32_t calculate(string inp) {
    stack** top;
    int32_t num1, num2, res;

    top = (stack**)malloc(sizeof(stack*));
    *top = NULL;

    for (int32_t i = 0; i < inp.len; i++) {
        if (inp.content[i] >= '0' && inp.content[i] <= '9') {
            push(top, getnum(inp, i));
            i = findinstr(inp, i, ' ');
        }
        else if (getp(inp.content[i]) > 0){
            num2 = pop(top);
            num1 = pop(top);
            if (inp.content[i] == '-')
                push(top, num1 - num2);
            else if (inp.content[i] == '+')
                push(top, num1 + num2);
            else if (inp.content[i] == '/') {
                if (num2 == 0) {
                    printf("Division by zero\n");
                    freestack(top);
                    exit(0);
                }
                push(top, num1 / num2);
            }
            else if (inp.content[i] == '*')
                push(top, num1 * num2);

        }
    }

    res = pop(top);

    return res;
}


int32_t main() {
    string inp;

    inp = getinp();

    printf("%d\n", calculate(inp));

    return 0;
}
