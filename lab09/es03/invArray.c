#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "inv.h"
#include "invArray.h"

/* struttura wrapper per oggetti inventario */
struct tabInv_t {
    Inv_t *vettInv;
    int nInv;
    int maxInv; // massimo numero oggetti equipaggiabili
};

/* alloca */
TABINV costruttore_tabInv()
{
    TABINV p = malloc(sizeof *p);
    return p;
}

/* dealloca */
TABINV distruttore_tabInv(TABINV p)
{
    free(p->vettInv);
    free(p);
}

/* carica oggetti */
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

/* ricerca un oggetto per nome */
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

/* restituisce il valore del numero di oggetti nel vettore */
int print_tabInv_nInv(TABINV tabInv)
{
    return tabInv->nInv;
}

/* stampa tutti gli oggetti nel vettore */
void print_tabInv(TABINV tabInv)
{
    stampa_oggetti(tabInv->vettInv, tabInv->nInv);
}

/* stampa un singolo oggetto */
void print_tabInv_oggetto(TABINV tabinv, int i)
{
    stampa_oggetto(tabinv->vettInv[i]);
}

/*
 * queste funzioni restituiscono tutti
 * i campi della variabile statisrica
 */
int print_tabInv_hp(TABINV tabinv, int i)
{
    return tabinv->vettInv[i].stat.hp;
}

int print_tabInv_mp(TABINV tabinv, int i)
{
    return tabinv->vettInv[i].stat.mp;
}

int print_tabInv_atk(TABINV tabinv, int i)
{
    return tabinv->vettInv[i].stat.atk;
}

int print_tabInv_def(TABINV tabinv, int i)
{
    return tabinv->vettInv[i].stat.def;
}

int print_tabInv_mag(TABINV tabinv, int i)
{
    return tabinv->vettInv[i].stat.mag;
}

int print_tabInv_spr(TABINV tabinv, int i)
{
    return tabinv->vettInv[i].stat.spr;
}
