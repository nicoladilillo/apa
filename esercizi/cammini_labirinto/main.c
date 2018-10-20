#include <stdio.h>
#include <stdlib.h>

#define N 50
#define C 'X'
#define B '+'

typedef struct {
    int r, c;
} Dim;

Dim leggi_matrice(FILE *fin, char m[][N])
{
    Dim dimensioni;
    int r, c;
    fscanf(fin, "%d %d\n", &r, &c);
    dimensioni.r = r;
    dimensioni.c = c;

    int i, j;
    for (i = 0; i < r; i++) {
        for (j = 0; j < c; j++)
            fscanf(fin, "%c", &m[i][j]);
        fscanf(fin, " "); /* legge a-capo ed eventuali spazi */
    }

    return dimensioni;
}

void stampa_matrice(char m[][N], Dim dimensioni)
{
    int i, j;

    for (i = 0; i < dimensioni.r; i++) {
        for (j = 0; j < dimensioni.c; j++)
            printf("%c", m[i][j]);
        printf("\n");
    }
}

void leggi_mosse(char nome[], char m[][N], Dim d);

int main(int argc, char *argv[]) {

    FILE *fp = NULL;
    fp = fopen(argv[1], "r");
    if (fp == NULL)
        exit(-1);

    char matr[N][N];
    Dim d = leggi_matrice(fp, matr);
    //stampa_matrice(matr, d);

    leggi_mosse(argv[2], matr, d);

    return 0;
}

int controllo_matrice(int R, int CO, int mc, int mr, Dim d)
{
    if ((CO+mc >= 0 && CO+mc-1 < d.c) && (R+mr >= 0 && R+mr-1 < d.r))
        return 1;
    else return 0;
}

int controllo_diagonale(int mc, int mr)
{
    if (abs(mc) == abs(mr))
        return 1;
    else return 0;
}

/*
 * vero solo se il contatore è minore del massimo spostamento
 * e se lo spostamento è diverso da zero
 */
int controllo_incremento(int z, int n)
{
    if ((z != 0) && ( n <= abs(z)))
        return 1;
    else return 0;
}

void leggi_mosse(char nome[], char m[][N], Dim d)
{
    FILE *fp;
    fp = fopen(nome, "r");

    // valori iniziali
    int X, Y;   X = Y = 0;
    m[X][Y] = '+';

    int x; // colonne
    int y; // righe
    int flag = 1;
    int l = 0; // lunghezza cammino
    int i, j;
    int app_x, app_y;

    while(fscanf(fp, "%d %d\n", &x, &y) == 2 && flag == 1) {
        printf("mosse x: %d, y: %d", x, y);
        y = -y; // spostamento lungo le colonne invertito
        if (!controllo_matrice(X, Y, x, y, d) && !controllo_diagonale(x,y))
            flag = 0;
        else {
            l += x != 0 ? abs(x) : abs(y);

            /*
             * con queste variabile effettuo un controllo e dico se incrementare
             * lex, le y o entrambe
             */
            app_x = x == 0 ? 0 : 1;
            app_y = y == 0 ? 0 : 1;

            for (i = app_x, j = app_y; (controllo_incremento(x, i) || controllo_incremento(y, j)) && flag == 1;
                    i += app_x, j += app_y) {
                if (m[X + j][Y + i] == B || m[X + j][Y + i] == C)
                    flag = 0;
                else
                    m[X + j][Y + i] = B;
            }

            if (flag == 1) {
                X += (j-1)*app_y;
                Y += (i-1)*app_x;
            }
            printf("\t--> %d - %d\n", X+1, Y+1);
        }
    }

    if (flag == 0)
        printf("\nCammino non valido\n");
    else {
        stampa_matrice(m, d);
        if ( X == d.r-1 && Y == d.c-1) {
            printf("Percorso valido lungo: %d\n", l);
            m[X][Y] = '+';
        }
        else printf("Non sei arrivato a destinazione\n");
    }
}