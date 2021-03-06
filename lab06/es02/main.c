#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define N 50+1 // per char item
#define M 20 // per comandi
#define C 5+1 // per codice con terminatore stringa

#define TASTIERA "tastiera"
#define FIL "file"
#define VIDEO "video"
#define CODICE "codice"
#define DATA "data"
#define L 'A' // carattere inizio codice


/**
 * TIPI DATO
 */

typedef enum {
    acquisizione, ricerca, cancellazione, stampa, fine, errore
} Comando;

typedef struct {
    char codice[C]; // AXXXX, dove X rappresenta una cifra nell'intervallo 0-9, ed è univoco
    char nome[N];
    char cognome[N];
    char data[M];
    char via[N];
    char citta[N];
    int cap;
} Item;

typedef struct node *link;

struct node {
    Item v;
    link next;
};


/**
 * FINE
 */

void funzione_fine(int **f)
{
    **f = 0;
    printf("Fine esecuzione!\n");
}


/**
 * ERRORE
 */

void funzione_errore()
{
    printf("Errore comando!\n");
}

/*
 * apri file sia per lettura che per scrittura
 */
void open_file(FILE **f, char *n, char *c)
{
    *f = NULL;
    *f = fopen(n, c);
    if (*f == NULL)
        printf("Errore apertura file!\n");
}


/**
 * ACQUISIZIONE
 */

int apri_file(FILE **f)
{
    char nome[M];
    printf("Inserire nome file: ");
    scanf("%s", nome);

    open_file(f, nome, "r");
    if (*f == NULL)
        return -1;

    /*
     * leggi e ritorna il numero dei valori contenuti nel file
     */
    int n;
    fscanf(*f, "%d", &n);

    return n;
}

int controllo_codice(char *str, link hd)
{
    /*
     * controlliamo se il codice è formato come primo carattere
     * da A e i successivi C-1 da caratteri numerici
     */
    int i;
    for (i = 0; str[i] != '\0'; i++) {
        /*
         * il controllo nel codice se il primo valore è 'A' viene fatto anche più avanti
         */
        if ((i == 0 && str[i] != 'A') || (i > 0 && !isdigit(str[i]))) {
            printf("Codice %s non valido!\n", str);
        }
    }

    /*
     * controlliamo se il codice è della giusta lunghezza
     */
    if (i != C-1) {
        printf("Codice %s lunghezza non valida!\n", str);
        return 0;
    }

    /*
     * controlliamo se il codice non è ripetuto
     */
    link x;
    for (x = hd;hd !=NULL && x != NULL; x = x->next) {
        if (strcmp(str, x->v.codice) == 0) {
            printf("Codice %s gia' esistente!\n", str);
            return 0;
        }
    }

    return 1;
}

Item lettura(FILE *f, link hd)
{
    Item t;

    /*
     * scansiona l'intera riga
     */
    fscanf(f, "%s %s %s %s %s %s %d", t.codice, t.nome, t.cognome,
           t.data, t.citta, t.via, &t.cap);

    /*
     * se il condice di controllo restituisce zero esegui questo controllo
     */
    if (!controllo_codice(t.codice, hd)) {
        /*
         * controlla più avanti questo valore nel codice
         */
        t.codice[0] = 'Z';
    }

    return t;
}

/*
 * se d1 è più recente di d2 allora la funzione ritornerà un valore positivo,
 * quindi d2 sarà una data più vecchia
 */
int confronto_data(char *d1, char *d2)
{
    int a1, m1, g1;
    sscanf(d1, "%d/%d/%d", &g1, &m1, &a1);

    int a2, m2, g2;
    sscanf(d2, "%d/%d/%d", &g2, &m2, &a2);

    // confronto anno
    if (a1 != a2)
        return a1-a2;

    // confronto mese
    if (m1 != m2)
        return m1-m2;

    // confronto giorno
    if (g1 != g2)
        return g1-g2;

    // se giorni uguali
    return 0;
}

void ordinamento(link *h, link t)
{
    /*
     * flag == a 1 se non abbiamo inseritoil nostro valore in lista
     */
    int flag = 1;
    link app, x, tmp;

    /*
     * confronto del primo valore
     */
    if (confronto_data((*h)->v.data, t->v.data ) < 0) {
        x = *h;
        *h = t;
        t->next = x;
        flag = 0;
    }

    /*
     * confronto per inserire elemento non come primo valore
     */
    for (app = *h, x = app->next; flag && x != NULL; app = x, x = x->next) {
        if (confronto_data(x->v.data, t->v.data ) < 0) {
            tmp = app->next;
            app->next = t;
            t->next = tmp;
            flag = 0;
        }
    }

    /*
     * inserisci in coda
     */
    if (flag) {
        app->next = t;
        t->next = NULL;
    }

}

