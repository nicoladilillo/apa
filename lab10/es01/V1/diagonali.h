#ifndef ES01_DIAGONALI_H
#define ES01_DIAGONALI_H

#include "item.h"

typedef struct {
    int n;
    int difficolta_tot;
    float valore_tot;
    int elem[D];
} diagonale_t;


diagonale_t *DIAGONALIcrea(int n);
void DIAGONALIdistruggi(diagonale_t *d);

#endif //ES01_DIAGONALI_H
