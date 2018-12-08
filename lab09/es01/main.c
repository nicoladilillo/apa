#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int s, f
} att;

/* leggi valori iniziali */
att *leggi_attivita(int *n, char *nome)
{
    FILE *fp = fopen(nome, "r");

    fscanf(fp, "%d\n", n);
    att *v = calloc(*n+1, sizeof(att)); // elemento 0 = (0 , 0)
    v[0].s = v[0].f = 0;

    int i;
    for (i = 1; i <= *n; i++)
        fscanf(fp, "%d %d\n", &v[i].s, &v[i].f);

    return v;
}

/* scambio di due elementi vettore dipo att */
void swap(att *v, int i, int j)
{
    att tmp;
    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
    return;
}

/* partizionamento per quicksort */
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

/* ordinamento quicksort */
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
    for (i = 0; i <= N; i++)
        printf("%d %d\n", v[i].s, v[i].f);
}

int durata(att v)
{
    return v.f-v.s;
}

void stampa(att *v, int *indici, int i, int f)
{
    if (i == 0)
        return;

    stampa(v, indici, indici[i], f);
    printf("(%d %d)", v[i].s, v[i].f);
    if (i != f)
        printf(", ");
}

/* stampa soluzione */
void stampa_sol_dinamica(att *v, int *indici, int *tail, int cnt, int max) {
    int i;
    for (i = 0; i <= cnt; i++) {
        if (cnt > 0)
            printf("SOLUZIONE %d:\t", i + 1);
        stampa(v, indici, tail[i], tail[i]);
        printf(";\n");
    }

    printf("\nDurata massima: %d", max);
}

// controlla se lavori si interconnetono
int check(att v1, att v2)
{
    if ((v1.s < v2.f) && (v2.s < v1.f))
        return 0;
    else
        return 1;
}

void soluzione_dinamica(att *v, int N)
{
    int tail[N]; // punta ultimo elemento della LIS, ipotizziamo ci siano diverse soluzioni
    int *opt = calloc(N+1, sizeof(int)); // durate totali soluzioni parziali
    int *indici = calloc(N+1, sizeof(int)); // puntatori valori precedenti
    int cnt = 0;

    // valore distanza massima
    int ris = opt[1] = durata(v[1]);
    tail[cnt] = 1;

    int i, j;
    for (i = 2; i <= N; i++) {
        for (j = i-1; j > 0; j--)
        if(check(v[i], v[j]) && (opt[i] < durata(v[i]) + opt[j])) {
            opt[i] = durata(v[i]) + opt[j];
            indici[i] = j;

            if (opt[i] > ris) {
                // se si supera il massimo
                ris = opt[i];
                cnt = 0;
                tail[cnt] = i;
            } else if (opt[i] == ris) {
                // se ha stesso valore di tail
                tail[++cnt] = i;
            }
        }
    }

    stampa_sol_dinamica(v, indici, tail, cnt, ris);
}

int main(int argc, char *argv[]) {

    int n;
    att *v = leggi_attivita(&n, argv[1]);
    quicksort(v, n, 0);

    // stampa_lavori(v, n);

    soluzione_dinamica(v, n);

    return 0;
}