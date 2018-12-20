#ifndef ES03_INVARRAY_H
#define ES03_INVARRAY_H

typedef struct tabInv_t *TABINV;

TABINV costruttore_tabInv();
TABINV distruttore_tabInv(TABINV p);
void funzione_carica_o(TABINV tabInv);
void ricerca_oggetto(TABINV tabInv);
int print_tabInv_nInv(TABINV tabInv);
void print_tabInv(TABINV tabInv);
int print_tabInv_hp(TABINV tabinv, int i);
int print_tabInv_mp(TABINV tabinv, int i);
int print_tabInv_atk(TABINV tabinv, int i);
int print_tabInv_def(TABINV tabinv, int i);
int print_tabInv_mag(TABINV tabinv, int i);
int print_tabInv_spr(TABINV tabinv, int i);
void print_tabInv_oggetto(TABINV tabinv, int i);

#endif //ES03_INVARRAY_H
