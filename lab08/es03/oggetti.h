#ifndef ES03_OGGETTI_H
#define ES03_OGGETTI_H

#define O 8 // massimo numero di oggetti che si possono equipaggiare
#define N 50+1 // 50 caratteri più tappo
#define L 20+1 // per comandi generali

/* settupla di statische personaggio e oggetti */
typedef struct {
    unsigned hp, mp, atk, def, mag, spr;
} stat_t;

/* struttura per ogni oggetto dell'inventario */
typedef struct {
    char nome[N];
    char tipo[N];
    stat_t stat;
} Inv_t;

/* struttura wrapper per oggetti inventario */
typedef struct {
    Inv_t *vettInv;
    int nInv;
    int maxInv;
} tabInv_t;

int apri_file(FILE **fp);
void funzione_carica_o(tabInv_t *tabInv);
void stampa_oggetti(Inv_t *o, int n);
Inv_t nuovo_oggetto(FILE *f);
void stampa_oggetti_puntati(Inv_t **o, int n);
void errore_comando(char *str);
void ricerca_oggetto(tabInv_t *tabInv);

#endif //ES03_OGGETTI_H
