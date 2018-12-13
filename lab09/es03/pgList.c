#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "pgList.h"

/* struttura wrapper per lista personaggi */
struct  tabPg_t {
    link headPg; // puntatore alla testa della lista
    link tailPg; // punattore alla coda della lista
    int nPg; // numero personaggi in lista
};

/* struttura ti tipo nodo */
struct nodePg_t {
    Pg_t p;
    link next;
};

TABPG costruttore_tabPg()
{
    TABPG p = malloc(sizeof *p);
    p->headPg = NULL;
    p->tailPg = NULL;
    return p;
}

void distruttore_tabPg(TABPG p)
{
    link x, t;

    for (x = p->headPg; x != NULL; x = t)
    {
        distruttore_equip(x->p.equip);
        t = x->next;
        free(x);
    }
}

int controllo_codice(char *cod, link x)
{
    /* controllo lunghezza */
    if (strlen(cod) != C-1) {
        printf("%s codice troppo lungo\n", cod);
        return 0;
    }

    /* controllo primi valori */
    if (cod[0] != PRI && cod[1] != SEC) {
        printf("%s codice non inizia con i carattero %c%c\n", cod, PRI, SEC);
        return 0;
    }

    /* controllo cifre successive alle prime due siano numeriche */
    int i;
    for (i = 2; i < C-1; i++) {
        if (!isalnum(cod[i])) {
            printf("%s codice non conteni caratteri numeri\n", cod);
            return 0;
        }
    }

    /* controllo se codice è univoco per ogni personaggio */
    for (; x != NULL; x = x->next)
        if (strcmp(cod, x->p.codice) == 0) {
            printf("%s codice gia' esistete\n", cod);
            return 0;
        }

    /* se abbiamo superato tutti i controlli il nostro codice è valido */
    return 1;
}

void funzione_carica_p(TABPG tabPg)
{
    FILE *fin;

    /* verifica se presente file da leggere*/
    if (!apri_file(&fin))
        return;

    /* ciclo acquisizione lista senza sapere il numero di elementi noti */
    int cnt = 0;
    Pg_t p;
    link *x;
    x = &(tabPg->headPg);
    link tail = NULL;

    while(leggi_personaggio(&p, fin)) {

        if (controllo_codice(p.codice, tabPg->headPg)) {
            tail = malloc(sizeof(struct nodePg_t)); // crea nuovo nodo
            *x = tail; // elemento precedente punta a nuovo nodo
            tail->p = p; // il nuovo nodo assume i valori appena letti
            tail->p.equip = costruttore_equip(); // azzera elementi in uso
            x = &(tail->next); // x punta all'elmento next del nuovo nodo che verrà modificato successivamente
            cnt++;
        }
    }

    *x = NULL; // ultimo elemento non punata a niente
    tabPg->tailPg = tail; // punatatore ad ultimo elemento
    tabPg->nPg = cnt; // numero elementi nella lista
}

void funzione_aggiungi_p(TABPG tabPg)
{
    Pg_t p;

    printf("[codice - nome - classe - hp - mp - atk - def - mag - spr]: ");
    leggi_personaggio(&p, stdin);

    if (controllo_codice(p.codice, tabPg->headPg)) {
        tabPg->tailPg->next = malloc(sizeof(struct nodePg_t)); // crea nuovo nodo aggiunto in coda
        tabPg->tailPg = tabPg->tailPg->next; // cambia elemento puntato da tail
        tabPg->tailPg->next = NULL; // nuovo nodo punta a NULL perchè ultimo
        tabPg->tailPg->p = p; // assegna valori appena letti al nuovo nodo
    }
}

void funzione_elimina_p(TABPG tabPg)
{
    link *x, app;
    char str[N];

    printf("Inserire codice: ");
    scanf("%s", str);

    /* questo for continua fin quando x non punatto al campo next dell'elemto puntato da tail */
    for (x = &tabPg->headPg; x != &tabPg->tailPg; x = &(*x)->next) {
        if(strcmp((*x)->p.codice, str) == 0) {
            app = *x;
            *x = (*x)->next; // punatatore a elemento next punta a elemento next di elemento puntato
            free(app);

            tabPg->nPg--;
            return;
        }
    }

    errore_comando("personaggio non trovato");
}

link ricerca(TABPG tabPg)
{
    link x;
    char str[N];

    printf("Inserire codice: ");
    scanf("%s", str);

    for (x = tabPg->headPg; x != NULL; x = x->next)
        if(strcmp(x->p.codice, str) == 0)
            return x;

    errore_comando("personaggio non trovato");
    return NULL;
}

void funzione_aggiungi_o(TABPG tabPg, TABINV tabInv)
{
    if (print_tabInv_nInv(tabInv) == 0) {
        errore_comando("non ci sono oggetti in lista");
        return;
    }

    link x = ricerca(tabPg);
    if (x == NULL) return; // se personaggio non trovato

    if (print_equipArray_inUso(x->p.equip) == 8) {
        errore_comando("il personaggio ha il numero massimo di oggetti disponibili");
        return;
    }

    print_tabInv(tabInv);

    int n;
    printf("Inserire numero oggetto: ");
    scanf("%d", &n);
    if (n > print_tabInv_nInv(tabInv)) {
        errore_comando("oggetto non presente");
        return;
    }

    /* assegnazione oggetto personaggio */
    n--;
    assegnazione_oggetto(x->p.equip, n);
}

void funzione_elimina_o(TABPG tabPg, TABINV tabInv)
{
    link x = ricerca(tabPg);
    if (x == NULL) return; // se personaggio non trovato

    if (print_equipArray_inUso(x->p.equip) == 0) {
        errore_comando("il personaggio non possiede oggetti");
        return;
    }

    print_equipArray(x->p.equip, tabInv);

    int n;
    printf("Inserire numero oggetto: ");
    scanf("%d", &n);
    if (n > print_equipArray_inUso(x->p.equip)) {
        errore_comando("oggetto non presente");
        return;
    }

    /* eliminazione oggetto personaggio */
    n--;
    eliminazione_oggetto(x->p.equip, n);
}

void funzione_ricerca_p_statistiche(TABPG tabPg, TABINV tabInv)
{
    link x = ricerca(tabPg);
    if (x == NULL)
        return;

    /* Stampiamo tutti i valori del personaggio */
    stampa_personaggio(x->p, x->p.stat);
    printf("\n");

    /* stampa oggetti posseduti dal personaggio */
    if (print_equipArray_inUso(x->p.equip) > 0)
        print_equipArray(x->p.equip, tabInv);
    else
        printf("Il personaggio non possiede oggetti\n");
}

void funzione_ricerca_p(TABPG tabPg)
{
    link x = ricerca(tabPg);
    if (x == NULL)
        return;

    printf("Codice presente! NOME: %s\n", x->p.nome);
}

void funzione_calcola_statistiche(TABPG tabPg, TABINV tabInv)
{
    link x = ricerca(tabPg);
    if (x == NULL) return; // se personaggio non trovato

    int n = print_equipArray_inUso(x->p.equip);
    if (n == 0)
        printf("il personaggio non possiede oggetti\n");

    int i;
    stat_t s;
    s.hp = s.spr = s.mag = s.atk = s.def = s.mp = 0;
    for (i = 0; i < n; i++)
        somma(&s, print_tabInv_vettInv_stat(tabInv, print_equip_indice(x->p.equip, i)));

    somma(&s, x->p.stat);
    check(&s);

    stampa_personaggio(x->p, s);
}