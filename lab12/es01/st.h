
#ifndef ES01_ST_H
#define ES01_ST_H

typedef struct st_t* ST;

ST STinit(int n);
void STinsert(ST st, char *name);
int STgetindex(ST st, char *name);
char *STselect(ST st, int i);

#endif //ES01_ST_H
