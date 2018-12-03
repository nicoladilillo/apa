#ifndef ES03_PERSONAGGI_H
#define ES03_PERSONAGGI_H

#include "oggetti.h"

#define C 6+1 // 6 caratteri più tappo, per codice

#define PRI 'P' // primo carattere codice
#define SEC 'G' // secondo carattere codice


/* oggi equipaggiati ad ogni personaggio [max 8] */
typedef struct {
    int inUso;
    Inv_t *vettEq[O]; // vettore di puntatori a oggetti in inventario
} tabEquip_t;

/* struttura per ogni personaggio */
typedef struct {
    stat_t stat;
    char codice[C];
    char nome[N];
    char classe[N];
    tabEquip_t equip; // oggetti equipaggiati
} Pg_t;

/* variabile di tipo puntatotr a nodePg_t */
typedef struct nodePg_t *link;

/* struttura wrapper per lista personaggi */
typedef struct {
    link headPg; // puntatore alla testa della lista
    link tailPg; // punattore alla coda della lista
    int nPg; // numero personaggi in lista
} tabPg_t;

/* struttura ti tipo nodo */
struct nodePg_t {
    Pg_t p;
    link next;
};

void funzione_aggiungi_o(tabPg_t *tabPg, tabInv_t *tabInv);
void funzione_calcola_statistiche(tabPg_t *tabPg);
link ricerca(tabPg_t *tabPg);
int controllo_codice(char *cod, link x);
int leggi_personaggio(Pg_t *p, FILE *f);
void funzione_carica_p(tabPg_t *tabPg);
void funzione_aggiungi_p(tabPg_t *tabPg);
void funzione_elimina_p(tabPg_t *tabPg);
void funzione_elimina_o(tabPg_t *tabPg);
void stampa_personaggio(Pg_t p, stat_t s);
void somma(stat_t *s, stat_t v);
int v(int x);
void check(stat_t *s);
void funzione_ricerca_p(tabPg_t *tabPg);
void funzione_ricerca_p_statistiche(tabPg_t *tabPg);

#endif //ES03_PERSONAGGI_H
