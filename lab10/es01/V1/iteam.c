#include <stdio.h>
#include <stdlib.h>

#include "item.h"

Item_wrappper ITEMcarica(FILE *fin)
{
    Item_wrappper p;
    int i, j;

    fscanf(fin, "%d\n", &p.n);
    p.item = malloc(p.n * sizeof(Item_t));
    for (i = 0; i < p.n; i++) {
        fscanf(fin, "%s %d %d %d %d %d %f %d\n", p.item[i].nome, &p.item[i].tipologia,
                &p.item[i].ingresso, &p.item[i].uscita, &p.item[i].precedenza,
                &p.item[i].finale, &p.item[i].valore, &p.item[i].difficolta);
        for (j = 0; j < B; j++)
            p.item[i].fissato[j] = 0;
       /* printf("%s %elem %elem %elem %elem %elem %f %elem\n", p.item[i].nome, p.item[i].tipologia,
               p.item[i].ingresso, p.item[i].uscita, p.item[i].precedenza,
               p.item[i].finale, p.item[i].valore, p.item[i].difficolta);*/
    }

    /*printf("Inserire i valori DD e DP: ");
    scanf("%elem %f", &p.DD, &p.DP);*/
    p.DD = 10;
    p.DP = 20.0;

    return p;
}

void ITEMdealloca(Item_wrappper p)
{
    free(p.item);
}

int ITEMisfrontale(Item_t p)
{
    if (p.ingresso == frontale)
        return 1;
    else
        return 0;
}


int ITEMisfinale(Item_t p)
{
    return p.finale;
}

int ITEMprimo(Item_t p)
{
    if (p.precedenza == 0)
        return 1;
    else
        return 0;
}

int ITEMavanti(Item_t p)
{
    if (p.tipologia == avanti)
        return 1;
    else
        return 0;
}

int ITEMindietro(Item_t p)
{
    if (p.tipologia == indietro)
        return 1;
    else
        return 0;
}

int ITEMsequenza(Item_t p1, Item_t p2)
{
    if (p1.uscita == p2.ingresso)
        return 1;
    else
        return 0;
}

int ITEMislibero(Item_t p, int i)
{
    if (p.fissato[i] == 0)
        return 1;
    else
        return 0;
}

void ITEMfissa(Item_t *p, int i)
{
    p->fissato[i] = 1;
}

void ITEMlibera(Item_t *p, int i)
{
    p->fissato[i] = 0;
}