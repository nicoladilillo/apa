
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "st.h"

#define S 2 // valore iniziale massimo di elementi item nella tabella

typedef struct {
    char nome[D+1];
    char rete[D+1];
} Item;

struct symboltable {
    Item *a; // vettore di item
    int N; // numero item inseriti
    int M; // item massimi inseribili
};

/* controlla se elemento già presente */
static int STexist(struct symboltable st, Item s)
{
    int i;
    for (i = 0; i < st.N; i++) {
        if (strcmp(st.a[i].nome, s.nome) == 0)
            return i;
    }

    return -1;
}

static void ordina(Item *a, int n)
{
    int i;
    Item app;
    for (i = n-1; i >= 1; i--) {
        // se il nome è minore altrimenti termina il ciclo
        if(strcmp(a[i].nome,a[i-1].nome) < 0) {
            app = a[i];
            a[i] = a[i-1];
            a[i-1] = app;
        } else break;
    }
}

/* aggiungiamo nuovo item alla nostra tabella di simboli
 * che andremo ad ordinare*/
static void STinsert(ST st, Item s)
{
    // se abbiamo già il numero di elementi massimi riallochiamo la memoria doppiandola
    if (st->N == st->M) {
        st->M *= S;
        st->a = realloc(st->a, st->M * sizeof(Item));
    }

    st->a[st->N] = s;
    st->N++;
    ordina(st->a, st->N);
}

/* inizializza tabella di simboli */
ST STinit()
{
    ST st = malloc(sizeof(ST));
    st->a = malloc(S * sizeof(Item));
    st->M = S;
    st->N = 0;

    return st;
}

/* ritorna numero elementi in tabella */
int STget(ST st)
{
    return st->N;
}

/* cerca elemento, se lo trova restiisce chiave,
 * altrimenti restitusce falso e inserisce elemento in tabella */
int STcheck(ST st, char *nome, char *rete)
{
    Item app;
    strcpy(app.nome, nome);
    strcpy(app.rete, rete);
    int i = STexist(*st, app);
    if (i >= 0)
        return 1;
    else
        STinsert(st, app);
    return 0;
}

/* restituisce chiave dell'elemnto da trovare */
int STsearch(ST st, char *str) {
    Item app;
    strcpy(app.nome, str);

    return STexist(*st, app);
}

/* stampa nome rete */
char *STprint(ST st, int i)
{
    return &st->a[i];
}