//
// Created by nicola on 31/01/19.
//

#ifndef ESAME_ST_H
#define ESAME_ST_H

typedef struct symboltable *ST;

ST STinit();
void STfree(ST st);
int STcount(ST st);
int STsearch(ST st, char *label);
void STinsert(ST st, char *label);
char *STsearchByIndex(ST st, int i);

#endif //ESAME_ST_H
