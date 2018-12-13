#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "invArray.h"

/* struttura wrapper per oggetti inventario */
struct tabInv_t {
    Inv_t *vettInv;
    int nInv;
    int maxInv;
};

TABINV costruttore_tabInv()
{
    TABINV p = malloc(sizeof *p);
    return p;
}

TABINV distruttore_tabInv(TABINV p)
{
    free(p->vettInv);
    free(p);
}

void funzione_carica_o(TABINV tabInv)
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

void ricerca_oggetto(TABINV tabInv)
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

int print_tabInv_nInv(TABINV tabInv)
{
    return tabInv->nInv;
}

void print_tabInv(TABINV tabInv)
{
    stampa_oggetti(tabInv->vettInv, tabInv->nInv);
}

Inv_t print_tabInv_vettInv(TABINV tabInv, int i)
{
    return tabInv->vettInv[i];
}

stat_t print_tabInv_vettInv_stat(TABINV tabInv, int i)
{
    return tabInv->vettInv[i].stat;
}