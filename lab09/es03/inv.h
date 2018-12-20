#ifndef ES03_OGGETTI_H
#define ES03_OGGETTI_H

#include "utility.h"

/* struttura per ogni oggetto dell'inventario */
typedef struct {
    char nome[N];
    char tipo[N];
    stat_t stat;
} Inv_t;

void stampa_oggetti(Inv_t *o, int n);
Inv_t nuovo_oggetto(FILE *f);
void stampa_oggetto(Inv_t o);


#endif //ES03_OGGETTI_H
