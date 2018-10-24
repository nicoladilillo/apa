#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 50
#define M 3

void cerca(int m[][N], int righe, int colonne);
void posizione_iniziale(int x, int y, int base, int altezza);

/*
*   Tramite questa funzione leggiamo la nostra matrice da file attraverso
*   l'utilizzo di puntatori, grazie ai quali assumiamo anche r nostri
*   valori di righe e colonne della matrice
*/
void leggi_matrice(int v[][N], int *righe, int *colonne, char nomefile[])
{
    FILE *fp = NULL;
    fp = fopen(nomefile, "r");
    if (fp == NULL)
    {
        printf("Errore accesso al file!\n");
        exit(-2);
    }

    int i, j;

    fscanf(fp, "%d %d\n", righe, colonne);

    for (i = 0; i < *righe; i++)
        for (j = 0; j < *colonne; j++)
            fscanf(fp, "%d ", &v[i][j]);
}

/*
*   Stampa la matrice letta con formattazione output
*/
void stampa_matrice(int m[N][N], int righe, int colonne)
{
    int i, j;

    printf("Righe: %d\t Colonne: %d\n\n", righe, colonne);

    printf("      ");
    for (i = 0; i < colonne; i++)
        printf("%d ", i+1);
    printf("\n\n");

    for (i = 0; i < righe; i++)
    {
        printf("%2d    ", i+1);
        for (j = 0; j < colonne; j++)
            printf("%d ", m[i][j]);
        printf("\n");
    }
}

int main(int argc, char *argv[])
{
    if(argc != 2)
    {
        printf("Errore parametri!\n");
        return -1;
    }
    int m[N][N];
    int nr, nc;

    clock_t start = clock();

    leggi_matrice(m, &nr, &nc, argv[1]);
    stampa_matrice(m, nr, nc);

    cerca(m, nr, nc);

    return 0;
}

int riconosciRegione(int m[][N], int nr, int nc, int righe, int *colonne, int *h, int *b)
{
    int flag = 2, z;

    if (m[righe][*colonne] == 1) {
        // calcolo altezza
        /*
        * con questo if controllo se la riga successiva possiede una sequenza di 1,
        * così incremento la base, o se quella precedente contiene già un uno, in questo
        * non faccio nulla, un controllo è già stato eseguito
        */
        if (righe == 0 || (righe > 0 && m[righe-1][*colonne] != 1)) {
            *h = 1;
            flag = 1;
            for (z = righe+1; z < nr && flag; z++) {
                if (m[z][*colonne] != 1)
                    flag = 0;
                else
                    (*h)++;
            }

            // calcolo base solo se abbiamo trovato un valore dell'altezza
            if (*colonne == 0 || (*colonne > 0 && m[righe][*colonne - 1] != 1)) {
                *b = 1;
                flag = 1;
                for (*colonne = *colonne + 1; *colonne < nc && flag; (*colonne)++) {
                    if (m[righe][*colonne] != 1) {
                        flag = 0;
                        (*colonne)--;
                    } else
                        (*b)++;
                }
            }
        }

    }

    /*
     * se il flag è diverso a due vuol dire che abbiamo eseguito dei controlli e il valore
     * del flag è sicuramente camabiato, altrimenti potremmo aver analizzato una cella
     * contenente uno zero o una cella di una regione già stampata precedentemente
     */
    if (flag != 2)
        return 1;
    else
        return 0;
}

/*
*   tramite questa funzione cerchiamo r nostri rettangoli con
*   altezza, base o area maggiore
*/
void cerca(int m[][N], int righe, int colonne)
{
    int i, j;
    int h, b;

    for (i = 0; i < righe; i++) {
        for (j = 0; j < colonne; j++) {
            /*
             * passo il valore delle colonne (j) perchè non avrò bisogno di controllare
             * le colonne successive a quelle dell'estremo controllato
             */
            if (riconosciRegione(m, righe, colonne, i, &j, &h, &b))
                /*
                 * 1) Ho aggiunto 1 a ogni estremo perchè volevo dare una visuale più realistica
                 * 2) incrementanto il contatore delle colonne (j) all'interno della funzione per la visualizzazione
                 *      della cella iniziale devo sottrarre la base
                 */
                printf("Estremo <%d,%d> di base %d e altezza %d\n", i+1, j+1-b,  b, h);
        }
    }

}