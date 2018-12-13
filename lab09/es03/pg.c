#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "pg.h"


int leggi_personaggio(Pg_t *p, FILE *f)
{
    if (fscanf(f, "%s %s %s %d %d %d %d %d %d", p->codice, p->nome, p->classe, &p->stat.hp, &p->stat.mp,
            &p->stat.atk, &p->stat.def, &p->stat.mag, &p->stat.spr) != EOF) {
        p->equip = costruttore_equip(); // azzera valore oggetti in uso
        return 1;
    }

    return 0;
}

/* stampa le caratterstiche di un personaggio */
void stampa_personaggio(Pg_t p, stat_t s)
{
    printf("%s di codice %s possiede di classe %s: ", p.nome, p.codice, p.classe);
    printf(" hp-> %d\t atk-> %d\t spr-> %d\t", s.hp, s.atk, s.spr);
    printf(" mp-> %d\t def-> %d\t mag-> %d\n", s.mp, s.def, s.mag);
}

/* somma totale statistiche */
void somma(stat_t *s, stat_t v)
{
    s->def += v.def;
    s->mp += v.mp;
    s->atk += v.atk;
    s->spr += v.spr;
    s->hp += v.hp;
    s->mag += v.mag;
}

/* se una statistica è minore di 0 assegna valore 1 */
int v(int x)
{
    if (x <= 0)
        return 1;
    else
        return x;
}

/* controlla eventuali valori minori di zero */
void check(stat_t *s)
{
    s->def = v(s->def);
    s->mp = v(s->mp);
    s->atk = v(s->atk);
    s->spr = v(s->spr);
    s->hp = v(s->hp);
    s->mag = v(s->mag);
}
