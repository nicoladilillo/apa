#ifndef ES03_ST_H
#define ES03_ST_H

#include <stdio.h>
#include <stdlib.h>

#define D 30 // lunghezza massima stringhe item

typedef struct symboltable* ST;

ST STinit();
int STget(ST st);
int STcheck(ST st, char *nome, char *rete);
int STsearch(ST st, char *str);
char *STprint(ST st, int i);

#endif //ES03_ST_H
