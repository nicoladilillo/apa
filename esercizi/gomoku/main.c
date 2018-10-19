#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 19

void stampa(char m[N][N]);

int main(int argc, char *argv[])
{
    char matr[N+1][N+1];
    int flag, g, a,j,i, riga, colonna;

    if(argc!=2){
        return -1;
    }
    FILE *fp= NULL;
    fp=fopen(argv[1], "r");
    if(fp==NULL){
        return -2;
    }


    // Lettura matrice da file

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            fscanf(fp, "%c", &matr[i][j]);
        fscanf(fp,"\n");
    }
    stampa(matr);


    // INIZIO GIOCO

    a = 1;
    g = 1;

    while(a<5)
    {

        printf("Turno giocatore ");
        if(g == 1)
            printf("nero ");
        else
            printf("bianco ");
        printf("(R/C): ");
        scanf("%d %d", &riga, &colonna);
        riga--;
        colonna--;

        if (riga < N && colonna < N && matr[riga][colonna] != 'N' && matr[riga][colonna] != 'B') {

            if(g == 1)
                matr[riga][colonna] = 'N';
            else
                matr[riga][colonna] = 'B';

            a = 1;
            for(j=1; (colonna+j) < N && matr[riga][colonna+j]==matr[riga][colonna]; j++) a++;
            for(j=1; a<5 && (colonna-j) > 0 && matr[riga][colonna-j]==matr[riga][colonna]; j++) a++;

            if (a<5) a=1;
            for(j=1; a<5 && j<N && j>0 && matr[riga+j][colonna]==matr[riga][colonna]; j++) a++;
            for(j=1; a<5 && j<N && j>0 && matr[riga-j][colonna]==matr[riga][colonna]; j++) a++;

            stampa(matr);

            if(g == 1)
                g = 2;
            else
                g = 1;

        } else {

            printf("Mossa non valida, Riprova!\n");

        }
    }

    printf("Vince il giocatore ");
    if(g == 1)
        printf(" Bianco\n");
    else
        printf(" Nero\n");

    return 0;
}

void stampa (char m[N][N]) {
    int i, j;

    system("clear");

    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++)
            printf("%c", m[i][j]);
        printf("\n");
    }

}
