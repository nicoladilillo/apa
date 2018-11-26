#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define M 31 // lunghezza massima stringa + tappo
#define N 1000 // numero massimo di righe
#define R 4 // data - codice - partenze - arrivi -> 4 tipi di chiavi
#define VIDEO "video"
#define FIL "file"
#define DATA "data"
#define CODICE "codice"
#define PARTENZA "partenza"
#define  ARRIVO "arrivo"

#define NAME "log.txt"

/*
 * tipo dato che conterrà tutte le nostre informazioni
 */
typedef struct riga {
    // int k; non abbiamo bisogno della chiave per satbilità
    char c[M]; // codice tratta
    char p[M]; // partenza
    char de[M]; // destinazione
    char da[M]; // data
    char o_p[M]; // ora partenza
    char p_a[M]; // ora arrivo
    int r; // ritardo espresso in minuti
} Riga;

/*
 * per switch menu
 */
typedef enum {
    stmp, ordinamento, ricerca, fine, err
} comando_e;

/*
 * in caso si errore con i comandi inseriti
 * stampa il seguente messaggio
 */
void errore()
{
    printf("errore comando\n");
}

/*
*   legge inizialmente a matrice dal file
*/
void leggi_matrice(Riga d[], int *r, char nome[])
{
    int i;

    FILE *fp = NULL;
    fp = fopen(nome, "r");
    if(fp == NULL) {
        printf("Errore lettura file\n");
        exit(-1);
    }

    fscanf(fp,"%d\n", r);
    for (i = 0; i < *r; i++) {
        fscanf(fp, "%s %s %s %s %s %s %d\n",
               d[i].c, d[i].p, d[i].de, d[i].da, d[i].o_p, d[i].p_a, &d[i].r);
    }
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

/*
 * per facilitare utlizzo menu con switchs
 */
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

/*
 * è la funzione che ci permette di selezionare il comando
 * impartito nel nostro menu
 */
void selezionaDati(Riga v[], int n, comando_e c, int *f, Riga *(*p)[][N]);

/*
 * assegna valore indirizzo iniziale delle view tale da
 * avere una view con la stessa sequenza inizale di dati
 */
void azzera(Riga *d, Riga *(*m)[][N], int n)
{
    int i, j;
    Riga ***tmp = *m;
    for (i = 0; i < R; i++)
        for (j = 0; j < n; j++)
            (*m)[i][j] = &d[j];

}

int main(int argc, char *argv[])
{
    char nomefile[] = "corse.txt"; // nome file sempre uguale

    Riga d[N];
    int r, flag = 5;
    /*
     * il nostro flag avrà 4 valore:
     * - 0 se ordinamento su view data
     * - 1 se ordinamento su view codice
     * - 2 se ordinamento su view partenza
     * - 3 se ordinamento su view arrivi
     * - 4 se dobbiamo concludere il nostro ciclo while di assimilazione comandi
     * - 5 se non abbiamo ancora ordinato il nostro vettore
     */

    Riga *p[R][N]; // numero di ordinamenti
    /*
     * è una matrice di punatori di tipo Riga dove ogni riga
     * si riferisce a una determinata view di ordinamento:
     * - p[0] è la view di ordinamento per data
     * - p[1] è la view di ordinamento per codice
     * - p[2] è la view di ordinamento per partenza
     * - p[3] è la view di ordinamento per arrivi
     */

    leggi_matrice(d, &r, nomefile);
    /*
     * prima di effettuare ogni ordinameto tramite view assegniamo come
     * valore di partenza di ogni puntatore in ogni colonna il ripsettiva
     * indirizzo della colonna del nostro vettore di dati letto
     */
    azzera(d, p, r);

    comando_e comando;

    while(flag != 4) {
        comando = leggi_comando();
        selezionaDati(d, r, comando, &flag, p) ;
    }
}


/**
 * PARTE STAMPA
 */

/*
 * se file sarà stdout avremo una normale printf,
 * altrimenti stamperà su file
 */
void stampa(FILE *file, Riga *d, int n, int f, Riga  *(*p)[][N]) {
    int i;

    /*
     * se f == 4 vuol dire che il nostro vettore non è stato ancora ordinato,
     * se così non fosse prenderemo come riferimento le nostre view di
     * ordinamento in base alla chiave altrimenti stampiamo un semplice
     * vettore di valori e non di puntatori
     */
    if (f == R+1)
    {
        for (i = 0; i < n; i++)
            fprintf(file, "%s %s %s %s %s %s %d\n", d[i].c, d[i].p, d[i].de,
                    d[i].da, d[i].o_p, d[i].p_a, d[i].r);
    } else {
        for (i = 0; i < n; i++)
            fprintf(file, "%s %s %s %s %s %s %d\n", (*p)[f][i]->c, (*p)[f][i]->p, (*p)[f][i]->de,
                    (*p)[f][i]->da, (*p)[f][i]->o_p, (*p)[f][i]->p_a, (*p)[f][i]->r);
    }
}

void stampa_matrice(Riga d[], int r, int f, Riga **v[])
{
    char cmd[M];
    FILE *fp;
    printf("%s/%s? ", VIDEO, FIL);
    scanf("%s", cmd);

    /*
     * seleziona se stampare a video o su file
     */
    if (strcmp(cmd, VIDEO) == 0)
        fp = stdout;
    else if (strcmp(cmd, FIL) == 0)
        fp = fopen(NAME, "w");
    else {
        errore();
        return;
    }

    stampa(fp, d, r, f, v);
    /*
     * facendo fclose non potrei più
     * utilizzare printf
     */

    if (fp != stdout)
        fclose(fp);
}


/**
 * PARTE ORDINAMENTO
 */


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

int confrontoarrivo (const Riga * a, const Riga * b)
{
    return strcmp(a->de, b->de);
}

/*
 * esegue il cambio dei nostri valori
 * in modo da ordinarli
 */
void swap(Riga *(*p)[][N], int i, int j, int f)
{
    void *tmp;

    tmp = (*p)[f][i];
    (*p)[f][i] = (*p)[f][j];
    (*p)[f][j] = (Riga *)tmp;
}

/*
 * non ordineremo il vettore creeremo delle view ordinate del vettore
 * tramite vettori di puntatore di tipo Riga (nel nostro caso una matrice di puntatori)
 */
void ordina_vettore (int n, int (*comp)(const Riga*,const Riga*), Riga *(*p)[][N], int f)
{
    int i, j, m;
    for (i = 0; i < n-1; i++) {
        m = i;
        for (j = i + 1; j < n; j++) {
            /*
             * la funzione di confronto viene passata
             * come parametro all nostra funzione
             */
            if (comp((*p)[f][m], (*p)[f][j]) > 0)
                m = j;
        }
        /*
         * trova il valore minimo da cambiare
         */
        if (i != m)
            swap(p, i, m, f);
     }
}

/*
 * scegliamo il tipo di ordinamento da effettuare
 */
void ordina(int n, int **f,  Riga *(*p)[][N])
{
    char cmd[M];
    printf("%s/%s/%s/%s? ", DATA, CODICE, PARTENZA, ARRIVO);
    scanf("%s", cmd);

    /*
     * chiamiamo la funzione ordina vettore con
     * il rispettivo parametro di confronto
     */
    if (strcmp(cmd, DATA) == 0) {
        **f = 0;
        ordina_vettore(n, confrontodata, p, **f);
    } else if (strcmp(cmd, CODICE) == 0) {
        **f = 1;
        ordina_vettore(n, confrontocodice, p, **f);
    } else if (strcmp(cmd, PARTENZA) == 0) {
        **f = 2;
        ordina_vettore(n, confrontopartenza, p, **f);
    } else if (strcmp(cmd, ARRIVO) == 0) {
        **f = 3;
        ordina_vettore(n, confrontoarrivo, p, **f);
    } else {
        errore();
        return;
    }

}


/**
 * PARTE RICERCA
 */

/*
 * ricerca se il nome della città è scritto anche solo in parte
 */
int ricerca_parziale(char *str,char *str2)
{
    while (*str2 != '\0') {
        if (*str == '\0' || *str != *str2)
            return -1;
        else {
            str2++; str++;
        }
    }

    return 0;
}

/*
 * ricerca che ha complessità log(n) ma può essere utilizzata
 * solo se il nostro vettore è ordinato secondo la
 * nostra chiave di ricerca
 */
Riga *ricercadicotomica(int r, char cmd[],  Riga *(*p)[][N])
{
    int f = 2;
    int q;
    int l = 0;

    while (l <= r)
    {
        q = (l+r)/2;
        if (ricerca_parziale((*(*p)[f][q]).p, cmd) == 0)
            return &(*(*p)[f][q]); // trovato valore nella posizione q
        if (strcmp((*(*p)[f][q]).p, cmd) < 0)
            l = q+1; // ricerca nel sottovettore destro
        else
            r = q-1; // ricerca nel sottovettore sinistro
    }

    /*
     * ritorna meno uno in caso di fallita ricerca
     */
    return NULL;
}

/*
 * normale ricerca che controlla i dati uno
 * alla volta da sinstra a destra
 */
Riga *ricercalineare(Riga m[], int n, char cmd[])
{
    int i;
    for (i = 0; i < n; i++) {
        if (ricerca_parziale(m[i].p, cmd) == 0)
            return &m[i]; // ritorna posizione valore trovato
    }

    /*
     * ritorna meno uno in caso di fallita ricerca
     */
    return NULL;
}

/*
 * ricerchiamo solo per partenza
 */
void ricercapartenza(Riga v[], int n, int f,  Riga *(*p)[][N])
{
    char tratta[M];
    Riga *app;

    printf("Inserire partenza di una tratta: ");
    scanf("%s", tratta);
    if (f == 2) {
        /*
         * quando vettore è ordinato ci dà complessità log(n)
         */
        app = ricercadicotomica(n, tratta, p);
    } else {
        /*
         * per qualsiasi caso a bene
         */
        app = ricercalineare(v, n, tratta);
    }

    /*
     * se il nostro indice di ritorno è -1 vuol
     * dire che il valore cerato non esiste
     */
    if (app == NULL)
        errore();
    else
        /*
         * - stdout stampa a video
         * - app stampa il nostro valore cercato
         * - 1 perchè stampiamo un solo elemento
         * - 4 perchè effettuiamo una stampa non con i puntatori
         */
        stampa(stdout, app, 1, R+1, p);
}

/**
 * PARTE MENU'
 */

/*
 * scegliamo cosa fare e che dato manipolare
 */
void selezionaDati(Riga v[], int n, comando_e c, int *f,  Riga *(*p)[][N])
{
    switch(c) {
        case stmp:
            stampa_matrice(v, n, *f, p);
            break;
        case ordinamento:
            ordina(n, &f, p);
            break;
        case ricerca:
            ricercapartenza(v, n, *f, p);
            break;
        case fine:
            printf("Uscita\n");
            *f = R;
            break;
        case err:
        default: errore();
    }
}