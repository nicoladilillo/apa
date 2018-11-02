#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define M 31 // lunghezza massima stringa + tappo
#define N 1000 // numero massimo di righe
#define VIDEO "video"
#define FIL "file"
#define DATA "data"
#define CODICE "codice"
#define PARTENZA "partenza"


#define NAME "log.txt"

typedef struct riga {
    int k;
    char c[M]; // codice tratta
    char p[M]; // partenza
    char de[M]; // destinazione
    char da[M]; // data
    char o_p[M]; // ora partenza
    char p_a[M]; // ora arrivo
    int r; // ritardo espresso in minuti
} Riga;

typedef enum {
    stmp, ordinamento, ricerca, fine, err
} comando_e;

void errore()
{
    printf("errore comando\n");
}

/*
*   legge inizialmente a matrice dal file
*/
void leggi_matrice(Riga d[], int *r, char nome[]);
void stampa(FILE *file, Riga d[], int i);
void stampa_matrice(Riga d[], int r);
comando_e leggi_comando(void);
void selezionaDati(Riga v[], int n, comando_e c, int *f);


int main(int argc, char *argv[])
{
    char nomefile[] = "corse.txt"; // nome file sempre uguale

    Riga d[N];
    int r, flag = 0;

    leggi_matrice(d, &r, nomefile);

    comando_e comando;

    while(flag != 1) {
        comando = leggi_comando();
        selezionaDati(d, r, comando, &flag);
    }
}

void leggi_matrice(Riga d[], int *r, char nome[])
{
    int i;

    FILE *fp = NULL;
    fp = fopen(nome, "r");
    if(fp == NULL) {
        printf("Errore lettura file\n");
        return -1;
    }

    fscanf(fp,"%d\n", r);
    for (i = 0; i < *r; i++) {
        fscanf(fp, "%s %s %s %s %s %s %d\n",
               d[i].c, d[i].p, d[i].de, d[i].da, d[i].o_p, d[i].p_a, &d[i].r);
        d[i].k = i; // chiave per ordinamento
    }
}

/*
 * se file sarà stdout avremo una normale printf,
 * altrimenti stamperà su file
 */
void stampa(FILE *file, Riga d[], int n)
{
    int i;

    for (i = 0; i < n; i++)
       fprintf(file, "%s %s %s %s %s %s %d\n", d[i].c, d[i].p, d[i].de, d[i].da, d[i].o_p, d[i].p_a, d[i].r);
}

void stampa_matrice(Riga d[], int r)
{
    char cmd[M];
    FILE *fp;
    printf("%s/%s? ", VIDEO, FIL);
    scanf("%s", cmd);

    if (strcmp(cmd, VIDEO) == 0)
        fp = stdout;
    else if (strcmp(cmd, FIL) == 0)
        fp = fopen(NAME, "w+");
    else {
        errore();
        return;
    }

    stampa(fp, d, r);
    /*
     * facendo fclose non potrei più
     * utilizzare printf
     */
    //fclose(fp);
}

/*
 * rende il comando tutto minucolo
 */
void strlower(char *s)
{
    if (*s != '\0') {
        *s = tolower(*s);
        s++;
    }
}

comando_e leggi_comando()
{
    comando_e c;
    char cmd[M];
    char *tabella[err] = {
            "stampa", "ordinamento", "ricerca", "fine",
    };

    printf("Comando stampa/ordinamento");
    printf("/ricerca/fine: ");
    scanf("%s", cmd);
    strlower(cmd); // nel caso il comando venga con caratteri maiuscoli
    c = stmp;
    while (c < err && strcmp(cmd,tabella[c]) != 0)
        c++;

    return c;
}

void swap(Riga m[], int i,  int j)
{
    Riga tmp;

    tmp = m[i];
    m[i] = m[j];
    m[j] = tmp;
}

int confrontodata (const Riga * a, const Riga * b)
{
    int n = strcmp(a->da, b->da);
    if (n == 0)
        n = strcmp(a->o_p, b->o_p);

    return n;
}

int confrontocodice (const Riga * a, const Riga * b)
{
   return strcmp(a->c, b->c);
}

int confrontopartenza (const Riga * a, const Riga * b)
{
    return strcmp(a->p, b->p);
}

void ordina_vettore (Riga v[], int n, int (*comp)(const Riga*,const Riga*))
{
    int i, j, m;
    for (i = 0; i < n - 1; i++)
        for (j = i+1; j < n; j++) {
            m = (*comp)(&v[i], &v[j]);
            /*
             * con la seconda condiione verifichiamo la stabilità
             */
            if (m > 0 || (m == 0 && v[j].k < v[i].k))
                swap(v, i, j);
        }
}

void ordina(Riga v[], int n, int **f)
{
    char cmd[M];
    printf("%s/%s/%s? ", DATA, CODICE, PARTENZA);
    scanf("%s", cmd);

    if (strcmp(cmd, DATA) == 0) {
        ordina_vettore(v, n, confrontodata);
       **f = 0;
    } else if (strcmp(cmd, CODICE) == 0) {
        ordina_vettore(v, n, confrontocodice);
        **f = 0;
    } else if (strcmp(cmd, PARTENZA) == 0) {
        ordina_vettore(v, n, confrontopartenza);
        **f = 2;
    } else {
        errore();
        return;
    }


}

void selezionaDati(Riga v[], int n, comando_e c, int *f)
{
    switch(c) {
        case stmp:
            stampa_matrice(v, n);
            break;
        case ordinamento:
            ordina(v, n, &f);
            break;
        case ricerca:
            break;
        case fine:
            printf("Uscita\n");
            *f = 1;
            break;
        case err:
        default: printf("comando errato\n");
    }
}