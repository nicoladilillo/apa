#include <stdio.h>
#include <stdlib.h>

#define OUTPUT "log.txt"

typedef enum {
    zaffiri, rubini, smeraldi, topazi, pietre
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

void stampa(int *collana, int max, FILE *f)
{
  int i;
  int c;

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

void swap(int *cnt, int *f, int pos, int *c)
{
    int i;

    *cnt = pos;
    for (i = 0; i < pos; i++)
        f[i] = c[i];
}

/*
 * uno zaffiro deve essere seguito immediatamente o da un altro zaffiro o da un rubino
 * uno smeraldo deve essere seguito immediatamente o da un altro smeraldo o da un topazio
 * un rubino deve essere seguito immediatamente o da uno smeraldo o da un topazio
 * un topazio deve essere seguito immediatamente o da uno zaffiro o da un rubino.
 */
int promising(int *collana, int pos, int *finale, int *cnt)
{
    /*
     * dobbiamo dire se il valore precedente a quello puntato
     * da pos sia seguito da un valore che rispetta un certo criterio
     */
    int c = collana[pos-1];


    /*
    * valutiamo se la nostra combinazione mentre viene creata ripsetti
    * le condizioni citate in testa alla funzone per costruire la collana
    */
    switch (c) {
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
            exit(-2);
    }

    /*
    * se siamo arrivati qua dobbiamo controllare che il nostro nuovo contatore sia
    * maggiore di quello vecchio e nel caso sostituira i valori della combinazione finale
    */
    if (pos+1 > *cnt)
        swap(cnt, finale, pos+1, collana);

    return 1;
}

/*
 * funzione di ricorsiva che aggiorna, se trova valori migliori, la lunghezza massima
 * della combinazione creata e la combinazione stessa
 */
int powerset(int *occ, int pos, int *collana, int max, int *cnt, int *f)
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
    if (pos <=1  || promising(collana, pos-1, f, cnt)) {
        /*
         * per avere una ripetizione su tutti i possibili valori da inserire
         */
        for (i = 0; i < pietre; i++)
            /*
             * inseriamo un elemento solo se sono ancora disponibile una delle sue occorrenze
             */
            if (occ[i] > 0) {
                /*
                 * inseriamo il nostro valore nella lista
                 */
                collana[pos] = i;
                /*
                 * decrementiamo il numero di valori di una data pietra dal vettore delle occorrenze
                 */
                occ[i]--;
                /*
                 * chiamata ricorsiva
                 */
                if(powerset(occ, pos + 1, collana, max, cnt, f))
                    return 1;
                /*
                 * incrementiamo nuovamente il valore di una data pietra per poterla
                 * utilzzare in una prossima chiamata in una diversa posizione
                 */
                occ[i]++;
            }
    }

    return 0;
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

int leggi_valori(FILE *in, int *v, FILE *out)
{

    fscanf(in, "%d %d %d %d", &v[zaffiri], &v[rubini], &v[topazi], &v[smeraldi]);
    informazioni(v, pietre, out);


    int max = 0;
    int i;
    for (i = 0; i < pietre; i++)
        max += v[i];
    fprintf(out, "TOT = %d\n", max);

    int cnt = 0;
    int *collana = malloc(max * sizeof(int));
    int *finale = malloc(max * sizeof(int));
    powerset(v, 0, collana, max, &cnt, finale);

    stampa(finale, cnt, out);

    free(collana);
    free(finale);

    return cnt;
}

int main(int argc, char **argv) {

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

    return 0;
}