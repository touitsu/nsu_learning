#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>


typedef struct string{
    char* content;
    int32_t len;
}string;


struct stack{
    int32_t value;
    struct stack* next;
};
typedef struct stack stack;


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

}


int16_t getp(char operation) {
    switch (operation) {
        case '+': return 1;
        case '-': return 1;
        case '*': return 2;
        case '/': return 2;
        case '(': return 0;
        default: return -1;
    }
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
    int32_t value;

    top = (stack**)malloc(sizeof(stack*));
    *top = NULL;
    str.content = (char*)malloc(sizeof(char) * (2));
    str.len = 0;
    c = ' ';

    while(c != '\n') {
        c = getchar();

        while (c >= '0' && c <= '9') {
            str = concat(str, c);
            c = getchar();
            if (!(c >= '0' && c <= '9')) {
                str = concat(str, ' ');
            }
        }

        if (c == ')') {
            value = pop(top);
            while (value != '(') {
                str = concat(str, value);
                str = concat(str, ' ');
                value = pop(top);
            }
        }

        if (c == '(')
            push(top, c);

        if (getp(c) != -1 && c != '(') {
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
    }

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
        else if (getp(inp.content[i]) != -1){
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


int main() {
    string inp;

    inp = getinp();

    printf("%d\n", calculate(inp));

    return 0;
}