link acquisizione_dati(link hd, int MAX, FILE *f)
{
    int i;
    link app;

    for (i = 0; i < MAX; i++ ){
        app = malloc(sizeof *app);

        /*
         * assegna valori letti ad una variabile di appoggio
         */
        app->v = lettura(f, hd);

        /*
         * se rispetta le condizioni iniziali del codice allora si inserisci
         * l'elemto il lista altrimenti si disalloca
         */
        if(app->v.codice[0] == L) {
            /*
             * in caso di vettore vuoto
             */
            if (hd == NULL) {
                hd = app;
                app->next = NULL;
            } else {
                ordinamento(&hd, app);
            }
        } else {
            free(app);
        }
    }

    return hd;
}

void funzione_acquisizione(link *hd)
{
    char cmd[M];
    FILE *fp;
    int n;

    printf("%s/%s? ", TASTIERA, FIL);
    scanf("%s", cmd);

    if(strcmp(cmd, TASTIERA) == 0) {
        fp = stdin;
        n = 1;
        printf("[codice nome cognome data città via CAP]: ");
    } else if (strcmp(cmd, FIL) == 0) {
        n = apri_file(&fp);
    } else {
        funzione_errore();
        return;
    }

    *hd = acquisizione_dati(*hd, n, fp);

    if (fp != stdin && fp != NULL)
        fclose(fp);
}


/**
 * STAMPA
 */

void operazione_stampa(Item t, FILE *fp)
{
    fprintf(fp, "%s %s %s %s %s %s %d\n", t.codice, t.nome, t.cognome, t.data, t.citta, t.via, t.cap);
}

void funzione_stampa(link hd)
{
    char cmd[C];
    link t;
    FILE *fp;

    printf("%s/%s? ", VIDEO, FIL);
    scanf("%s", cmd);

    if (strcmp(VIDEO, cmd) == 0) {
        fp = stdout;
    } else if (strcmp(FIL, cmd) == 0) {
        open_file(&fp, "log.txt", "w+");
    } else {
        funzione_errore();
        return;
    }

    if (hd == NULL)
        printf("Lista vuota!\n");
    else {
        for (t = hd; t != NULL; t = t->next)
            operazione_stampa(t->v, fp);
    }

    if (fp != stdout)
        fclose(fp);
}


/**
 * RICERCA
 */

/*
 * In base alla chiave inserita restituisce un puntatore
 * alll'indirizzo del valore ricercato all'interno della lista
 */
link *operazione_ricerca(link *hd)
{
    link *x;
    char codice[C];

    printf("Inserisci codice ricerca: ");
    scanf("%s", codice);

    if (hd == NULL)
        printf("Lista vuota!\n");
    else {
        for (x = hd; *x != NULL; (*x) = (*x)->next)
            if (strcmp(codice, (*x)->v.codice) == 0)
                return x;
    }

    printf("Elemento non trovato!\n");
    return NULL;
}

/*
 * stampa tutti i campi dell'elemento valore ricercato
 */
void funzione_ricerca(link hd)
{
    link *x = operazione_ricerca(&hd);

    if (x != NULL)
        operazione_stampa((*x)->v, stdout);
}


/**
 * CANCELLAZIONE
 */

