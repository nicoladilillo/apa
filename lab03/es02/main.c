#include <stdio.h>
#include <stdlib.h>
#define N 8
/*
 * le costanti che mi scandiscono la posizione di fine
 * esponente
 */
#define F 8
#define D 11
#define DL 15

char controll_edian()
{
    int i = 1;
    char *j = (char *)(&i);

    /*
     * Abbiamo utilizzato una variabile intera che occupa 4 byte.
     * Successivamente abbiamo passata questa l'indirizzo della variabile intera
     * ad un puntatore ti tipo char, il quale è un formato che occupa solo 1 byte.
     * In questo modo visualizzando il contenuto della nostra del nostro puntatore
     * possiamo trovare 1 se utilizziamo il formato Big Edian, visualizzando il bit più
     * significativo nell'indirizzo di memoria più basso, oppure 0 se la nostra CPU
     * utilizza il formato Little Edian
     */


    return (*j)+48;

}

/*
 * Stabilisce se si utilizza il metodo big edian o little edian
 * assegnando un valore a una variabile di controllo che sarà utilizzata successivamente
 * per scegliere quale strategia usare
 */
int find_edian()
{
    if (controll_edian() == '1') {
        printf("Formato Little Edian\n");
        return 0;
    }
    else {
        printf("Formato Big Edian\n");
        return 1;
    }
}
/*
 * utilizziamo gli spazi per scandire rispettivamente
 * segno - esponente - mantissa
 */
void spazi(int n, int dim)
{

    if (n == 1)
        printf(" ");
    else {
        switch (dim) {
            case 4:
                if (n == F+1)
                    printf(" ");
                break;
            case 8:
                if (n == D+1)
                    printf(" ");
                break;
            case 16:
                if (n == DL+1)
                    printf(" ");
                break;
        }
    }
}

/*
 * procede con la rappresentazione binaria del valore individuato
 * dalla puntatore alla cella di memoria puntata
 */
void codifica(int n, int *cnt, int d)
{
    int p, i;
    // trova p massima
    for (p=1, i= 0; 2*p<=n; p=p*2, i++);

    while (p>0) {

        spazi(*cnt, d);

        /*
         * con la nostra variabile i stampiano tutti i nostri zeri
         * mancanti al nostro numero di 8 bit
         */
        if (N - i -1 != 0) {
            printf("0");
            i++;
        } else {
            /*
             * perocediamo con la conversione binaria dopo
             * aver stampata tutti gli zeri
             */
            if (p <= n) {
                printf("1");
                n = n - p;
            } else printf("0");
            p = p/2;
        }

        /*
         * incremento il numero dei posti contati per
         * inserire la giusta spaziatura in fase di stampa
         */
        (*cnt)++;
    }
    // printf(" ");
}

/*
 * tramite questa funzione settiamo i parametri per iniziare la
 * codifica e la stampa dei nostri numeri dal bit più significativo
 */
int inizzializzazione(unsigned char **ind, void *p, int n, int edian)
{
    int i;

    if (edian == 1) {
        /*
        * se big edian settiamo il puntatore iniziale alla cella di
        * memoria con valore minore e successivamete incrementiamo di 1
        */
        *ind = p;
        i = 1;
    } else {
        /*
        * se little edian settiamo il puntatore iniziale alla cella di
        * memoria con valore massimo e successivamete disincrementiamo di 1
        */
        *ind = p+(n-1);
        i = -1;
    }

    return i;
}

stampaCodifica(void *p, int dim, int c)
{
    unsigned char *k;
    int icr = inizzializzazione(&k, p, dim, c);
    int i = 0;

    /*
     * dim*N indica da quanti bit è formato il nostro intero dato
     */
    while (i < dim*N) {

        codifica(*k, &i, dim);

        /*
         * incremento variabile per il nostro ciclo
         */
        k += icr;
    }

    printf("\n");

}

int main() {
    float af;
    double ad;
    long double ald;

    int bigEdian = find_edian();

    printf("Dimensione vettore float: %ld byte\n", sizeof(af));
    printf("Dimensione vettore double: %ld byte\n", sizeof(ad));
    printf("Dimensione vettore long doublet: %ld byte\n", sizeof(ald));

    printf("Inserisci un numero: ");
    scanf("%f", &af);

    ad = (double)af;
    ald = (long double)af;

    stampaCodifica((void *)&af, sizeof(af), bigEdian);
    stampaCodifica((void *)&ad,sizeof(ad),bigEdian);
    stampaCodifica((void *)&ald,sizeof(ald),bigEdian);

    return 0;
}