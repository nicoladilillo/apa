#include <stdio.h>
#include <stdlib.h>
#include <printf.h>

#define N 100
#define C 255
#define FIL "brani.txt"

typedef struct {
    int n;
    char c[N][C];
} Ragazzo;

int apri_file(Ragazzo v[], char nome[], int g[])
{
    int n, i, j;

    FILE *fp = NULL;
    fp = fopen(nome, "r");
    if (fp == NULL)
    {
        printf("Errore apertura file\n");
        exit(-1);
    }

    /*
     * numero ragazzi
     */
    fscanf(fp, "%d", &n);

    for (i = 0; i < n; i++) {
        g[i] = 0;
        /*
         * utilizziamo la variabile app per semplificare il codice
         */
        fscanf(fp, "%d", &v[i].n);
        for (j = 0; j < v[i].n; j++)
            fscanf(fp, "%s", v[i].c[j]);
    }

    return n;
}

void stampa(Ragazzo r[], int n, int p[])
{
    int i;

    for (i = 0; i < n; i++)
        printf("%s ", r[i].c[p[i]]);
    printf("\n");
}

int esplorasoluzioni(Ragazzo r[], int n, int start, int v[], int c)
{
    int i;

    if (start == n) {
        stampa(r, n, v);
        return c+1;
    } else {
        for (i = 0; i < r[start].n; i++) {
            v[start] = i;
            c = esplorasoluzioni(r, n, start + 1, v, c);
        }
    }

    return c;
}

int main(void) {

    Ragazzo r[N];
    int v[N];

    int n = apri_file(r, FIL, v);

    int cnt = esplorasoluzioni(r, n, 0, v, 0);

    printf("Numero playlist possibili: %d", cnt);

    return 0;
}