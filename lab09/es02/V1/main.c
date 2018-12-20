#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define OUTPUT "log.txt"

int try = 0;

typedef enum {
    zaffiri, rubini, topazi, smeraldi, pietre
} Pietre;

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

int check(int *v, int ****lookup)
{
    /* se soluzione calcolata ritona valore 2 => vero */
    return lookup[v[0]][v[1]][v[2]][v[3]];
}

/*
 * La soluzione che memorizziamo sono identificate dal numero
 * di pietre a nostra dispoziione, che identificano la cella
 * della matrice che andiamo a modiifcare.Se effettuiamo un
 * determinato controllo con un certo numero di occorrenze di
 * pietre, allora non dobbiamo effettuare nuovamente un nuovo
 * controllo se ritrovomo lo stesso numero.
 */
void memorizza_soluzione(int *v, int ****lookup)
{
    /* setta soluzione come calcolata => setta 1 */
    lookup[v[0]][v[1]][v[2]][v[3]] = 1;
}

/* restituisce lunghezza massima collana */
int powerset(int *occ, const int pos, const int max, int *cnt, int ****lookup, const int p)
{
    int i;

    /* se abbiamo superato la lunghezza massima allora modifichiamo il valore */
    if (*cnt < pos)
        *cnt = pos;

    /* se abbiamo trovato come soluzione la lunghezza massima
     * allora non ci interessa più calcolare altre soluzioni */
    if (*cnt >= max)
        return 1;

    /* controlloliamo se abbiamo già calcolato questa soluzione, se si ritorniamo indietro*/
    if (check(occ, lookup)) {
        try++; // quante volte evitiamo di calcolare una soluzione già calcolata
        return 0;
    }


    /* per avere una ripetizione su tutti i possibili valori da inserire */
    for (i = 0; i < pietre; i++) {
        // controlla se possibili inserire pietra con questo ordine
        if (occ[i] > 0 && promising(p, i)) {
            /* decrementiamo il numero di valori di una data pietra dal vettore delle occorrenze */
            occ[i]--;

            /* chiamata ricorsiva che termina completamente tutto se pos coincide con max*/
            if (powerset(occ, pos + 1, max, cnt, lookup, i))
                return 1;

            /*incrementiamo nuovamente il valore di una data pietra per poterla
             * utilzzare in una prossima chiamata in una diversa posizione */
            occ[i]++;
        }
    }

    memorizza_soluzione(occ, lookup);

    return 0;
}

int ****malloc_lookup(int *v)
{
    int i, j, k;
    int ****lookup = malloc((v[0]+1)* sizeof(int ***));
    for (i = 0; i <= v[0]; i++) {
        lookup[i] = malloc((v[1]+1)*  sizeof(int **));
        for (j = 0; j <= v[1]; j++) {
            lookup[i][j] = malloc((v[2]+1) * sizeof(int *));
            for (k = 0; k <= v[2]; k++) {
                /*
                 * allochiamo matrice ci dimensione pari al numero
                 * delle pietre e settiamo tutti i valri pari a zero
                 * per indicare che non abbiamo calcolato ancora nessuna soluzione
                 */
                lookup[i][j][k] = calloc((v[3]+1), sizeof(int));
            }
        }
    }

    return lookup;
}

void libera(int ****lookup, int *v)
{
    int i, j, k;
    for (i = 0; i <= v[0]; i++) {
        for (j = 0; j <= v[1]; j++) {
            for (k = 0; k <= v[2]; k++) {
                free(lookup[i][j][k]);
            }
            free(lookup[i][j]);
        }
        free(lookup[i]);
    }
    free(lookup);
}

void fX(int *v, int max, int p, int *cnt)
{
    /* alloca dinamicamente stringa memorizzacione soluzione*/
    int ****lookup = malloc_lookup(v);
    /* disincremento valore di pietra aggiunta inizialmente */
    v[p]--;
    powerset(v, 1, max, cnt, lookup, p);
    v[p]++;
    libera(lookup, v);
}

int trova_lunghezza_massima(int *v, int max)
{
    int cnt = 0;
    int i;
    /*
     * crea collana con valore iniziale pari
     * a una delle possibili pietre
     */
    for (i = 0; i < pietre; i++)
        fX(v, max, i, &cnt);

    return cnt;
}

void informazioni(int *t, int n, FILE *f, int max) {
    int i;

    for (i = 0; i < n; i++)
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
    fprintf(f, "TOT = %d\n", n);
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
    /* stampa informazioni numero pietre */
    informazioni(p, pietre, out, max);

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

    /* calcolo tempo elaborazione */
    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);
    return 0;
}