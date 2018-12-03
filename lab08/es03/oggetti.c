#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <memory.h>

#include "oggetti.h"


void errore_comando(char *str)
{
    printf("Errore %s!\n", str);
}


int apri_file(FILE **fp)
{
    char nome[L];

    printf("Inserire nome file: ");
    scanf("%s", nome);

    *fp = NULL;
    *fp = fopen(nome, "r");
    if (*fp == NULL) {
        errore_comando("apertura file");
        return 0;
    } else
        return 1;
}

/**
 * CARICA OGGETTI
 */

/* letttura nuovo oggetto */
Inv_t nuovo_oggetto(FILE *f)
{
    Inv_t t;
    fscanf(f, "%s %s %d %d %d %d %d %d", t.nome, t.tipo, &t.stat.hp, &t.stat.mp, &t.stat.atk,
           &t.stat.def, &t.stat.mag, &t.stat.spr);

    return t;
}

void funzione_carica_o(tabInv_t *tabInv)
{
    FILE *fin ;

    /* verifica se presente file da leggere*/
    if (!apri_file(&fin))
        return;

    /* lettura numero personaggi */
    fscanf(fin, "%d", &tabInv->nInv);

    tabInv->vettInv = malloc(tabInv->nInv* sizeof(Inv_t));

    int i;
    for (i = 0; i < tabInv->nInv; i++)
        /* acquisizione nuovo oggetto */
        tabInv->vettInv[i] = nuovo_oggetto(fin);

}

/** AGGIUNGI O ELIMINA OGGETTI */

void stampa_oggetti(Inv_t *o, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d) %s %s %d %d %d %d %d %d\n", i+1, o[i].nome, o[i].tipo, o[i].stat.hp, o[i].stat.mp,
               o[i].stat.atk, o[i].stat.def, o[i].stat.mag, o[i].stat.spr);
}

void stampa_oggetti_puntati(Inv_t **o, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d) %s %s %d %d %d %d %d %d\n", i+1, o[i]->nome, o[i]->tipo, o[i]->stat.hp, o[i]->stat.mp,
               o[i]->stat.atk, o[i]->stat.def, o[i]->stat.mag, o[i]->stat.spr);
}


/** RICERCA OGGETTO */

void ricerca_oggetto(tabInv_t *tabInv)
{
    int i;
    char nome[L];

    printf("Inserisci nome: ");
    scanf("%s", nome);
    for (i = 0; i < tabInv->nInv; i++) {
        if (strcmp(nome, tabInv->vettInv[i].nome) == 0)
            break; // uscita non strutturata
    }

    if (i > tabInv->nInv)
        printf("Oggetto non trovato\n");
    else
        stampa_oggetti(&tabInv->vettInv[i], 1);
}
