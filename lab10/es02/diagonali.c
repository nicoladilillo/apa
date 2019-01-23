#include <stdlib.h>

#include "diagonali.h"

diagonale_t *DIAGONALIcrea(int n)
{
    diagonale_t *d = malloc(n* sizeof(diagonale_t));
    return d;
}

void DIAGONALIdistruggi(diagonale_t *d)
{
    free(d);
}