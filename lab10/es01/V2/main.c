#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#include "diagonali.h"

#define DIFFICOLTA 8

// valore moltiplicatore
const float MOLTIPLICATORE = 1.5;

void apri_file(FILE **fp, char *nome)
{
    *fp = NULL;
    *fp = fopen(nome, "r");
    if (*fp == NULL) {
        printf("Errore lettura file!\n");
        exit(-1);
    }
}

int factorial(int n)
{
    if (n == 1)
        return 1;
    else
        return n*(factorial(n-1));
}

int promising(Item_t *p, diagonale_t *d, int DD, int arrivo)
{
    /* non può superare una certa difficoltà una diagonale*/
    if (d->difficolta_tot + p->difficolta > DD)
        return 0;

    /* controllo arrivo elemento precednetie */
    if (p->ingresso != arrivo)
        return 0;

    return 1;
}

void stampa(Item_t *p, diagonale_t *d, int n, float punti, int DD, float DP)
{
    int i, j;

    printf("DD = %2d DP = %2.0f\nTOT = %5.3f\n", DD, DP, punti);
    for (i = 0; i < n; i++) {
        printf("DIAG #%d > %5.3f ", i+1, d[i].valore_tot);
        if (i == n-1 && d[i].bonus)
            printf("* %2.2f (BONUS)", MOLTIPLICATORE);
        printf("\n");
        for (j = 0; j < d[i].n; j++)
            printf("%s ", p[d[i].elem[j]].nome);
        printf("\n");
    }
}

void check_iniziale(diagonale_t *d, Item_t *p, int i)
{
    /* il ginnasta deve includere almeno un elemento acrobatico avanti
    * e almeno un elemento acrobatico indietro nel corso del suo programma */
    if (!d->f1 && ITEMavanti(p[i])) d->f1 = 1;
    if (!d->f2 && ITEMindietro(p[i])) d->f2 = 1;
}

void check_parziale(diagonale_t *d, Item_t *p, int i)
{

    // check iniziali
    check_iniziale(d, p, i);

    /* il ginnasta deve presentare almeno una diagonale in cui
     * compaiono almeno due elementi acrobatici in sequenza */
    if (!d->f3 && (ITEMsequenza(p[d->elem[d->n-2]], p[i]))) d->f3 = 1;
}

/* crea un vettore con tutte le possibili diagonali utlizzabili che non superano un determinato DD e DP
 * dove con k indichiamo l'indici della diagonale precedente nel vettore delle diagonali*/
void powersetDiagonali(Item_wrappper p, int *cnt, int pos, int max, diagonale_t *d, int k, int direzione)
{
    /* condizione di terminazione */
    if (pos >= max)
        return;

    int i;
    int q; // valore diagonale corrente

    /* controlliamo se elementi sono disponibili */
    for (i = 0; i < p.n; i++) {
        /* controlliamo se possiamo agiungere elemento nella diag */
        if (promising(&p.item[i], &d[k], p.DD, direzione)) {

            q = (*cnt)++; // assegniamo il valore corrente a q
            d[q] = d[k]; // copiamo la diagonale di riferimento per aggiungere altri elementi
            /* creiamo nuova diagonale */
            d[q].elem[d[q].n] = i;
            d[q].bonus = (p.item[i].difficolta >= DIFFICOLTA) ? 1 : 0;
            d[q].valore_tot += p.item[i].valore;
            d[q].difficolta_tot += p.item[i].difficolta;
            d[q].n++;

            check_parziale(&d[q], p.item, i);

            /* finale: l'elemento finale non può essere seguito da altri elementi
             * quindi possiamo non continuare le nostre chiamare ricorsive*/
            if (!ITEMisfinale(p.item[i]))
                powersetDiagonali(p, cnt, pos + 1, max, d, q, p.item[i].uscita);
        }
    }
}


