#include <stdio.h>
#include <stdlib.h>
#define N 30

void leggi_matrice(int (*m)[N][N], char nome[], int *righe, int *colonne);
void sposta_matrice(int (*m)[N], int v, int movimento, int max);
void visualizza_matrice(int (*m)[N][N], int righe, int colonne);

int main(int argc, char *argv[])
{
    if(argc != 2)
        return -1;

    int m[N][N], r, c;
    /*
    * assumiamo la nostra matrice tramite puntatori attraverso la
    + funzione leggi_matrice, la quale ci restituisce il valore
    * delle righe e delle colonne, sempre tramite puntatori
    */
    leggi_matrice(&m, argv[1], &r, &c);

    int v[4] = {1, 1, 1, 1}, flag = 0, app[N], g, i, p;
    while(flag == 0) {

        // scegliere se spostare righe o colonne
        printf("spostare riga[1] o colonna[2] [fine => 3]: ");
        scanf("%d", &v[0]);

        // scegliere in che direzione spostarsi
        if(v[0] == 3)
            flag = 1;
        else {
            if(v[0] == 1)
                printf("destra[1] o sinistra[-1]: ");
            else
                printf("giù[1] o su[-1]: ");
            scanf("%d", &v[1]);

            // scegliere la posizione da cui partire
            if(v[0] == 1)
                printf("scegliere la riga: ");
            else
                printf("scegliere la colonna: ");
            scanf("%d", &v[2]);

            // scegliere il numero di posizioni
            printf("Inserire numero posizioni: ");
            scanf("%d", &v[3]);

            /*
            *   Creiamo un vettore di appoggio che poi sarà ruotato
            *   indifferentemente se si tratti di riga o colonna
            */
            p = v[2]-1;
            if(v[0] == 1) {
                for(i = 0; i < c; i++)
                    app[i] = m[p][i];
                g = c;
            }
            else {
                for(i = 0; i < c; i++)
                    app[i] = m[i][p];
                g = r;
            }

            /*
            *   qui avviene la nostra rotazione
            */
            sposta_matrice(app, v[3], v[1], g);

            /*
            *   reinseriamo il nostro vettore all'interno della matrice
            */
            if(v[0] == 1)
                for(i = 0; i < c; i++)
                    m[p][i] = app[i];
            else
                for(i = 0; i < c; i++)
                    m[i][p] = app[i];

            visualizza_matrice(m, r, c);
        }
    }

    return 0;
}

/// lettura iniziale matrice
void leggi_matrice(int (*m)[N][N], char nome[], int *righe, int *colonne)
{
    FILE *fp = NULL;
    fp = fopen(nome, "r");
    if(fp == NULL)
        return -2;

    int tmp, i, j;
    fscanf(fp,"%d %d", righe, colonne);
    for(i = 0; i < (*righe); i++)
        for(j = 0; j < (*colonne); j++) {
            fscanf(fp,"%d ", &tmp);
            (*m)[i][j] = tmp;
        }
}

/// spostare la nostra matrice
void sposta_matrice(int (*m)[N], int v, int movimento, int max)
{
    int tmp, i, j, f;

    for(i = 0; i < v; i++) {

        /*
        *   in base al tipo di movimento abbiamo due diverse
        *   modalità di movimento
        */
        if(movimento == 1) {
            f = j = 0;
        } else if(movimento == -1) {
            f = j = max-1;
        }

        /*
        *   scambiamo i valori della nostra matrice
        */
        while((j+movimento)>=0 && (j+movimento)<=(max-1)) {
            tmp = (*m)[j+movimento];
            (*m)[movimento+j] = (*m)[f];
            (*m)[f] = tmp;
            j += movimento;
        }
        (*m)[f] = tmp;
    }
}

/// stampa la matrice ordinata
void visualizza_matrice(int (*m)[N][N], int righe, int colonne)
{
    int i, j;

    for(i = 0; i < righe; i++) {
        for(j = 0; j < colonne; j++)
            printf("%d ", (*m)[i][j]);
        printf("\n");
    }
}
