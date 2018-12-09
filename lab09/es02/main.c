#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OUTPUT "log.txt"

typedef enum {
    zaffiri, rubini, topazi, smeraldi, pietre
} Pietre;

typedef struct {
    int n;
    int f;
    int **occ;
} Lookup_t;

void apri_file(FILE **f, char *nome, char *c)
{
    *f = NULL;
    *f = fopen(nome, c);
    if (*f == NULL)
    {
        printf("Errore lettura file\n");
        exit(-1);
    }
}

/*
 * uno zaffiro deve essere seguito immediatamente o da un altro zaffiro o da un rubino
 * uno smeraldo deve essere seguito immediatamente o da un altro smeraldo o da un topazio
 * un rubino deve essere seguito immediatamente o da uno smeraldo o da un topazio
 * un topazio deve essere seguito immediatamente o da uno zaffiro o da un rubino.
 */
int promising(int p1, int p2)
{
    /*
    * valutiamo se la nostra combinazione mentre viene creata rispetti
    * le condizioni citate in testa alla funzione per costruire la collana
    */
    switch (p1) {
        case zaffiri:
            if (p2 != zaffiri && p2 != rubini)
                return 0;
            break;
        case smeraldi:
            if (p2 != smeraldi && p2 != topazi)
                return 0;
            break;
        case rubini:
            if (p2 != smeraldi && p2 != topazi)
                return 0;
            break;
        case topazi:
            if (p2 != zaffiri && p2 != rubini)
                return 0;
            break;
        default:
            exit(-2);
    }

    return 1;
}

int check(int *v1, Lookup_t v2, int n)
{
    int i, j, flag;
    for (i = 0; i < n; i++) {
        flag = 1;
        for (j = 0; flag && j < pietre; j++) {
            if (v1[j] != v2.occ[i][j])
                flag = 0;
        }

        if (flag == 1)
            return 1;
    }

    return 0;
}

/* restituisce lunghezza massima collana */
int powerset(int *occ, const int pos, const int max, int *cnt, Lookup_t **lookup, const int p)
{
    int i;

    if (*cnt < pos)
        *cnt = pos;

    if (*cnt >= max)
        return 1;


    /* controlloliamo se abbiamo già calcolato questa soluzione, se si ritorniamo */
    if (lookup[p][pos].f && check(occ, lookup[p][pos], lookup[p][pos].n))
        return 0;


    /* per avere una ripetizione su tutti i possibili valori da inserire */
    for (i = 0; i < pietre; i++) {
        // controlla se possibili inserire pietra con questo ordine
        if (occ[i] > 0 && promising(p, i)) {
            /* decrementiamo il numero di valori di una data pietra dal vettore delle occorrenze */
            occ[i]--;

            /* chiamata ricorsiva che termina completamente tutto se pos coincide con max*/
            if (powerset(occ, pos + 1, max, cnt, lookup, i)) return 1;

            /*incrementiamo nuovamente il valore di una data pietra per poterla
             * utilzzare in una prossima chiamata in una diversa posizione */
            occ[i]++;
        }
    }

    // memorizza soluzione
    lookup[p][pos].f = 1;
    int n = lookup[p][pos].n;
    for (i = 0; i < pietre; i++)
        lookup[p][pos].occ[n][i] = occ[i];
    lookup[p][pos].n++;

    return 0;
}

Lookup_t *azzera(int max)
{
    int i, j, k, m;
    Lookup_t **lookup;
    lookup = malloc(pietre* sizeof(Lookup_t));
    for (i = 0; i < pietre; i++) {
        lookup[i] = malloc(max*  sizeof(Lookup_t));
        for (j = 0; j < max; j++) {
            lookup[i][j].f = lookup[i][j].n = 0;
            lookup[i][j].occ = malloc(max*max * sizeof(int *));
            for (k = 0; k < max*max; k++) {
                lookup[i][j].occ[k] = malloc(pietre * sizeof(int));
                for (m = 0; m < pietre; m++)
                    lookup[i][j].occ[k][m] = -1;
            }
        }
    }

    return lookup;
}

void libera(Lookup_t **lookup, int max)
{
    int i, j, k;
    for (i = 0; i < pietre; i++) {
        for (j = 0; j < max; j++) {
            for (k = 0; k < max; k++) {
                free(lookup[i][j].occ[k]);
            }
            free(lookup[i][j].occ);
        }
        free(lookup[i]);
    }
    free(lookup);
}

void fX(int *v, int max, int p, int *cnt, Lookup_t *lookup)
{
    v[p]--;
    powerset(v, 1, max, cnt, lookup, p);
    v[p]++;
}

int trova_lunghezza_massima(int *v, int m)
{
    // alloca dinamicamente stringa controllo
    Lookup_t **lookup = azzera(m);
    int cnt = 0;
    int i;
    for (i = 0; i < pietre; i++)
        fX(v, m, i, &cnt, lookup);

    // libera(lookup, m);


    return cnt;
}

void informazioni(int *t, int max, FILE *f) {
    int i;

    for (i = 0; i < max; i++)
    {
        switch (i) {
            case zaffiri:
                fprintf(f, "zaffiri = ");
                break;
            case smeraldi:
                fprintf(f, "smeraldi = ");
                break;
            case rubini:
                fprintf(f, "rubini = ");
                break;
            case topazi:
                fprintf(f, "topazi = ");
                break;
            default: exit(-2);
        }
        fprintf(f, " %d, ", t[i]);
    }
}

int leggi_valori(FILE *in, int *p, FILE *out)
{
    int i;
    int max = 0;

    /* lettuta numero pietre */
    for (i = 0; i < pietre; i++) {
        fscanf(in, "%d ", &p[i]);
        max += p[i];
    }
    informazioni(p, pietre, out);
    fprintf(out, "TOT = %d\n", max);

    int cnt = trova_lunghezza_massima(p, max);

    return cnt;
}

int main(int argc, char **argv) {

    clock_t start = clock();

    FILE *fin, *fout;
    apri_file(&fin, argv[1], "r");
    apri_file(&fout, OUTPUT, "w");

    int n, i, cnt;
    int p[pietre];

    fscanf(fin, "%d ", &n);
    for (i = 0; i < n; i++) {
        fprintf(fout,"#TEST %d\n", i+1);
        printf("#TEST %d ", i+1);
        cnt = leggi_valori(fin, p, fout);
        fprintf(fout, "Collana massima di lunghezza %d\n", cnt);
        printf(" eseguito\n");
    }

    fclose(fin);
    fclose(fout);

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);
    return 0;
}