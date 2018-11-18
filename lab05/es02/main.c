#include <stdio.h>
#include <stdlib.h>

#define V 2

/*
 * errore paramentri
 */
void error(int n, int parametri)
{
    if (n!= parametri) {
        printf("errore parametri!\n");
        exit(1);
    }
}

/*
 * file per apertura e controllo errori
 */
void open_file(FILE **f, char *s)
{
    *f = NULL;
    *f = fopen(s, "r");
    if (*f == NULL) {
        printf("Errore apertura file!\n");
        exit(2);
    }
}

void controllo_locazione(int *x)
{
    if (x == NULL) {
        printf("Errore di allocazione!\n");
        exit(3);
    }
}

/*
 * la funzione alloca la mia matrice e mi restituisce
 * il puntatore al primo valore
 */
int **malloc2dR(char *nome, int *r, int *c)
{
    FILE *fp;
    /*
     * apertura file e passaggio valore by reference
     */
    open_file(&fp, nome);

    int i, j, **v;
    /*
     * leggi numero di righe e colonne
     */
    fscanf(fp, "%d %d", r, c);

    /*
     * alloco prima il mio vettore di righe che
     * consiste in un vettore di puntatori
     */
    v = malloc(*r * sizeof(int));
    controllo_locazione(v);

    for (i = 0; i < *r; i++) {
        /*
         * alloca tutti i valori di una riga
         */
        v[i] = malloc(*c * sizeof(int));
        controllo_locazione(&v[i]);
        for (j = 0; j < *c; j++)
            fscanf(fp, "%d ", &v[i][j]);
    }

    return v;
}

/*
 * separa i bianchi dai neri partendo dai bianchi
 */
void separa(int **mat, int nr, int nc, int **b, int **n, int *nb, int *nn)
{
    /*
     * calcolo numero elementi bianchi e neri
     */
    *nn = (nr * nc)/2;
    *nb = (nr * nc) - *nn;

    /*
     * allocazione dinamica dei nostri vettori
     */
    *b = malloc(*nb * sizeof(int));
    *n = malloc(*nn * sizeof(int));

    /*
     * creazione variabili iniziali
     */
    int i, j, flag_c, flag_r = 0;
    int cnt_b, cnt_n;
    cnt_b = cnt_n = 0;

    /*
     * alterno basandomi sul primo valore della riga che poi
     * viene alternato lungo tutti i valori di quella riga
     */
    for (i = 0; i < nr; i++) {
        flag_c = flag_r;

        for (j = 0; j < nc; j++) {
            if (flag_c == 0) {
                (*b)[i+j/2] = mat[i][j];
                cnt_b++;
                flag_c = 1;
            } else {
                (*n)[cnt_n] = mat[i][j];
                cnt_n++;
                flag_c = 0;
            }
        }

        if (flag_r == 0)
            flag_r = 1;
        else
            flag_r = 0;
    }
}

void stampa(int *x, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d ", *(x+i));
    printf("\n");
}

int main(int argc, char *argv[])
{
    error(argc, V);

    int nr, nc;
    int **x = malloc2dR(argv[1], &nr, &nc);

    /*
     * punatatori ai vettori allocati dinamicamente
     * che separano i bianchi e i neri
     */
    int nb, nn;
    int *bianchi, *neri;
    separa(x, nr, nc, &bianchi, &neri, &nb, &nn);

    printf("BIANCHI: ");
    stampa(bianchi, nb);

    printf("NERI: ");
    stampa(neri, nn);

    free(bianchi);
    free(neri);

    return 0;
}