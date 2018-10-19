#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define N 50
#define M 3

/*
*   questa struttura ci sarà utile per gestire la nostra stampa
*/
typedef struct posizione {
    int n[2];
    int t[M];
} Posizioni;

void cerca(int m[][N], int righe, int colonne, Posizioni p[]);
void controllo(int base, int altezza, Posizioni p[], int x, int y);
void posizione_iniziale(Posizioni c[], int x, int y, int base, int altezza, int i);

/*
*   Tramite questa funzione leggiamo la nostra matrice da file attraverso
*   l'utilizzo di puntatori, grazie ai quali assumiamo anche i nostri
*   valori di righe e colonne della matrice
*/
void leggi_matrice(int v[][N], int *righe, int *colonne, char nomefile[])
{
    FILE *fp = NULL;
    fp = fopen(nomefile, "r");
    if (fp == NULL)
    {
        printf("Errore accesso al file!\n");
        return -2;
    }

    int i, j;

    fscanf(fp, "%d %d\n", righe, colonne);

    for (i = 0; i < *righe; i++)
        for (j = 0; j < *colonne; j++)
            fscanf(fp, "%d ", &v[i][j]);
}

/*
*   Stampa la matrice letta
*/
void stampa_matrice(int m[N][N], int righe, int colonne)
{
    int i, j;

    printf("Righe: %d\t Colonne: %d\n\n", righe, colonne);

    for (i = 0; i < righe; i++)
    {
        for (j = 0; j < colonne; j++)
            printf("%d ", m[i][j]);
        printf("\n");
    }
}

void stampa(Posizioni p[])
{
    int i;

    for(i = 0; i < M; i++) {
        printf("Max ");
        switch (i)
        {
        case 0:
            printf("Base: ");
            break;
        case 1:
            printf("Altezza: ");
            break;
        case 2:
            printf("Aerea: ");
            break;
        }
        printf("estr. sup. SX=<%d,%d> b = %d, h = %d, Area = %d\n",
               p[i].n[0], p[i].n[1], p[i].t[0], p[i].t[1], p[i].t[2]);
    }

}

void azzera(Posizioni m[])
{
    int i, j;
    for (i = 0; i < M; i++)
        for (j = 0; j < M; j++)
            m[i].t[j] = 0;
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
    Posizioni p[M]; // posizioni di base, altezza e area

    clock_t start = clock();

    azzera(p);

    leggi_matrice(m, &nr, &nc, argv[1]);
    stampa_matrice(m, nr, nc);

    cerca(m, nr, nc, p);
    stampa(p);

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);

    return 0;
}

/*
*   tramite questa funzione cerchiamo i nostri rettangoli con
*   altezza, base o area maggiore
*/
void cerca(int m[][N], int righe, int colonne, Posizioni p[])
{
    int i, j, k, z;
    int flag, b, h;

    for (i = 0; i < righe; i++) {
        for (j = 0; j < colonne; j++) {
            if (m[i][j] == 1) {

                // calcolo altezza
                /*
                * con questo if controllo se la riga successiva possiede una sequenza di 1,
                * così incremento la base, o se quella precedente contiene già un uno, in questo
                * non faccio nulla, un controllo è già stato eseguito
                */
                if (i == 0 || (i > 0 && m[i-1][j] != 1)) {
                    h = 1;
                    flag = 0;
                    for (z = i+1; z < righe && flag == 0; z++) {
                        if (m[z][j] != 1)
                            flag = 1;
                        else
                            h++;
                    }
                }


                // calcolo base
                if(j == 0 || (j > 0 && m[i][j-1] != 1)) {
                    b = 1;
                    flag = 0;
                    for (j = j+1; j < colonne && flag == 0; j++) {
                        if (m[i][j] != 1)
                            flag = 1;
                        else
                            b++;
                    }
                }

                controllo(b, h, p, j, i);
            }
        }
    }

}

/*
*   in p[0] avremo la base
*   in p[1] avremo l'altezza
*   in p[2] avremo l'aerea
*/
void controllo(int base, int altezza, Posizioni p[], int x, int y)
{
    if(base > p[0].t[0]) {
        posizione_iniziale(p, x, y, base, altezza, 0);
    }

    if(altezza > p[1].t[1]) {
        posizione_iniziale(p, x, y, base, altezza, 1);
    }

    if(base*altezza > p[2].t[2]) {
        posizione_iniziale(p, x, y, base, altezza, 2);
    }
}

void posizione_iniziale(Posizioni c[], int x, int y, int base, int altezza, int i)
{
    c[i].n[0] = y; // riga
    c[i].n[1] = x-base; // colonna
    c[i].t[0] = base;
    c[i].t[1] = altezza;
    c[i].t[2] = base*altezza;
}
