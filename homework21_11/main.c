#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <malloc.h>


typedef struct linkedlist{
    int32_t value;
    int32_t key;
    struct linkedlist* next;
}linkedlist;


int32_t addelement(linkedlist** start, int32_t value, int32_t key) {
    linkedlist *res, *cur;

    res = (linkedlist*)malloc(sizeof(linkedlist));

    res->value = value;
    res->key = key;
    res->next = NULL;

    cur = *start;

    if(cur == NULL) {
        *start = res;
        return 0;
    }

    while(cur->next != NULL) {
        cur = cur->next;
    }

    cur->next = res;

    return 0;
}


void freelist(linkedlist** start) {
    linkedlist *cur, *toremove;

    cur = *start;

    while (cur->next != NULL){
        toremove = cur;
        cur = cur->next;
        free(toremove);
    }

    free(cur);
    free(start);
}

void printlist(linkedlist** start) {
    linkedlist* cur;

    cur = *start;

    while(cur->next != NULL) {
        printf("Key: %d ", cur->key);
        printf("Value: %d\n", cur->value);
        cur = cur->next;
    }

    printf("Key: %d ", cur->key);
    printf("Value: %d\n", cur->value);

}


int32_t findinlist(linkedlist** start, int32_t key) {
    linkedlist* cur = *start;
    while (cur->next != NULL) {
        if(cur->key == key)
            return cur->value;

        cur = cur->next;
    }

    return -1;
}


int32_t getlistlen(linkedlist** start) {
    linkedlist* cur = *start;
    int32_t res;

    res = 0;

    if (cur == NULL){
        return 0;
    }

    while (cur -> next != NULL){
        res++;
        cur = cur->next;
    }

    return res + 1;
}


int32_t removeelement(linkedlist** start, linkedlist* element) {
    linkedlist* cur;

    cur = *start;

    if(cur == element) {
        *start = cur->next;
        free(cur);
        return 0;
    }

    while (cur->next != element && cur->next != NULL)
        cur = cur->next;

    if (cur->next == NULL)
        return 1;

    cur->next = element->next;

    free(element);
}


void removeeven(linkedlist** start){
    linkedlist* cur;

    cur = *start;

    while(cur->next != NULL) {
        if (cur->key % 2 == 0){
            removeelement(start, cur);
            cur = *start;
        }
        cur = cur->next;
    }

    if (cur->key % 2 == 0)
        removeelement(start, cur);

}


int32_t iscycled(linkedlist** start) {
    linkedlist *p1, *p2;

    p1 = *start;
    p2 = (*start)->next;

    while(p1 != NULL && p2 != NULL && p1 != p2) {
        p1 = p1->next;
        p2 = p2->next->next;
    }

    if(p1 == p2)
        return 1;

    return 0;
}


linkedlist** combine(linkedlist** start1, linkedlist** start2) {
    linkedlist** res;
    linkedlist *cur1, *cur2;

    res = (linkedlist**)malloc(sizeof(linkedlist*));
    *res = NULL;

    cur1 = *start1;
    cur2 = *start2;

    while (cur1 != NULL && cur2 != NULL) {
        if (cur1->value >= cur2->value) {
            addelement(res, cur2->value, cur2->key);
            cur2 = cur2->next;
        }
        else {
            addelement(res, cur1->value, cur1->key);
            cur1 = cur1->next;
        }
    }

    if (cur1 != NULL) {
        while (cur1 != NULL) {
            addelement(res, cur1->value, cur1->key);
            cur1 = cur1->next;
        }
    }
    else{
        while (cur2 != NULL) {
            addelement(res, cur2->value, cur2->key);
            cur2 = cur2->next;
        }
    }

    return res;
}


int32_t main() {
    linkedlist** list;
    linkedlist** list2;
    linkedlist** combinedlist;

    list = (linkedlist**)malloc(sizeof(linkedlist*));
    *list = NULL;
    list2 = (linkedlist**)malloc(sizeof(linkedlist*));
    *list2 = NULL;

    addelement(list, 1, 30);
    addelement(list, 3, 20);
    addelement(list, 5, 3);
    addelement(list2, 2, 1);
    addelement(list2, 4, 2);
    addelement(list2, 6, 3);

    combinedlist = combine(list, list2);

    printlist(combinedlist);

    freelist(list);
    freelist(list2);
    freelist(combinedlist);

    return 0;
}
