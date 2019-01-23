#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "st.h"

struct st_t {
    int n;
    int m;
    char **item;
};

ST STinit(int n) {
  ST st = (ST)malloc(sizeof(ST));
  st->n = n;
  st->m = 0;
  st->item = malloc(n * sizeof(char *));
  int i;
  for (i = 0; i < n; i++)
      st->item[i] = malloc(sizeof(char));

  return st;
};

void STinsert(ST st, char *name) {
    if (st->n == st->m) {
        printf("Impossibile inserire altri elementi\n");
        return;
    }

    strcpy(st->item[st->m], name);
    st->m++;
}

int STsearchbyindex(ST st, char *name) {
    int i;
    for (i = 0; i < st->m; i++)
        if (strcmp(name, st->item[i]) == 0)
            return i;

    return -1;
}