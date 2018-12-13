#ifndef ES03_INVARRAY_H
#define ES03_INVARRAY_H

#include "inv.h"

typedef struct tabInv_t *TABINV;

TABINV costruttore_tabInv();
TABINV distruttore_tabInv(TABINV p);
void funzione_carica_o(TABINV tabInv);
void ricerca_oggetto(TABINV tabInv);
int print_tabInv_nInv(TABINV tabInv);
void print_tabInv(TABINV tabInv);
Inv_t print_tabInv_vettInv(TABINV tabInv, int i);
stat_t print_tabInv_vettInv_stat(TABINV tabInv, int i);

#endif //ES03_INVARRAY_H
