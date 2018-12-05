#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int s, f
} att;

int check(att *v, att *s, int n, int *o, int *cnt, int *durata)
{
    int i, d = 0;
    att tmp;
    int c = 0;

    for (i = 0; i < n; i++) {
        if (o[i] == 1) {
            if (c == 0) {
                tmp = v[i];
                c++;
            } else {
                // negazione condizione di intersezione dettata nell'esercizio
                if (!((tmp.s < v[i].f) && (v[i].s < tmp.f))) {
                    tmp = v[i];
                    c++;
                } else {
                    return 0;
                }
            }
            d += v[i].f - v[i].s;
        }
    }

    // se soluzione trovata è meglio della precedente
    if (d > *durata) {
        *durata = d;
        c = 0;
        for (i = 0; i < n; i++)
        {
            if (o[i] == 1)
                s[c++] = v[i];
        }
        *cnt = c;
    }

    return 1;
}

void powerset(att *v, att *s, int n, int start, int *cnt, int *d, int *occ)
{
    if (start >= n) return; // terminazione ricorsione

    int i;
    for (i = start; i < n; i++) {
        // prima con
        occ[i] = 0;
        if(check(v, s, n, occ, cnt, d))
            powerset(v, s, n, i+1, cnt, d, occ);

        // dopo senza
        occ[i] = 1;
        if(check(v, s, n, occ, cnt, d))
            powerset(v, s, n, i+1, cnt, d, occ);

        occ[i] = 0;
    }
}

void stampa(att *c, int n, int d)
{
    int i;
    for (i = 0; i < n; i++)
        printf("(%d, %d)", c[i].s, c[i].f);

    printf("\nDurata: %d\n\n", d);
}

void attSel(int N, att *v) {
    att sol[N];
    int durata = 0;
    int occ[N];
    int cnt = 0;

    powerset(v, sol, N, 0, &cnt, &durata, occ);

    stampa(sol, cnt, durata);
}

/* leggi valori iniziali */
att *leggi_attivita(int *n, char *nome)
{
    FILE *fp = fopen(nome, "r");

    fscanf(fp, "%d\n", n);
    att *v = malloc(*n * sizeof(att));

    int i;
    for (i = 0; i < *n; i++)
        fscanf(fp, "%d %d\n", &v[i].s, &v[i].f);

    return v;
}

int main(int argc, char *argv[]) {

    int n;
    att *v = leggi_attivita(&n, argv[1]);
    attSel(n, v);

    return 0;
}