#include <stdlib.h>

#include "diagonali.h"

diagonale_t *DIAGONALIcrea(int n)
{
    diagonale_t *d = malloc(n* sizeof(diagonale_t));
    int i;
    for (i = 0; i < n; i++) {
        d[i].n = d[i].difficolta_tot = 0;
        d[i].valore_tot = 0.0;
    }

    return d;
}

void DIAGONALIdistruggi(diagonale_t *d)
{
    free(d);
}