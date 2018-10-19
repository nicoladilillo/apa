#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define M 31 // lunghezza massima stringa + tappo
#define N 1000 // numero massimo di righe

typedef struct riga {
    char c[M]; // codice tratta
    char p[M]; // partenza
    char de[M]; // destinazione
    char da[M]; // data
    char o_p[M]; // ora partenza
    char p_a[M]; // ora arrivo
    int r; // ritardo espresso in minuti
} Riga;

typedef enum {
    r_date, r_partenza, r_capolinea,
    r_ritardo, r_ritardo_tot, r_fine, r_err
} comando_e;

/*
*   legge inizialmente a matrice dal file
*/
void leggi_matrice(Riga d[], int *r, char nome[]);
void stampa(Riga d[], int i);
void stampa_matrice(Riga d[], int r);
comando_e leggi_comando(void);
void selezionaDati(Riga v[], int n, comando_e c, int *f);

void confronto_date(Riga t[], int n)
{
    char d1[M], d2[M];
    printf("Inserisci prima data[aaaa/mm/gg]: ");
    scanf("%s", d1);
    printf("Inserisci seconda data[aaaa/mm/gg]: ");
    scanf("%s", d2);

    int i;
    for (i = 0; i < n; i++)
        if(strcmp(t[i].da,d1) > 0 && strcmp(t[i].da,d2) < 0)
            stampa(t, i);

    printf("\n");
}

void confronto_partenze(Riga t[], int n)
{
    char p[M];
    printf("Inserisci partenza: ");
    scanf("%s", p);

    int i;
    for (i = 0; i < n; i++)
        if (strcmp(p, t[i].p) == 0)
            stampa(t, i);

    printf("\n");
}

void confronto_capolinea(Riga t[], int n)
{
    char de[M];
    printf("Inserisci capolinea: ");
    scanf("%s", de);

    int i;
    for (i = 0; i < n; i++)
        if (strcmp(de, t[i].de) == 0)
            stampa(t, i);

    printf("\n");
}

void confronto_ritardi(Riga t[], int n)
{
    int tot = 0;
    char d1[M], d2[M];
    printf("Inserisci prima data[aaaa/mm/gg]: ");
    scanf("%s", d1);
    printf("Inserisci seconda data[aaaa/mm/gg]: ");
    scanf("%s", d2);

    int i;
    for (i = 0; i < n; i++)
        if(strcmp(t[i].da,d1) > 0 && strcmp(t[i].da,d2) < 0)
            tot += t[i].r;

    if (tot == 0)
        printf("Nessun ritardo\n");
    else
        printf("%d\n", tot);
}

void ritardo_totale(Riga t[], int n)
{
    char c[M];
    printf("Inserisci codice: ");
    scanf("%s", c);

    int i, tot = 0;
    for (i = 0; i < n; i++)
        if (strcmp(c, t[i].c) == 0)
            tot += t[i].r;

    if (tot == 0)
        printf("Nessun ritardo\n");
    else
        printf("%d\n", tot);
}

int main(int argc, char *argv[])
{
    char nomefile[] = "corse.txt"; // nome file sempre uguale

    Riga d[N];
    int r, flag = 0;

    leggi_matrice(d, &r, nomefile);
    //stampa_matrice(d, r);

    comando_e comando;

    while(flag == 0) {
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
    for(i = 0; i < *r; i++)
        fscanf(fp, "%s %s %s %s %s %s %d\n",
               d[i].c, d[i].p, d[i].de, d[i].da, d[i].o_p, d[i].p_a, &d[i].r);

}

void stampa(Riga d[], int i)
{
    printf("%s %s %s %s %s %s %d", d[i].c, d[i].p, d[i].de, d[i].da, d[i].o_p, d[i].p_a, d[i].r);
}

void stampa_matrice(Riga d[], int r)
{
    int i;
    for(i = 0; i < r; i++)
        stampa(d, i);
}

comando_e leggi_comando()
{
    comando_e c;
    char cmd[M];
    char *tabella[r_err] = {
        "date", "partenza", "capolinea", "ritardo", "ritardo_tot", "fine",
    };

    printf("Comando date/partenza/capolinea");
    printf("/ritardo/ritardo_tot/fine: ");
    scanf("%s", cmd);
    // cmd = strlower(cmd); // nel caso il comando venga con caratteri maiuscoli
    c = r_date;
    while (c < r_err && strcmp(cmd,tabella[c]) != 0)
        c++;

    return c;
}

void selezionaDati(Riga v[], int n, comando_e c, int *f)
{
    switch(c) {
    case r_date:
        confronto_date(v, n);
        break;
    case r_partenza:
        confronto_partenze(v, n);
        break;
    case r_capolinea:
        confronto_capolinea(v, n);
        break;
    case r_ritardo:
        confronto_ritardi(v, n);
        break;
    case r_ritardo_tot:
        ritardo_totale(v, n);
        break;
    case r_fine:
        printf("Uscita\n");
        *f = 1;
        break;
    case r_err:
    default: printf("comando errato\n");
    }
}
