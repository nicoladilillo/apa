#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int x, y;
} Arco;

void open_file(FILE **f, char *n)
{
    *f = NULL;
    *f = fopen(n, "r");
    if (*f == NULL) {
        printf("Errore apertura file!\n");
        exit(1);
    }
}

void leggi_file(int *n, int *e, Arco **a, char *nome)
{
    FILE *fp;
    open_file(&fp, nome);

    fscanf(fp, "%d %d\n", n, e);
    *a = malloc(*e * sizeof(Arco));

    int i;
    for (i = 0; i < *e; i++)
        fscanf(fp, "%d %d\n", &(*a)[i].x, &(*a)[i].y);

    fclose(fp);
}

int check(int n,int *m, int e, Arco *a)
{
    int i, j, cnt = 0;

    int v[n];
    for (i = 0; i < n; i++)
        v[i] = 0;

    for (j = 0; j < n; j++) {
        /*
         * solo se nostro vertice è considereato
         */
        if (m[j] == 1) {
            v[j] = 1;
            /*
             * j è il vertice che sto considerando
             * i sono i vertici che guarda il vertice j
             */
            for (i = 0; i < e; i++) {
                if (a[i].x == j)
                    v[a[i].y] = 1;
                else if (a[i].y == j)
                    v[a[i].x] = 1;
            }
        }
    }

    for (i = 0; i < n; i++)
        if (v[i] == 1)
            cnt++;

    return cnt;
}

void powerset(int n, int e, Arco *a, int *mark, int start, int cnt)
{
    int i;

    // terminazione
    if (start >= n) {
        /*
         * controllo se trovato vertex cover, devo avere almeno due vertici
         */
        if (cnt >= 2 && check(n, mark, e, a) == n) {
            printf("{ ");
            for (i = 0; i < n; i++) {
                /*
                 * stampa solo valori controllati
                 */
                if (mark[i] == 1)
                    printf("%d ", i);
            }
            printf("}\n");
        }
        return;
    }

    /*
     * passo tutte le possibili soluzioni di vertici
     */
    mark[start] = 0;
    powerset(n, e, a, mark, start+1, cnt);
    mark[start] = 1;
    powerset(n, e, a, mark, start+1, cnt+1);

    return;
}

void powerset_wrapper(int N, int e, Arco *a)
{
    int *mark = (int *)malloc(N*sizeof(int));
    powerset(N, e, a, mark, 0, 0);
    free(mark);
}

int main(int argc, char *argv[]) {

    int N, E;
    Arco *a;

    leggi_file(&N, &E, &a, argv[1]);
    powerset_wrapper(N, E, a);
    return 0;
}