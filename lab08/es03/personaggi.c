#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "personaggi.h"


/**
* CARICA PERSONAGGI
*/

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

int leggi_personaggio(Pg_t *p, FILE *f)
{
    if (fscanf(f, "%s %s %s %d %d %d %d %d %d", p->codice, p->nome, p->classe, &p->stat.hp,
               &p->stat.mp, &p->stat.atk, &p->stat.def, &p->stat.mag, &p->stat.spr) != EOF) {
        p->equip.inUso = 0; // azzera valore oggetti in uso
        return 1;
    }

    return 0;
}

void funzione_carica_p(tabPg_t *tabPg)
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
            tail->p.equip.inUso = 0; // azzera elementi in uso
            x = &(tail->next); // x punta all'elmento next del nuovo nodo che verrà modificato successivamente
            cnt++;
        }
    }

    *x = NULL; // ultimo elemento non punata a niente
    tabPg->tailPg = tail; // punatatore ad ultimo elemento
    tabPg->nPg = cnt; // numero elementi nella lista
}

/**
 * AGGIUNGI PERSONAGGIO
 */

void funzione_aggiungi_p(tabPg_t *tabPg)
{
    Pg_t p;

    printf("[codice - nome - classe - hp - mp - atk - def - mag - spre]: ");
    leggi_personaggio(&p, stdin);

    if (controllo_codice(p.codice, tabPg->headPg)) {
        tabPg->tailPg->next = malloc(sizeof(struct nodePg_t)); // crea nuovo nodo aggiunto in coda
        tabPg->tailPg = tabPg->tailPg->next; // cambia elemento puntato da tail
        tabPg->tailPg->next = NULL; // nuovo nodo punta a NULL perchè ultimo
        tabPg->tailPg->p = p; // assegna valori appena letti al nuovo nodo
    }
}


/**
 * ELIMINA PERSONAGGIO
 */

void funzione_elimina_p(tabPg_t *tabPg)
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

link ricerca(tabPg_t *tabPg)
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


/**
 * CALCOLA STATISTICHE
 */

/* stampa le caratterstiche di un personaggio */
void stampa_personaggio(Pg_t p, stat_t s)
{
    printf("%s di codice %s possiede di classe %s: ", p.nome, p.codice, p.classe);
    printf(" hp-> %d\t atk-> %d\t spr-> %d\t", s.hp, s.atk, s.spr);
    printf(" mp-> %d\t def-> %d\t mag-> %d\n", s.mp, s.def, s.mag);
}

/* somma totale statistiche */
void somma(stat_t *s, stat_t v)
{
    s->def += v.def;
    s->mp += v.mp;
    s->atk += v.atk;
    s->spr += v.spr;
    s->hp += v.hp;
    s->mag += v.mag;
}

/* se una statistica è minore di 0 assegna valore 1 */
int v(int x)
{
    if (x < 0)
        return 1;
    else
        return x;
}

/* controlla eventuali valori minori di zero */
void check(stat_t *s)
{
    s->def = v(s->def);
    s->mp = v(s->mp);
    s->atk = v(s->atk);
    s->spr = v(s->spr);
    s->hp = v(s->hp);
    s->mag = v(s->mag);
}

void funzione_calcola_statistiche(tabPg_t *tabPg)
{
    link x = ricerca(tabPg);
    if (x == NULL) return; // se personaggio non trovato

    if (x->p.equip.inUso == 0) {
        errore_comando("il personaggio non possiede oggetti");
    }

    int i;
    stat_t s;
    s.hp = s.spr = s.mag = s.atk = s.def = s.def = 0;
    for (i = 0; i < x->p.equip.inUso; i++)
        somma(&s, x->p.equip.vettEq[i]->stat);

    somma(&s, x->p.stat);
    check(&s);

    stampa_personaggio(x->p, s);
}


/**
 * AGGIUNGI O ELIMINA OGGETTI PERSONAGGIO
 */

void funzione_aggiungi_o(tabPg_t *tabPg, tabInv_t *tabInv)
{
    if (tabInv->nInv == 0) {
        errore_comando("non ci sono oggetti in lista");
        return;
    }

    link x = ricerca(tabPg);
    if (x == NULL) return; // se personaggio non trovato

    if (x->p.equip.inUso == 8) {
        errore_comando("il personaggio ha il numero massimo di oggetti disponibili");
        return;
    }

    stampa_oggetti(tabInv->vettInv, tabInv->nInv);

    int n;
    printf("Inserire numero oggetto: ");
    scanf("%d", &n);
    if (n > tabInv->nInv) {
        errore_comando("oggetto non presente");
        return;
    }

    /* assegnazione oggetto personaggio */
    n--;
    x->p.equip.vettEq[x->p.equip.inUso] = &tabInv->vettInv[n];
    x->p.equip.inUso++;
}

void funzione_elimina_o(tabPg_t *tabPg)
{
    link x = ricerca(tabPg);
    if (x == NULL) return; // se personaggio non trovato

    if (x->p.equip.inUso == 0) {
        errore_comando("il personaggio non possiede oggetti");
        return;
    }

    stampa_oggetti_puntati(x->p.equip.vettEq, x->p.equip.inUso);

    int n;
    printf("Inserire numero oggetto: ");
    scanf("%d", &n);
    if (n > x->p.equip.inUso) {
        errore_comando("oggetto non presente");
        return;
    }

    /* eliminazione oggetto personaggio */
    n--;
    int i;
    for (i = n; i < x->p.equip.inUso-1; i++)
        x->p.equip.vettEq[n] = x->p.equip.vettEq[n+1];
    x->p.equip.inUso--;
}


/*
 * RICERCA PERSONAGGIO
 */
void funzione_ricerca_p(tabPg_t *tabPg)
{
    link x = ricerca(tabPg);
    if (x == NULL)
        return;

    printf("Codice presente! NOME: %s\n", x->p.nome);
}

void funzione_ricerca_p_statistiche(tabPg_t *tabPg)
{
    link x = ricerca(tabPg);
    if (x == NULL)
        return;

    /* Stampiamo tutti i valori del personaggio */
    stampa_personaggio(x->p, x->p.stat);
    printf("\n");

    /* stampa oggetti posseduti dal personaggio */
    if (x->p.equip.inUso > 0)
        stampa_oggetti_puntati(x->p.equip.vettEq, x->p.equip.inUso);
    else
        printf("Il personaggio non possiede oggetti\n");
}