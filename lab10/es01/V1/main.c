#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "diagonali.h"

#define DIFFICOLTA 8

// valore moltiplicatore
const float MOLTIPLICATORE = 1.5;

int try = 0;

void apri_file(FILE **fp, char *nome)
{
    *fp = NULL;
    *fp = fopen(nome, "r");
    if (*fp == NULL) {
        printf("Errore lettura file!\n");
        exit(-1);
    }
}

/* controllo se rispettiamo tutte le condizioni finali */
int check_finale(Item_t *p, diagonale_t *d, int n_diag, diagonale_t *d_f, float *punti_finali, float DP, float punti)
{
    int i, j;
    int f1, f2, f3;

    f1 = f2 = f3 = 1;

    for (i = 0; i < n_diag; i++) {
        /* tutte le diag devono contenere almeno un elemento */
        if (d[i].n == 0) return 0;
        for (j = 0; (f1 || f2 || f3) && j < d[i].n; j++) {
            /* il ginnasta deve includere almeno un elemento acrobatico avanti
             * e almeno un elemento acrobatico indietro nel corso del suo programma */
            if (f1 && ITEMavanti(p[d[i].elem[j]])) f1 = 0;
            if (f2 && ITEMindietro(p[d[i].elem[j]])) f2 = 0;

            /* il ginnasta deve presentare almeno una diagonale in cui
             * compaiono almeno due elementi acrobatici in sequenza */
            if (j > 1 &&  f3 && (ITEMsequenza(p[d[i].elem[j-1]], p[d[i].elem[j-1]]))) f3 = 0;
        }
    }

    /* non abbiamo rispettato le condizioni finali */
    if (f1 || f2 || f3)
        return 0;

    /* se abbiamo trovato valori migliori cambiamo*/
    if (punti > *punti_finali) {
        *punti_finali = punti;
        for (i = 0; i < n_diag; i++)
            d_f[i] = d[i];
    }

    return 1;
}

/**
 *
 * @param p il nostro elemento
 * @param d elementi delle nostre diagonali
 * @param n_diag numero di diagonali da comporre
 * @param max massimi elementi da aggiungere nelle diagonali
 */
int promising(Item_t *p, diagonale_t *d, int max, int DD, float DP, float *punti, int n_diag)
{
    int i;
    float tot = 0.0;
    /* possiamo inserire massimo 5 elementi nella diagonale */
    if (d->n == max)
        return 0;

    /* non può superare una certa difficoltà una diagonale*/
    if (d->difficolta_tot + p->difficolta > DD)
        return 0;

    /* finale: l'elemento finale non può essere seguito da altri elementi */
    if (d->n > 0 && ITEMisfinale(p[d->elem[d->n-1]]))
        return 0;

    /* una diagonale inizia sempre frontalmente */
    if ((d->n == 0) && (!ITEMisfrontale(*p)))
        return 0;

    /* requisito di precedenza: l'elemento può essere eseguito come primo di una
     * sequenza o deve essere preceduto da almeno un altro elemento */
    if(d->n == 0 && !ITEMprimo(*p))
        return 0;

    /* punteggio totale */
    for (i = 0; i < n_diag; i++)
        tot += d[i].valore_tot;

    /* aggiungi moltipiclatore */
    for (i = 0; i < d[B-1].n; i++) {
        if (p[d[B-1].elem[i]].difficolta >= DIFFICOLTA) {
            tot += (d[B-1].valore_tot * MOLTIPLICATORE) - d[B-1].valore_tot;
            break;
        }
    }
    /* non può superare un certo valore */
    if (tot > DP)
        return 0;

    *punti = tot;
    return 1;
}

void stampa(const Item_t *p, diagonale_t *d, int n, float punti)
{
    int i, j;

    for (i = 0; i < n; i++) {
        printf("DIAGONALE %dlem: ", i+1);
        for (j = 0; j < d[i].n; j++)
            printf("%s ", p[d[i].elem[j]].nome);
        printf("\n");
    }

    //printf("\nComplessita': %elem\n", elem->difficolta_tot);
    printf("\nValore: %f\n", punti);
}

/**
 *
 * @param p tutti i nostri elementi
 * @param cnt numero elementi controllati
 * @param max massimi elementi da inserire nelle diagonali
 * @param d elementi delle nostre diagonali
 * @param n_diag numero di diagonali da comporre
 * @param punt_tot punti totali diagonali
 */
int powerset(Item_wrappper p, int cnt, int max, diagonale_t *d, int n_diag, diagonale_t *d_f, float *punt_tot, float punti)
{
    /* condizione di terminazione */
    if (punti == p.DP && cnt >= n_diag) {
        if(check_finale(p.item, d,  n_diag, d_f, punt_tot, p.DP, punti))
            return 1;
        else
            return 0;
    }

    int i, j;
    /* controllo se non possiamo aggiungere elementi per check finale */
    int flag = 1;

    /* inseriamo elementi nella diagonale selezionata come nel modo seguente */
    for (j = n_diag-1; j >= 0; j--) {
        /* controlliamo se elementi sono disponibili */
        for (i = 0; i < p.n; i++) {
            if (ITEMislibero(p.item[i], j)) {
                /* controlliamo se possiamo agiungere elemento nella diag */
                if (promising(&p.item[i], &d[j], max, p.DD, p.DP, &punti, n_diag)) {
                    ITEMfissa(&p.item[i], j); // fissiamo elemento
                    try++;
                    flag = 0; // abbiamo aggiunto un elemento

                    d[j].elem[d[j].n] = i;
                    d[j].n++;

                    d[j].valore_tot += p.item[i].valore;
                    d[j].difficolta_tot += p.item[i].difficolta;

                    if (powerset(p, cnt + 1, max, d, n_diag, d_f, punt_tot, punti))
                        return 1;

                    d[j].valore_tot -= p.item[i].valore;
                    d[j].difficolta_tot -= p.item[i].difficolta;
                    /* eliminiamo elemento dalla diagonale aggiunto dalla funzione promising */
                    d[j].n--;
                    ITEMlibera(&p.item[i], j); // liberiamo elemento
                }
            }
        }
    }

    /*
     * non abbiamo aggiunto alcun elemento alle nostre diagonali se
     * il valore di flag non cambia, quindi facciamo il check finale
     */
    if (flag) {
        check_finale(p.item, d, n_diag, d_f, punt_tot, p.DP, punti);
    }

    return 0;
}

void wrapper_powerset(Item_wrappper p, int n)
{
    diagonale_t *diagonali_finale = DIAGONALIcrea(n);
    diagonale_t *diagonali = DIAGONALIcrea(n);
    float punti = 0.0;

    powerset(p, 0, D, diagonali, n, diagonali_finale, &punti, 0);

    stampa(p.item, diagonali_finale, n, punti);

    DIAGONALIdistruggi(diagonali);
    DIAGONALIdistruggi(diagonali_finale);
}

int main(int argc, char *argv[])
{
    clock_t begin = clock();

    FILE *fp;
    apri_file(&fp, argv[1]);

    Item_wrappper p = ITEMcarica(fp);

    // ITEMstampa(p);
    wrapper_powerset(p, B);

    ITEMdealloca(p);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTIME EXECUTION: %g", time_spent);

    return 0;
}