/* controllo se rispettiamo tutte le condizioni finali */
int check_finale(diagonale_t *d, int n_diag, diagonale_t *d_f, float *punti_finali) {
    int i;
    int f1, f2, f3;
    float tot = 0;
    int best_bonus = -1;
    diagonale_t app;

    f1 = f2 = f3 = 1;

    for (i = 0; i < n_diag; i++) {
        if (f1 && d[i].f1)
            f1 = 0;
        if (f2 && d[i].f2)
            f2 = 0;
        if (f3 && d[i].f3)
            f3 = 0;
        tot += d[i].valore_tot;

        if (d[i].bonus) {
            if (best_bonus == -1 || d[i].valore_tot > d[best_bonus].valore_tot)
                best_bonus = i;
        }
    }

    /* non abbiamo rispettato le condizioni finali */
    if (f1 || f2 || f3)
        return 0;

    // se indici bonus con punteggio migliore modifichiamo diagonale se elemento non in ultima posizione
    if (best_bonus != -1 && best_bonus != n_diag - 1) {
        app = d[n_diag - 1];
        d[n_diag - 1] = d[best_bonus];
        d[best_bonus] = app;

        tot += d[n_diag - 1].valore_tot * (MOLTIPLICATORE - 1);
    }


    if (tot > *punti_finali) {
        *punti_finali = tot;
        for (i = 0; i < n_diag; i++)
            d_f[i] = d[i];
    }

    // modifichiamo la diagonale prima di tornare se l'avevamo cambiata
    if (best_bonus != -1 && best_bonus != n_diag - 1) {
        app = d[n_diag - 1];
        d[n_diag - 1] = d[best_bonus];
        d[best_bonus] = app;
    }

    return 1;
}

void powersetCompleto(Item_wrappper p, diagonale_t *d, diagonale_t *d_f, diagonale_t *d_app, int cnt, int n_diag,
        int pos, int punti, int start, float *punteggio)
{
    /* quando abbiamo abbiamo riempito tutte le diagonali terminiamo */
    if (pos < 0) {
        check_finale(d_app, n_diag, d_f, punteggio);
        return;
    }

    int i;
    /* inseriamo diagonali */
    for (i = start; i < cnt; i++) {
        // pruning per vedere se rispettiamo punteggio massimo
        if ((punti + d[i].difficolta_tot) <= p.DP) {
            punti += d[i].difficolta_tot; // aggiorna punti totali diagonale calcolata
            d_app[pos] = d[i];
            // ricorriamo disincrementando la posizione
            powersetCompleto(p, d, d_f, d_app, cnt, n_diag, pos - 1, punti, i, punteggio);
            punti -= d[i].difficolta_tot;
        }
    }

}

void wrapper_powerset(Item_wrappper p, int n)
{
    diagonale_t *diagonali = DIAGONALIcrea(factorial(p.n));
    int cnt = 0;
    int i;
    // elaborazione primo elemeto
    for (i = 0; i < p.n; i++) {
        /* requisito di precedenza: l'elemento può essere eseguito come primo di una
        * sequenza o deve essere preceduto da almeno un altro elemento e una diagonale
        * inizia sempre frontalmente*/
        if (ITEMisfrontale(p.item[i]) && ITEMprimo(p.item[i])) {
            // settagggio valori iniziali
            diagonali[cnt].elem[0] = i;
            diagonali[cnt].n = 1;
            diagonali[cnt].bonus = (p.item[i].difficolta >= DIFFICOLTA) ? 1 : 0;
            diagonali[cnt].valore_tot = p.item[i].valore;
            diagonali[cnt].difficolta_tot = p.item[i].difficolta;
            diagonali[cnt].f1 = diagonali[cnt].f2 = diagonali[cnt].f3 = 0;

            // check parziale diagonale su primo elemento
            check_iniziale(&diagonali[cnt], p.item, i);

            // inizio ricorsione
            cnt++;
            powersetDiagonali(p, &cnt, 1, D, diagonali, cnt-1, p.item[i].uscita);
        }
    }

    // crea diagonali finali e di appoggio
    diagonale_t *diagonali_finale = DIAGONALIcrea(n);
    diagonale_t *diagonali_app = DIAGONALIcrea(n);
    float punti = 0.0;

    /* powerset completo */
    powersetCompleto(p, diagonali, diagonali_finale, diagonali_app, cnt, n, n-1, 0, 0, &punti);

    stampa(p.item, diagonali_finale, n, punti, p.DD, p.DP);

    DIAGONALIdistruggi(diagonali);
    DIAGONALIdistruggi(diagonali_finale);
    DIAGONALIdistruggi(diagonali_app);
}

int main(int argc, char *argv[])
{
    clock_t begin = clock();

    FILE *fp;
    apri_file(&fp, argv[1]);

    Item_wrappper p = ITEMcarica(fp);

    wrapper_powerset(p, B);

    ITEMdealloca(p);

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\nTIME EXECUTION: %g", time_spent);

    return 0;
}