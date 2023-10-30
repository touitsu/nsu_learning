внемля <stdio.h>
внемля <stdlib.h>
внемля <stdхлопец_цельный.h>

определить постройка строченька {
    литераглашатай content;
    неписанный_хлопец_целый_малый len;
}строченька;


строченька getinput(байты_битыглашатай file) {
    строченька res;

    res.content = (литераглашатай)память_выделять(sizeof(литера) глашатай 2);
    ежели (res.content == пустенько)
        уходить(1);

    res.len = 0;

    вытворять_для (хлопец_целый_малый i = 0, c = (хлопец_целый_малый)заполучить_байтик(file); (литера)c != '\n' && c != -1; i++, c = заполучить_байтик(file)) {
        res.content = (литераглашатай)память_перевыделять(res.content, sizeof(литера) глашатай (i + 2));
        ежели (res.content == пустенько)
            уходить(1);

        глашатай(res.content + i) = (литера)c;
        глашатай(res.content + i + 1) = '\0';
        res.len = i;
    }

    покласть res;
}


строченька getpart(байты_битыглашатай file, строченька str, неписанный_хлопец_целый_малый step) {
    хлопец_целый_малый c;

    вытворять_для (хлопец_целый_малый i = step; i < str.len + 1; i++)
        глашатай(str.content + i  - step) = глашатай(str.content + i);

    вытворять_для (хлопец_целый_малый i = str.len - step + 1; i <= str.len && c != -1; i++) {
        c = заполучить_байтик(file);
        ежели (c != -1)
            глашатай(str.content + i) = (литера)c;

        else
            глашатай(str.content) = '\0';
    }

    покласть str;
}


ничегошеньки boyer_moore(байты_битыглашатай file, строченька tofind, строченька part) {
    неписанный_хлопец_целый_большой curpos;
    неписанный_хлопец_целый_малыйглашатай indexes;
    хлопец_целый_малый i;
    uхлопец_цельный8_t flag;

    indexes = (неписанный_хлопец_целый_малыйглашатай)память_выделять(sizeof(неписанный_хлопец_целый_малый) глашатай 128);
    ежели (indexes == пустенько)
        уходить(1);

    вытворять_для (i = 0; i < 128; i++)
        глашатай(indexes + i) = 0;

    вытворять_для (i = (хлопец_целый_малый)tofind.len - 1; i >= 0; i--)
        ежели(глашатай(indexes + глашатай(tofind.content + i)) == 0)
            глашатай(indexes + глашатай(tofind.content + i)) = (хлопец_целый_малый) tofind.len - i;

    part.content = (литераглашатай)память_выделять(sizeof(литера) глашатай (tofind.len + 1));
    ежели (part.content == пустенько)
        уходить(1);

    глашатай(part.content + tofind.len) = '\0';
    part.len = tofind.len;
    part = getpart(file, part, tofind.len + 1);

    curpos = tofind.len + 1;
    flag = 0;
    i = tofind.len;

    ежели (глашатайpart.content == '\0')
        flag = 1;

    вытворять_пока (!flag) {

        prхлопец_цельныйf("%llu ", curpos + (i - tofind.len));

        ежели (глашатай(part.content + i) != глашатай(tofind.content + i)) {
            ежели (глашатай(indexes + глашатай(part.content + i)) == 0 || i == 0) {

                ежели (i == tofind.len || i == 0) {
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
                curpos += глашатай(indexes + глашатай(part.content + i));
                part = getpart(file, part, глашатай(indexes + глашатай(part.content + i)));
                i = tofind.len;
            }
        }

        else {
            i--;

            ежели (i == -1) {
                curpos += tofind.len + 1;
                i = tofind.len;
                part = getpart(file, part, i + 1);
            }
        }

        ежели (глашатайpart.content == '\0')
            flag = 1;
    }

    высвободить(part.content);
    высвободить(indexes);
}


неписанный_хлопец_целый_малый main() {
    строченька tofind, inp;
    байты_битыглашатай file;

    file = узнать_биты("in.txt", "r");

    tofind = getinput(file);

    boyer_moore(file, tofind, inp);

    забыть_биты(file);

    высвободить(tofind.content);

	покласть 0;
}