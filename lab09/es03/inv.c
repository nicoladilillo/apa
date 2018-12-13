#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "utility.h"
#include "inv.h"

/* letttura nuovo oggetto */
Inv_t nuovo_oggetto(FILE *f)
{
    Inv_t t;
    fscanf(f, "%s %s %d %d %d %d %d %d", t.nome, t.tipo, &t.stat.hp, &t.stat.mp, &t.stat.atk,
           &t.stat.def, &t.stat.mag, &t.stat.spr);

    return t;
}

void stampa_oggetti(Inv_t *o, int n)
{
    int i;
    for (i = 0; i < n; i++) {
        printf("%d) ", i + 1);
        stampa(o[i]);
    }
}

void stampa(Inv_t o)
{
    printf("%s %s %d %d %d %d %d %d\n", o.nome, o.tipo, o.stat.hp, o.stat.mp,
           o.stat.atk, o.stat.def, o.stat.mag, o.stat.spr);
}
