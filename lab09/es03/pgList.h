#ifndef ES03_PGLIST_H
#define ES03_PGLIST_H

#include "invArray.h"

typedef struct  tabPg_t*  TABPG;

/* variabile di tipo puntatotr a nodePg_t */
typedef struct nodePg_t *link;

TABPG costruttore_tabPg();
void distruttore_tabPg(TABPG p);
void funzione_carica_p(TABPG tabPg);
void funzione_aggiungi_p(TABPG tabPg);
void funzione_elimina_p(TABPG tabPg);
void funzione_aggiungi_o(TABPG tabPg, TABINV tabInv);
void funzione_calcola_statistiche(TABPG tabPg, TABINV tabInv);
void funzione_elimina_o(TABPG tabPg, TABINV tabInv);
void funzione_ricerca_p_statistiche(TABPG tabPg, TABINV tabInv);
void funzione_ricerca_p(TABPG tabPg);

#endif //ES03_PGLIST_H
