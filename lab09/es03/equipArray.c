#include <stdio.h>
#include <stdlib.h>

#include "equipArray.h"

#define O 8 // massimo numero di oggetti che si possono equipaggiare

/* ogni equipaggiati ad ogni personaggio [max 8] */
struct invArray_t {
    int inUso;
    int vettEq[O]; // vettore di indici
};

INVARRAY costruttore_equip(void)
{
    INVARRAY p = malloc(sizeof *p);
    p->inUso = 0;
    return p;
}

void distruttore_equip(INVARRAY p)
{
    free(p);
}

int print_equipArray_inUso(INVARRAY equip)
{
    return equip->inUso;
}

void assegnazione_oggetto(INVARRAY equip, int n)
{
    equip->vettEq[equip->inUso] = n;
    equip->inUso++;
}

void print_equipArray(INVARRAY equip, TABINV tabInv)
{
    int i;
    for (i = 0; i < equip->inUso; i++)
    {
        printf("%d) ", i + 1);
        stampa(print_tabInv_vettInv(tabInv ,equip->vettEq[i]));
    }
}

void eliminazione_oggetto(INVARRAY equip, int n)
{
    int i;
    for (i = n; i < equip->inUso-1; i++)
        equip->vettEq[n] = equip->vettEq[n+1];
    equip->inUso--;
}

int print_equip_indice(INVARRAY equip, int i)
{
    return equip->vettEq[i];
}