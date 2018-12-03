#include <stdio.h>
#include <stdlib.h>

#define OUTPUT "log.txt"

typedef struct {
    int n; // numero pietre
    int v; // valore pietre
} Item;

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

void stampa(int *collana, int max, int val, FILE *f)
{
    int i;
    int c;

    fprintf(f, "Soluzione ottima di valore %d usando %d gemma\n", val, max);
    fprintf(f, "Combinazione collana: ");
    for (i = 0; i < max; i++) {
        c = collana[i];
        switch (c) {
            case zaffiri:
                fprintf(f, "z");
                break;
            case smeraldi:
                fprintf(f, "s");
                break;
            case rubini:
                fprintf(f, "r");
                break;
            case topazi:
                fprintf(f, "t");
                break;
            default: exit(-2);
        }
    }

    fprintf(f, "\n");
}

void swap(int *cnt, int *cnt_val, int *f, int pos, int val, int *c)
{
    int i;

    *cnt = pos+1;
    *cnt_val = val;

    /* esegui cambio cominazione */
    for (i = 0; i <= pos; i++)
        f[i] = c[i];
}

/*
 * uno zaffiro deve essere seguito immediatamente o da un altro zaffiro o da un rubino
 * uno smeraldo deve essere seguito immediatamente o da un altro smeraldo o da un topazio
 * un rubino deve essere seguito immediatamente o da uno smeraldo o da un topazio
 * un topazio deve essere seguito immediatamente o da uno zaffiro o da un rubino.
 */
int promising(int *collana, int pos, int val, int *finale, int *cnt, int *cnt_val,
        int rpt, int max_rpt, int n_z, int n_s)
{
    /*
     * dobbiamo dire se il valore precedente a quello puntato
     * da pos sia seguito da un valore che rispetta un certo criterio
     */
    int i = collana[pos-1];

    /*
    * valutiamo se la nostra combinazione mentre viene creata ripsetti
    * le condizioni citate in testa alla funzone per costruire la collana
    */
    switch (i) {
        case zaffiri:
            if (collana[pos] != zaffiri && collana[pos] != rubini)
                return 0;
            break;
        case smeraldi:
            if (collana[pos] != smeraldi && collana[pos] != topazi)
                return 0;
            break;
        case rubini:
            if (collana[pos] != smeraldi && collana[pos] != topazi)
                return 0;
            break;
        case topazi:
            if (collana[pos] != zaffiri && collana[pos] != rubini)
                return 0;
            break;
        default:
            /*
             * se valore diverso da quelli possibili c'è un errore, quindi terminiamo il porgramma
             */
            exit(-2);
    }

    /*
     * il numero degli zaffiri non può essere maggiore del numero degli smeraldi
     */
    if (n_z > n_s) return 0;

    /* non dobbiamo superare le ripetizioni massime */
    if (rpt > max_rpt) return 0;

    /*
    * se siamo arrivati qua dobbiamo controllare che il nostro nuovo valore sia
    * maggiore di quello vecchio e nel caso sostituira i valori della combinazione finale
    */
    if (val > *cnt_val)
        swap(cnt, cnt_val, finale, pos, val, collana);

    return 1;
}

/*
 * funzione di ricorsiva che aggiorna, se trova valori migliori, la lunghezza massima
 * della combinazione creata e la combinazione stessa
 */
