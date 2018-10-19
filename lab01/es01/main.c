#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 20

typedef struct team {
    char name[N];
    int p[N];
} Team;

int read_file(Team *t, char name[]);

int max_point(int *cnt, Team t[], int giornate, int n_squadre)
{
    int i, j, k, max = 0;
    int sum[N];

    for(i = 0; i < n_squadre; i++) {
        sum[i] = 0;
        for(j = 0; j < giornate; j++)
            sum[i] = sum[i] + t[i].p[j];

        if(sum[i] == max) {
            cnt[k] = i;
            k++;
        } else if (sum[i] > max) {
            max = sum[i];
            cnt[0] = i;
            k = 1;
        }
    }

    return k;
}

int main(int argc, char *argv[])
{
    if(argc != 2)
        return -1;

    Team t[N];
    int q = read_file(t, argv[1]); // numero totale di squadre

    // ricerca squadra capolista giornata
    int s, flag = 0, cnt[N], k;
    while(flag == 0) {
        printf("Inserisci giornata[-1 esci]: ");
        scanf("%d",&s);

        if(s == -1) {
            flag = 1;
            printf("Esci\n");
        }
        else if(s <= strlen(t[0].name)) {
            k = max_point(cnt, t, s, q); // k è un vettore in caso ci siaano due capoliste
            for(s = 0; s < k; s++)
                printf("%s ",t[cnt[s]].name);
            printf("\n");
        }
        else
            printf("Giornata non presente\n");
    }

    return 0;
}

int read_file(Team *t, char name[])
{
    // open file to read
    FILE *fp = NULL;
    fp = fopen(name,"r");
    if(fp == NULL)
        return -2;

    // read file data information
    char point;
    int j, i = 0;

    while(fscanf(fp,"%s ", t[i].name) != EOF) {

        point = ' ';
        for(j = 0; point == ' '; j++) {
            fscanf(fp,"%d%c",&t[i].p[j],&point);
        }

        // numero di squadre
        i++;
    }

    // ritorna il numero massimo di giornate giocate
    return i;
}
