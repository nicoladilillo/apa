#include <stdio.h>
#include <stdlib.h>

#define E -1

typedef struct {
    char colore;
    int valore;
} Val_t;

typedef struct {
    Val_t val1, val2; // val1 è per righe, val2 è per colonne
    int usato;
} cella;

typedef struct {
    cella *c;
    int n; // numero celle
} cella_t; // cella wrapper

typedef struct {
    cella *c; // cella puntata nella schacchiera
    int ruotato; // se la cella è ruotata
    int fissato;
} board;

typedef struct {
    board **b;
    int r, c;
} board_t;

void apri_file(FILE **f, char *name)
{
    *f = NULL;
    *f = fopen(name, "r");

    if (*f == NULL) {
        printf("Errore apertura file!\n");
        exit(1);
    }
}

void leggi_Tessere(cella_t *p, char *str)
{
    FILE *fin;
    apri_file(&fin, str);

    fscanf(fin, "%d\n", &p->n);
    p->c = malloc(p->n * sizeof(cella));

    int i;
    for (i = 0; i < p->n; i++) {
        fscanf(fin, "%c %d %c %d\n", &p->c[i].val1.colore, &p->c[i].val1.valore,
               &p->c[i].val2.colore, &p->c[i].val2.valore);
        p->c->usato = 0;
    }
}

int leggi_Board(board_t *p, cella *c, char *str)
{
    FILE *fin;
    apri_file(&fin, str);

    fscanf(fin, "%d %d", &p->r, &p->c);

    int i, j, t, r, cnt = 0;
    p->b = malloc(p->r * sizeof(board));
    for (i = 0; i < p->r; i++) {
        p->b[i] = malloc(p->c * sizeof(board));
        for (j = 0; j < p->c; j++) {
            fscanf(fin, "%d/%d", &t, &r);
            if (t != E || r != E) {
                p->b[i][j].c = &c[t];
                p->b[i][j].ruotato = r;
                p->b[i][j].fissato = 1; // diciamo che è un valore fissato
                c[t].usato = 1;
                cnt++;
            }
        }

    }

    return cnt;
}

Val_t ruota(cella *c, int r, int n)
{
    if (n == 1) {
        // se servono valori riga
        if (r)
            return c->val2;
        else
            return c->val1;
    } else if (n == 2) {
        // se servono valori colonna
        if (r)
            return c->val1;
        else
            return c->val2;
    }
}

void check(board_t b, board **f, int *s)
{
    int i, j, c;
    Val_t v[2];
    int somma;
    int flag;
    int tot = 0;

    // controllo per righe
    c = 1;
    for (i = 0; i < b.r; i++) {
        v[0] = ruota(b.b[i][0].c, b.b[i][0].ruotato, c);
        somma = v[0].valore;
        flag = 1;
        for (j = 1; flag && j < b.c; j++) {
            // le celle in una riga devono avere tutte lo stesso colore
            v[1] = ruota(b.b[i][j].c, b.b[i][j].ruotato, c);
            if (v[1].colore == v[0].colore) {
                somma += v[1].valore;
                v[0] = v[1];
            } else {
                somma = 0;
                flag = 0;
            }
        }
        tot += somma;
    }

    // controllo per colonne
    c = 2;
    for (i = 0; i < b.c; i++) {
        v[0] = ruota(b.b[0][i].c, b.b[0][i].ruotato, c);
        somma = v[0].valore;
        flag = 1;
        for (j = 1; flag && j < b.r; j++) {
            v[1] = ruota(b.b[j][i].c, b.b[j][i].ruotato, c);
            // le celle in una colonna devono avere tutte lo stesso colore
            if (v[1].colore == v[0].colore) {
                somma += v[1].valore;
            } else {
                somma = 0;
                flag = 0;
            }
        }
        tot += somma;
    }

    // se la somma totale è maggiore della precedente allora scambia valori
    if (tot > *s)
    {
        *s = tot;
        for (i = 0; i < b.r; i++)
            for (j = 0; j < b.c; j++) {
                f[i][j].c = b.b[i][j].c;
                f[i][j].ruotato = b.b[i][j].ruotato;
            }
    }
}

void powerset(cella_t c, board_t b, int cnt, board **f, int *s)
{
    // se abbiamo analizzato tutte le celle controlliamo il valore ottenuto
    if (cnt >= c.n) {
        check(b, f, s);
        return;
    }

    int i, j, k;
    for (i = 0; i < c.n; i++) {
        // controllo se cella non è stata presa
        if (!c.c[i].usato) {
            c.c[i].usato = 1;
            for (j = 0; j < b.r; j++) {
                for (k = 0; k < b.c; k++) {
                    // controllo se cella nella board non è già stata occupata
                    if (!b.b[j][k].fissato) {
                        b.b[j][k].fissato = 1;

                        b.b[j][k].c = &c.c[i];
                        b.b[j][k].ruotato = 0; // prima non ruotato
                        powerset(c, b, cnt + 1, f, s);
                        b.b[j][k].ruotato = 1;
                        powerset(c, b, cnt + 1, f, s); // dopo ruotato

                        b.b[j][k].fissato = 0;
                    }
                }
            }
            c.c[i].usato = 0;

        }
    }
}

void stampa_val(Val_t v1, Val_t v2)
{
    printf("%c-%c%d-%d\t", v1.colore, v2.colore, v2.valore, v1.valore);
}

void stampa(board **m, int r, int c, int s)
{
    int i, j;

    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++) {
            if (m[i][j].ruotato)
                stampa_val(m[i][j].c->val2, m[i][j].c->val1);
            else
                stampa_val(m[i][j].c->val1, m[i][j].c->val2);
        }
        printf("\n");
    }

    // printf("Somma totale %d\n", s);
}

void wrapper_powerset(cella_t c, board_t b, int n)
{
    // creazione matrice finale
    board **finale = malloc(b.r * sizeof(board));
    int i;
    for (i = 0; i < b.r; i++)
        finale[i] = malloc(b.c * sizeof(board));
    int somme = 0;

    powerset(c, b, n, finale, &somme);

    stampa(finale, b.r, b.c, somme);

}

int main(int argc, char *argv[]) {

    cella_t celle;
    board_t boards;

    leggi_Tessere(&celle, argv[1]);
    int n = leggi_Board(&boards, celle.c, argv[2]);

    wrapper_powerset(celle, boards, n);

    return 0;
}