int powerset(Item *occ, int pos, int val, int *collana, int max, int *cnt, int max_val,
        int *cnt_val, int rpt, int max_rpt, int *f, int n_z, int n_s)
{
    int i;

    /*
     * condizione di terminazione che non ci fa iniziare nessuna delle nostre
     * chiamate ricorsive se abbiamo trovato una combinazione
     * pietre che possiede lunghezza massima, altrimenti si continua ad effettuare
     * chiamate ricorsive fin quando non si trova una combinazione tale da avere
     * la lunghezza massim a di pietre possibili da sistemare
     */
    if (*cnt == max)
        return 1;

    /*
     * - ricorsione per ottenere disposizini ripetute
     * - promising per controllare se stiamo procedendo nella giusta direzione
     *   controllando la mossa precedente, pos-1 perchè nella posizione pos non
     *   abiamo inserito ancora nessun valore
     */
    if (pos <= 1  || promising(collana, pos-1, val, f, cnt, cnt_val, rpt, max_rpt, n_z, n_s)) {
        /*
         * per avere una ripetizione su tutti i possibili valori da inserire
         */
        for (i = 0; i < pietre; i++)
            /*
             * inseriamo un elemento solo se sono ancora disponibile una delle sue occorrenze
             */
            if (occ[i].n > 0) {

                /* inseriamo il nostro valore nella lista */
                collana[pos] = i;
                if (i == smeraldi) n_s++;
                if (i == zaffiri) n_z++;

                /* controllo valori ripetuti */
                if(pos > 0 && collana[pos-1] == collana[pos])
                    rpt++;
                else rpt = 1;

                /* decrementiamo il numero di valori di una data pietra dal vettore delle occorrenze */
                occ[i].n--;

                /* chiamata ricorsiva */
                if(powerset(occ, pos + 1, val + occ[i].v, collana, max, cnt, max_val, cnt_val, rpt, max_rpt, f, n_z, n_s))
                    return 1;

                /*
                 * incrementiamo nuovamente il valore di una data pietra per poterla
                 * utilzzare in una prossima chiamata in una diversa posizione e
                 * stesso discorso per i contatori degli zaffiri o degli smeraldi
                 */
                occ[i].n++;
                if (i == smeraldi) n_s--;
                if (i == zaffiri) n_z--;
            }
    }
    return 0;
}

void wrapper_powerset(Item *v, int max, int max_val, FILE *f, int rpt)
{
    int cnt = 0;
    int cnt_val = 0;
    int *collana = malloc(max * sizeof(int));
    int *finale = malloc(max * sizeof(int));
    powerset(v, 0, 0, collana, max, &cnt, max_val, &cnt_val, 0, rpt, finale, 0, 0);

    stampa(finale, cnt, cnt_val, f);

    free(collana);
    free(finale);
}

void informazioni(Item *t, int N, FILE *f, int max, int ripetizioni) {
    int i;

    for (i = 0; i < N; i++)
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
        fprintf(f, " %d [%d], ", t[i].n, t[i].v);
    }

    fprintf(f, "TOT = %d ", max);
    fprintf(f, "{max_rip = %d}\n", ripetizioni);
}

void leggi_valori(FILE *in, FILE *out)
{
    /* vettore che conterrà il numero delle nostre pietre */
    Item p[pietre];
    int rpt; // numero ripetizioni
    int max = 0; // numero massimo di pietre nella collana
    int val_max = 0; // valore massimo della collana
    int i;

    /* lettuta numero pietre */
    for (i = 0; i < pietre; i++) {
        fscanf(in, "%d ", &p[i].n);
        max += p[i].n;
    }

    /* lettuta numero pietre */
    for (i = 0; i < pietre; i++) {
        fscanf(in, "%d ", &p[i].v);
        val_max += p[i].n * p[i].v;
    }

    fscanf(in, "%d", &rpt);

    /* stampa informazioni su numero e valori pietre */
    informazioni(p, pietre, out, max, rpt);

    wrapper_powerset(p, max, val_max, out, rpt);
}

int main(int argc, char **argv) {

    FILE *fin, *fout;
    apri_file(&fin, argv[1], "r");
    apri_file(&fout, OUTPUT, "w");

    int n, i;

    fscanf(fin, "%d ", &n);
    for (i = 0; i < n; i++) {
        fprintf(fout,"#TEST %d\n", i+1);
        printf("#TEST %d ", i+1);
        leggi_valori(fin, fout);
        printf(" eseguito\n");
    }

    fclose(fin);
    fclose(fout);

    return 0;
}