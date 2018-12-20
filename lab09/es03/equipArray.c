#include <stdio.h>
#include <stdlib.h>

#include "equipArray.h"

#define O 8 // massimo numero di oggetti che si possono equipaggiare

/* ogni equipaggiati ad ogni personaggio [max 8] */
struct invArray_t {
    int inUso;
    int vettEq[O]; // vettore di indici
};

/* alloca */
INVARRAY costruttore_equip(void)
{
    INVARRAY p = malloc(sizeof *p);
    p->inUso = 0;
    return p;
}

/* dealloca */
void distruttore_equip(INVARRAY p)
{
    free(p);
}

/* ritorna valore oggetti in uso */
int print_equipArray_inUso(INVARRAY equip)
{
    return equip->inUso;
}

/* assegna nuovo oggetto */
void assegnazione_oggetto(INVARRAY equip, TABINV tabInv)
{
    print_tabInv(tabInv);

    int n;
    printf("Inserire numero oggetto: ");
    scanf("%d", &n);
    if (n > print_tabInv_nInv(tabInv)) {
        printf("oggetto non presente\n");
        return;
    }

    /* assegnazione oggetto personaggio */
    equip->vettEq[equip->inUso] = n-1;
    equip->inUso++;
}

/* stampa tutti gli oggetti equipaggiati */
void print_equipArray(INVARRAY equip, TABINV tabInv)
{
    int i;
    for (i = 0; i < equip->inUso; i++)
    {
        printf("%d) ", i + 1);
        print_tabInv_oggetto(tabInv, equip->vettEq[i]);
    }
}

/* rimuovi oggetto */
void eliminazione_oggetto(INVARRAY equip, int n)
{
    int i;
    for (i = n; i < equip->inUso-1; i++)
        equip->vettEq[n] = equip->vettEq[n+1];
    equip->inUso--;
}

/* restutuisci l'indice di un oggetto nel vettore oggetti */
int print_equip_indice(INVARRAY equip, int i)
{
    return equip->vettEq[i];
}