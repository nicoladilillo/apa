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

void stampa(int n, int *mark)
{
    int i;

    printf("{ ");
    for (i = 0; i < n; i++) {
        /*
         * stampa solo valori controllati che hanno
         * superato check
         */
        if (mark[i] == 1)
            printf("%d ", i);
    }
    printf("}\n");
}

int check(int n,int *m, int e, Arco *a)
{
    int i, j, cnt = 0;

    /*
     * creiamo un vettore per tutti gli archi che impostiamo
     * come zero per tutti per indicare che l'arco non è coperto
     */
    int v[e];
    for (i = 0; i < n; i++)
        v[i] = 0;

    /*
     * controlla se i vertici considerati comprendono tutti gli archi
     */
    for (i = 0; i < n; i++) {
        /*
         * condizione vertice considerato
         */
        if (m[i] == 1) {
            for (j = 0; j < e; j++) {
                /*
                 * confronto se vertice considerato è
                 * presente in un arco
                 */
                if (a[j].x == i || a[j].y == i)
                    /*
                     * settiamo quell'arco come coperto
                     */
                    v[j] = 1;
            }
        }
    }

    /*
     * contiamo quanti archi abbiamo coperto
     */
    for (i = 0; i < e; i++)
        if (v[i] == 1)
            cnt++;

    return (cnt == e);
}

void powerset(int n, int e, Arco *a, int *mark, int start)
{
    int i;

    // terminazione
    if (start >= n) {
        /*
         * controllo se trovato vertex cover, devo avere almeno due vertici
         */
        if (check(n, mark, e, a)) {
            stampa(n, mark);
        }
        return;
    }

    /*
     * passo tutte le possibili soluzioni di vertici e il
     * numero di vertici controllati, alternativa al for
     */
    mark[start] = 0;
    powerset(n, e, a, mark, start+1);
    mark[start] = 1;
    powerset(n, e, a, mark, start+1);

    return;
}

void powerset_wrapper(int N, int e, Arco *a)
{
    int *mark = (int *)malloc(N*sizeof(int));
    powerset(N, e, a, mark, 0);
    free(mark);
}

int main(int argc, char *argv[]) {

    int N, E; // numero di vertici e di archi
    Arco *a;

    leggi_file(&N, &E, &a, argv[1]);
    powerset_wrapper(N, E, a);
    free(a);

    return 0;
}