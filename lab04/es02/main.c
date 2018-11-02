#include <stdio.h>
#include <stdlib.h>

#define M 50+1

int leggi_vettore(int *m, char *nome, int MAX)
{
    FILE *fp = NULL;
    fp = fopen(nome, "r");
    if (fp == NULL)
    {
        printf("Errore parametri");
        exit(-1);
    }
    char riga[MAX];
    int x, i, cnt = 0;

    fgets(riga, MAX, fp);
    char *s = riga;

    while(sscanf(s, "%d%n", &x, &i) > 0)
    {
        s = s + i;
        m[cnt] = x;
        cnt++;
    }

    return cnt;
}

void change(int *v, int i, int j)
{
    int tmp;

    tmp = v[i];
    v[i] = v[j];
    v[j] = tmp;
}

/*
 * creiamo la nostra partizione partndo dall'elemento finale
 */
int partition(int *v, int l, int r)
{
    int i, j, x;
    i = l-1;
    j = r-1;
    x = v[j];

    /*
     * ciclo for che sostituisce semplicemente i valori agli estremi i e j
     */
    for( ; ; )
    {
        /*
         * questo ragionamento ci è possibile perchè
         * avendo assegnato inizialmente i = l-1 e j = r-2
         * possiamo incrememtare e vedere se il nostro valore è corretto
         */
        while(v[++i] < x);
        while(v[--j] > x)
            if (j == l)
                break;
        if (i >= j)
            break;
        /*
         * cambiamo i nostri valori di infici i e j
         */
        change(v, i, j);
    }
    /*
     * cambiamo la posizione del nostro indice
     * dividendo il nostro vettore in due partizioni
     */
    change(v, i, r-1);

    /*
     * posizione del nostro indice
     */
    return i;
}

void quicksort(int *v, int l, int r)
{
    int q;

    // condizione di terminazione
    if (l == r)
        return;

    /*
     * indici dove si è creata la partizione
     */
    q = partition(v, l, r);
    /*
     * ricotsione vettore sinistro
     */
    quicksort(v, l, q);
    /*
     * ricorsione vettore destro
     */
    quicksort(v, q+1, r);
    return;
}

int majiority(int *a, int N)
{
    int i, cnt = 1;

    // questo ordinamento ha complessità O(nlog(n))
    quicksort(a, 0, N);


    /*printf("-- %d numeri --\t\t\t", N);
    for(i = 0; i < N; i++)
        printf("%d ", a[i]);*/

    /*
     * una vota ordinato il vettore possiamo contare il numero di occorrenze
     * di ogni valore con complessità O(n) che risulta inferiore a O(nlog(n))
     */
    for(i = 1; i < N; i++)
    {
        if(a[i] == a[i-1])
            cnt++;
        else
            cnt = 0;

        /*
         * condizione di maggiorità
         */
        if (cnt > N/2)
            return a[i];
    }

    /*
     * se nessuna valore soddisfa la condizione di maggiorità
     */
    return -1;
}

int main(int argc, char *argv[]) {

    int v[M], n;
    n = leggi_vettore(v, argv[1], M);

    printf("\n%d", majiority(v, n));

    return 0;
}