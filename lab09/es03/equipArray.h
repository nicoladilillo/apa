#ifndef ES03_EQUIPARRAY_H
#define ES03_EQUIPARRAY_H

#include "inv.h"
#include "invArray.h"

typedef struct invArray_t *INVARRAY;

INVARRAY costruttore_equip(void);
void distruttore_equip(INVARRAY p);
int print_equipArray_inUso(INVARRAY equip);
void assegnazione_oggetto(INVARRAY equip, int n);
void eliminazione_oggetto(INVARRAY equip, int n);
void print_equipArray(INVARRAY equip, TABINV tabInv);
int print_equip_indice(INVARRAY equip, int i);

#endif //ES03_EQUIPARRAY_H
