#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define S 20  // numero sequenze
#define C 5  // caratteri sequenze
#define N 200 // caratteri riga
#define T 10 // valori visualizzati
#define D 25 // lunghezza parola

typedef struct valori {
    int n, p[100];
    char s[C+1];
    char  r[100][D];
} Valori;

/*
 * funzione che ritorna un valore costante per un carattere, sia se
 * maiuscolo, sia se minuscolod
 */
int lower(char c)
{
    if (c < 'a')
        c = c + 'a' - 'A';
    return c;
}

void apri_file(FILE **fp, char n[]) {
    *fp = NULL;
    *fp = fopen(n, "r");
    if (*fp == NULL) {
        printf("Errore lettura file\n");
        exit(-2);
    }
}

/*
 *  leggi la sequenza dei caratteri da confrotnare
 */
void leggi_sequenze(char nome[], Valori v[], int *max)
{
    FILE *fp;
    apri_file(&fp, nome);

    fscanf(fp, "%d\n", max);

    int i;

    for (i = 0; i < *max; i++) {
        fscanf(fp, "%s\n", v[i].s);
        v[i].n = 0;
    }

    fclose(fp);
}

/*
 * n --> dove inizia una parola
 * x --> dove iniziamo a contare le nostre sequenze
 * k --> inizio della parola
 * x > n sempre
 *
 * questa funzione permette di incrementare i valori di un sequenza di parole
 */
void incrementa(char parola[], Valori v[], int k, int t, int *j, int n)
{
    int i, flag = 0;
    int x = *j;
    /*
     * nella varibile tmp andremo a inserie "a pezzi" la parola
     * in cui è contenuta la nostra sequenza
     */
    char tmp[D] = " ";

    for (i = 0; isalnum(parola[x+i]) && parola[x+i] != '\0' && v[k].s[i] != '\0' && flag == 0; i++) {
        if (lower(parola[x+i]) != lower(v[k].s[i]))
            flag = 1;
        else
            tmp[i+x-n] = parola[x+i];
    }

    /*
     * se flag non cambia e confrontiamo tutta l'intera sequenza vuol dire che abbiamo
     * ottenuto la nostra corrispondenza e andiamo a incementare i nostri dati
     */
    if (flag == 0 && i == strlen(v[k].s)) {
        /*
         * per evitare di confrontare successivamente caratteri già confrontati
         * inserendo il -1 perchè incrementeremo nel for successivo
        */
        *j = x + i - 1;
        v[k].p[v[k].n] = t;
        for (i = i; isalnum(parola[x+i]) && parola[x+i] != '\0'; i++)
            tmp[i+x-n] = parola[x+i];
        tmp[i] = '\0';
        for (i = 0; (i+n) < x; i++)
            tmp[i] = parola[n+i];
        strcpy(v[k].r[v[k].n], tmp);
        v[k].n++;
    }
}

void controllo(char r[], int ns, Valori v[], int *tot)
{
    int i, j, k = 0;
    int flag = 0;

    for (j = 0; r[j] != '\0'; j++) {
        /*
         * con il flag teniamo conto se il nostro carattere è alfanumerico (flag == 0)
         * o se non lo è (flag == 1), in questo caso controlliamo se il successivo è alfanumerico
         * incrementiamo il contatore delle parole e azzeriamo il flag altrimenti non faccimo nulla
         */
        if (flag == 1 && isalnum(r[j+1])) {
            k = j + 1;
            flag = 0;
        }

        /*
         * il nostro for sarà eseguito solo se il carattere considerato è alfanumerico
         */
        if (isalnum(r[j])) {
            for (i = 0; i < ns; i++) {
                /*
                 * Con la funzione lower confrontiamo due caratteri allo stesso modo,
                 * sia se sono minuscoli, sia se sono maiuscoli.
                 */
                if (lower(r[j]) == lower(v[i].s[0])) {
                    /*
                     * Se un carattere è uguale al primo di una sequenza si inizia un controllo
                     * per verificare se quella sequenza è contnuta di seguito al carattere.
                     * I valori passati tengono conto anche della parola in cui è contenuto il carattere
                     */
                    incrementa(r, v, i, *tot, &j, k);
                }
            }
            /*
             * Se il carattere successivo non è alfanumerico vuol dire che abbiamo un'altra
             * parola e che dobbiamo inizzializzare il flag a 1 per l'iterazione successiva
             */
            if (!isalnum(r[j+1])) {
                (*tot)++;
                flag = 1;
            }
        }
    }
}

/*
 * Consente la visualizzazione delle corrispondenze delle sequenze trovate.
 * Nel nostro caso abbiamo settato la visualizzazione a sole 10 sequenze ma variando
 * il valore di T è possibile aumentare il valore
 */
void visualizza(Valori v[], int ns)
{
    int i, tot = 0;
    int j;

    for (i = 0; i < ns && v[i].n > 0; i++) {
        printf("\nLa sequenza '%s' è contenuta ", v[i].s);
        for (j = 0; j < v[i].n && tot != T ; j++) {
            printf("in '%s' (parola in posizione %d)", v[i].r[j], v[i].p[j]+1);
            /*
             * formattazione testo
             */
            if(j == v[i].n-1 || tot == T-1)
                printf(".");
            else if (j == v[i].n-2 || tot == T-2)
                printf(" e");
            else
                printf(",");

            printf("\n\t");
            tot++;
        }
    }
}

/*
 * gli argomenti passati corrispondono a:
 * argv[1] --> nome file dove sono contenute le sequenze
 * argv[2] --> nome file dov'è contenuto il testo
 */
int main(int argc, char *argv[]) {

    if (argc != 3) {
        printf("Errore parametri\n");
        return  -1;
    }


    Valori p[S];
    int s;
    /*
     * lettura delle sequenze da fileS
     */
    leggi_sequenze(argv[1], p, &s);

    FILE *fp;
    apri_file(&fp, argv[2]);

    char riga[N];
    int tot_parole = 0;

    clock_t start = clock();
    while (fgets(riga, N+1, fp) != NULL) {
        controllo(riga, s, p, &tot_parole);
    }

    /*
     * visualizza la posizione delle nostre sequenze e la parola all'interno
     * della quale sono contenute
     */
    visualizza(p, s);

    clock_t stop = clock();
    double elapsed = (double) (stop - start) / CLOCKS_PER_SEC;
    printf("\nTime elapsed: %.5f\n", elapsed);
    return 0;
}
