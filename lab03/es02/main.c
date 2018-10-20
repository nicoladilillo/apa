#include <stdio.h>
#include <stdlib.h>

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
 */
int find_edian()
{
    if (controll_edian() == '1') {
        printf("Formato Big Edian\n");
        return 1;
    }
    else {
        printf("Formato Little Edian\n");
        return 0;
    }
}

void codifica(int n)
{
    int p = 255;

    // trova p massima
    // for (p=1; 2*p<=n; p=p*2);

    while (p>0) {
        if (p<=n) {
            printf("1"); n=n-p;
        }
        else printf("0");
        p = p/2;
    }
    printf(" ");
}

/*
 * tramite questa funzione settiamo i parametri per iniziare la
 * codifica e la stampa dei nostri numeri
 */
int inizzializzazione(unsigned char **ind, int *p, int n, int edian)
{
    int i;

    if (edian == 1) {
        *ind = p;
        i = 1;
    } else {
        *ind = p + n - 1;
        i = -1;
    }

    return i;
}

stampaCodifica(void *p, int dim, int c)
{
    unsigned char *k;
    int icr, v;

    icr = inizzializzazione(&k, p, dim, c);
    int i = 0;
    while (i < dim) {

        v = (int)*k;
        printf("%d=", v);
        codifica(v);

        /*
         * incremento variabile per il nostro ciclo
         */
        k += icr;
        i++;
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

    int n;
    printf("Inserisci un numero: ");
    scanf("%d", &n);

    af = (float)n;
    ad = (double)n;
    ald = (long double)n;

    stampaCodifica((void *)&af,sizeof(af),bigEdian);
    stampaCodifica((void *)&ad,sizeof(ad),bigEdian);
    stampaCodifica((void *)&ald,sizeof(ald),bigEdian);

    return 0;
}