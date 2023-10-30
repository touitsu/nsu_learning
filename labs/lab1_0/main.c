#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef struct string {
    char* content;
    uint16_t len;
}string;


string getinput(FILE* file) {
    string res;

    res.content = (char*)malloc(sizeof(char) * 2);
    if (res.content == NULL)
        exit(1);

    res.len = 0;

    for (int16_t i = 0, c = (int16_t)fgetc(file); (char)c != '\n' && c != -1; i++, c = fgetc(file)) {
        res.content = (char*)realloc(res.content, sizeof(char) * (i + 2));
        if (res.content == NULL)
            exit(1);

        *(res.content + i) = (char)c;
        *(res.content + i + 1) = '\0';
        res.len = i;
    }

    return res;
}


string getpart(FILE* file, string str, uint16_t step) {
    int16_t c;

    for (int16_t i = step; i < str.len + 1; i++)
        *(str.content + i  - step) = *(str.content + i);

    for (int16_t i = str.len - step + 1; i <= str.len && c != -1; i++) {
        c = fgetc(file);
        if (c != -1)
            *(str.content + i) = (char)c;

        else
            *(str.content) = '\0';
    }

    return str;
}


void boyer_moore(FILE* file, string tofind, string part) {
    uint64_t curpos;
    uint16_t* indexes;
    int16_t i;
    uint8_t flag;

    indexes = (uint16_t*)malloc(sizeof(uint16_t) * 128);
    if (indexes == NULL)
        exit(1);

    for (i = 0; i < 128; i++)
        *(indexes + i) = 0;

    for (i = (int16_t)tofind.len - 1; i >= 0; i--)
        if(*(indexes + *(tofind.content + i)) == 0)
            *(indexes + *(tofind.content + i)) = (int16_t) tofind.len - i;

    part.content = (char*)malloc(sizeof(char) * (tofind.len + 1));
    if (part.content == NULL)
        exit(1);

    *(part.content + tofind.len) = '\0';
    part.len = tofind.len;
    part = getpart(file, part, tofind.len + 1);

    curpos = tofind.len + 1;
    flag = 0;
    i = tofind.len;

    if (*part.content == '\0')
        flag = 1;

    while (!flag) {

        printf("%llu ", curpos + (i - tofind.len));

        if (*(part.content + i) != *(tofind.content + i)) {
            if (*(indexes + *(part.content + i)) == 0 || i == 0) {

                if (i == tofind.len || i == 0) {
                    curpos += tofind.len  + 1;
                    part = getpart(file, part, tofind.len  + 1);
                    i = tofind.len;
                }
                else {
                    curpos += tofind.len;
                    part = getpart(file, part, tofind.len );
                    i = tofind.len;
                }
            }

            else {
                curpos += *(indexes + *(part.content + i));
                part = getpart(file, part, *(indexes + *(part.content + i)));
                i = tofind.len;
            }
        }

        else {
            i--;

            if (i == -1) {
                curpos += tofind.len + 1;
                i = tofind.len;
                part = getpart(file, part, i + 1);
            }
        }

        if (*part.content == '\0')
            flag = 1;
    }

    free(part.content);
    free(indexes);
}


uint16_t main() {
    string tofind, inp;
    FILE* file;

    file = fopen("in.txt", "r");

    tofind = getinput(file);

    boyer_moore(file, tofind, inp);

    fclose(file);

    free(tofind.content);

	return 0;
}