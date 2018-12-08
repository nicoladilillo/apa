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

void swap(att *v, int i, int j)
{
    att tmp;
    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
    return;
}

int partition(att *v, int r, int l)
{
    int q = r;
    int i , j;
    i = l;
    j = r-1;

    for( ; ; ) {
        while (v[i].s < v[q].s || (v[i].s == v[q].s && v[i].f < v[q].f)) {
            i++;
            if (i == r) break;
        }

        while (v[j].s > v[q].s || (v[j].s == v[q].s && v[j].f > v[q].f)) {
            j--;
            if (j == l) break;
        }

        if (i >= j) break;
        swap(v, i, j);
    }
    q = i;
    swap(v, q, r);

    return q;
}

void quicksort(att *v, int r, int l)
{
    if (r <= l)
        return;

    int q = partition(v, r, l);
    quicksort(v, r, q+1);
    quicksort(v, q-1, l);
    return;
}


void stampa_lavori(att *v, int N)
{
    int i;
    for (i = 0; i < N; i++)
        printf("%d %d\n", v[i].s, v[i].f);
}

int main(int argc, char *argv[]) {

    int n;
    att *v = leggi_attivita(&n, argv[1]);
    quicksort(v, n-1, 0);
    stampa_lavori(v, n);
    attSel(n, v);

    return 0;
}