link cancella_data(link *hd)
{
    char d1[M], d2[M];
    printf("Inserire la prima data [gg/mm/aaaa]: ");
    scanf("%s", d1);
    printf("Inserire la seconda data [gg/mm/aaaa]: ");
    scanf("%s", d2);

    link x, y, *app, t = NULL;

    /*
     * controllo primi elementi se hd punta ad un elemento
     */
    while (*hd != NULL && confronto_data((*hd)->v.data, d1) < 0 && confronto_data((*hd)->v.data, d2) > 0) {
        /*
         * se t non punta nessun elemetno
         */
        if (t == NULL)
            t = *hd;

        app = &(*hd)->next;
        (*hd) = (*hd)->next;
    }

    /*
     * se eliminiamo tutti gli elementi avremo un errore mel
     * for successivo sde andiamo ad effettuare questa inizializzazione
     */
    if (*hd != NULL)
        y = (*hd)->next;

    for (x = *hd; x != NULL && y != NULL; y = y->next)  {
        if(confronto_data(y->v.data, d1) < 0 && confronto_data(y->v.data, d2) > 0) {
            /*
             * se t == NULL allora non punta a niente quindi crea una
             * specie di head che punta al primo elemento della lista
             * di elementi da eliminare
             */
            if (t == NULL) {
                t = y;
            }
            else {
                /*
                 * altrimenti modifica il valore del puntatore dell'ultimo elemento
                 */
                (*app) = y;
            }

            /*
             * ad app assegneremo l'indirizzo di memoria next dell'elemento che andremo
             * che elimineremo dove se aggiungeremo un altro elemento modificheremo il valore di next
             * con quello dell'elemento da inserire altrimenti alla fine sarà settato a NULL
             */
            app = &y->next;

            /*
             * esegui eliminazione in lista
             */
            x->next = y->next;

        } else {
            /*
             * se non abbiamo eliminato nessun elemento allora
             * possiamo avanzare anche con il puntatore
             * all'elmento precedente
             */
            x = x->next;
        }
    }

    /*
     * se abbiamo inserito degli elementi in lista dobbiamo
     * dobbiamo settare a NULL l'ultimo next
     */
    if (t != NULL)
        (*app) = NULL;

    return t;
}

void wrapper_cancella_data(link hd)
{
    link x, remove;

    /*
         * restituisce il primo punatore a una lista di elementi estratti
         * che verranno eliminati e disallocati
         */
    x = cancella_data(hd);
    while (x != NULL) {
        remove = x;
        (x) = (x)->next;
        operazione_stampa(remove->v, stdout);
        free(remove);
    }
}

link operazione_cancella(link *h)
{
    link x, app;
    char codice[C];

    printf("Inserisci codice: ");
    scanf("%s", codice);

    if (h == NULL)
        printf("Lista vuota!\n");
    else {

        for (app = NULL, x = *h; x != NULL; app = x, x = x->next) {
            if (strcmp(x->v.codice, codice) == 0) {
                /*
                 * modifica head
                 */
                if (x == *h)
                    *h = x->next;
                else
                    app->next = x->next;

                return x;
            }
        }
    }

    printf("Elemento non trovato!\n");
    return NULL;
}

/*
 * esegue cancellazione per codice codice
 */
void cancella_codice(link *hd)
{
    link remove = operazione_cancella(hd);
    operazione_stampa(remove->v, stdout);
    if (remove != NULL)
        free(remove);
}

void funzione_cancella(link *hd)
{
    char cmd[M];
    link x;

    printf("%s/%s? ", CODICE, DATA);
    scanf("%s", cmd);

    if (strcmp(cmd, CODICE) == 0) {
        cancella_codice(hd);
    } else if (strcmp(cmd, DATA) == 0) {
        wrapper_cancella_data(hd);
    } else {
        funzione_errore();
        return;
    }

}


/**
 * MENU'
 */

Comando leggi_comando(void) {
    char *t[errore] = {"acquisizione", "ricerca",
                       "cancellazione", "stampa",
                       "fine"};

    int i;
    char cmd[N];
    printf("\nInserisci comando [");
    for (i = 0; i < errore; i++) {
        printf("%s", t[i]);
        if (i != errore - 1)
            printf("/");
    }
    printf("]: ");
    scanf("%s", cmd);

    Comando c = acquisizione;
    while (c < errore && strcmp(t[c], cmd) != 0)
        c++;

    return c;
}

void seleziona_comando(Comando c, link *hd, int *f)
{

    switch (c) {
        case(acquisizione):
            funzione_acquisizione(hd);
            break;

        case(ricerca):
            funzione_ricerca(*hd);
            break;

        case(cancellazione):
            funzione_cancella(hd);
            break;

        case(stampa):
            funzione_stampa(*hd);
            break;

        case(fine):
            funzione_fine(&f);
            break;

        case(errore):
        default:
            funzione_errore();
            break;
    }
}


/**
 * MAIN
 */

int main() {

    int flag = 1;
    link head = NULL;
    Comando c;

    while(flag) {
        c = leggi_comando();
        seleziona_comando(c, &head, &flag);
    }

    return 0;
}