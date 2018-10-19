#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 200
#define M 30

typedef struct scambio {
    char p[M];
    int n;
} Scambi;

int main(int argc, char *argv[])
{
    // lettura file dizionario
    FILE *fp = NULL;
    fp = fopen(argv[2],"r");
    if (fp == NULL)
        return -1;

    // Dichiarazione dati
    Scambi diz[M];
    int i, j, k, flag, s;

    fscanf(fp,"%d\n",&s);
    for(i = 0; i < s; i++) {
        fscanf(fp, "$%d$ %s\n", &diz[i].n, diz[i].p);
        //printf("%s\n", diz[i].p);
    }

    //apertura sorgente
    fp = NULL;
    fp = fopen(argv[1],"r");
    if (fp == NULL)
        return -2;


    char p[M];
    FILE *fw;
    fw = fopen("ricodificato.txt","w+");
    //fprintf(fw,"ciao\n");

    while(fscanf(fp,"%s",p) != EOF) {

        for (j = 0; j<strlen(p); j++) {
            flag = 0;
            // confronto con parole dizionario
            for (i = 0; i < s && flag != 2; i++) {
                //printf("%c - %c\n",p[j],diz[i].p[1]);
                if (p[j] == diz[i].p[0]) {
                    flag = 0;
                    for (k = 1; diz[i].p[k] != '\0' && flag == 0 && (j+k)<strlen(p); k++) {
                        if(p[j+k] != diz[i].p[k])
                            flag = 1;
                    }

                    if(flag == 0) {
                        fprintf(fw,"$%d$",diz[i].n);
                        flag = 2;
                    }
                }
            }
            if (flag == 2)
                j+=k;
            else
                fprintf(fw,"%c",p[j]);
        }

        fscanf(fp,"%c",&p[j]);
        if(p[j] == '\n')
            fprintf(fw,"\n");
        else
            fprintf(fw," ");
    }

    fclose(fw);
    fclose(fp);

    return 0;
}
