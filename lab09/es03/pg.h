#ifndef ES03_PERSONAGGI_H
#define ES03_PERSONAGGI_H

#include <stdio.h>

#include "utility.h"
#include "equipArray.h"

#define C 6+1 // 6 caratteri più tappo, per codice

#define PRI 'P' // primo carattere codice
#define SEC 'G' // secondo carattere codice

/* struttura per ogni personaggio */
typedef struct {
    stat_t stat;
    char codice[C];
    char nome[N];
    char classe[N];
    INVARRAY equip; // oggetti equipaggiati
} Pg_t;


int leggi_personaggio(Pg_t *p, FILE *f);
void stampa_personaggio(Pg_t p, stat_t s);
void somma(stat_t *s, stat_t v);
void check(stat_t *s);

#endif //ES03_PERSONAGGI_H
