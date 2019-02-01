#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"
#include "st.h"

#define C 2

typedef struct {
    char str[N];
} Item_t;

struct symboltable {
    int n;
    int m;
    Item_t *t;
};

ST STinit() {
    ST st = malloc(sizeof(ST));
    st->n = 0;
    st->m = 2;
    st->t = malloc(st->m* sizeof(Item_t));

    return st;
}

void STfree(ST st) {
    free(st->t);
    free(st);
}

int STcount(ST st) {
    return st->n;
}

int STsearch(ST st, char *label) {
    int i;

    for (i = 0; i < st->n; i++)
        if(strcmp(st->t[i].str, label) == 0)
            return i;

    return -1;
}

void STinsert(ST st, char *label) {
    int i = STsearch(st, label);
    if (i != -1)
        return;

    // se raggiungiamo il limite di posizioni allocate roddoppiamo le posizioni
    if (st->n == st->m) {
        st->m *= C;
        st->t = realloc(st->t, st->m*sizeof(Item_t));
    }

    strcpy(st->t[st->n].str, label);
    st->n++;
}

char *STsearchByIndex(ST st, int i) {
    return st->t[i].str;
}
