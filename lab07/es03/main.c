#include <stdio.h>
#include <memory.h>
#include <stdlib.h>
#include <ctype.h>

#define N 50+1 // 50 caratteri più tappo
#define C 6+1 // 6 caratteri più tappo, per codice
#define O 8 // massimo numero di oggetti che si possono equipaggiare
#define L 20+1 // per comandi generali

#define PRI 'P' // primo carattere codice
#define SEC 'G' // secondo carattere codice

/**
 * STRUTTURE DATI PER GIOCO
 */

/* settupla di statische personaggio */
typedef struct {
    unsigned hp, mp, atk, def, mag, spr;
} stat_t;

/* struttura per ogni oggetto dell'inventario */
typedef struct {
    char nome[N];
    char tipo[N];
    stat_t stat;
} Inv_t;

/* struttura wrapper per oggetti inventario */
typedef struct {
    Inv_t *vettInv;
    int nInv;
    int maxInv;
} tabInv_t;

/* oggi equipaggiati ad ogni personaggio [max 8] */
typedef struct {
    int inUso;
    Inv_t *vettEq[O]; // vettore di puntatori a oggetti in inventario
} tabEquip_t;

/* struttura per ogni personaggio */
typedef struct {
    stat_t stat;
    char codice[C];
    char nome[N];
    char classe[N];
    tabEquip_t equip; // oggetti equipaggiati
} Pg_t;

/* variabile di tipo puntatotr a nodePg_t */
typedef struct nodePg_t *link;

/* struttura wrapper per lista personaggi */
typedef struct {
    link headPg; // puntatore alla testa della lista
    link tailPg; // punattore alla coda della lista
    int nPg; // numero personaggi in lista
} tabPg_t;

/* struttura ti tipo nodo */
struct nodePg_t {
    Pg_t p;
    link next;
};


typedef enum {
    caricare_p, caricare_o,  aggiungere_p, eliminare_p, aggiungere_o, eliminare_o, calcolo, fine, errore
} comando_t;


/**
 * PARTE MENU'
 */

comando_t leggiComando(void)
{
    char *t[errore] = {
            "caricare_p", "caricare_o", "aggiungere_p", "eliminare_p",
            "aggiungere_o", "eliminare_o", "calcolo", "fine"
    };

    int i;
    char cmd[L];

    /* crazione printf scelte */
    printf("\nInserisci comando [");
    for (i = 0; i < errore; i++) {
        printf("%s", t[i]);
        if (i != errore - 1)
            printf("/");
    }
    printf("]: ");
    scanf("%s", cmd);

    comando_t c = caricare_p;
    while (c < errore && strcmp(t[c], cmd) != 0)
        c++;

    return c;

}

void errore_comando(char *str)
{
    printf("Errore %s!\n", str);
}

int apri_file(FILE **fp)
{
    char nome[L];

    printf("Inserire nome file: ");
    scanf("%s", nome);

    *fp = NULL;
    *fp = fopen(str, "r");
    if (*fp == NULL) {
        errore_comando("apertura file");
        return 0;
    } else
        return 1;
}

/**
 * CARICA PERSONAGGI
 */

int controllo_codice(char *cod, link x)
{
    /* controllo lunghezza */
    if (strlen(cod) != C-1)
        return 0;

    /* controllo primi valori */
    if (cod[0] != PRI && cod[1] != SEC)
        return 0;

    /* controllo cifre successive alle prime due siano numeriche */
    int i;
    for (i = 2; i < C; i++) {
        if (!isalnum(cod[i]))
            return 0;
    }

    /* controllo se codice è univoco per ogni personaggio */
    for (; x != NULL; x = x->next)
        if (strcmp(cod, x->p.codice) == 0)
            return 0;

    /* se abbiamo superato tutti i controlli il nostro codice è valido */
    return 1;
}

int leggi_personaggio(Pg_t *p, FILE *f)
{
    if (fscanf(f, "%s %s %s %d %d %d %d %d %d\n", p->codice, p->nome, p->classe, &p->stat.hp,
           &p->stat.mp, &p->stat.atk, &p->stat.def, &p->stat.mag, &p->stat.spr) != EOF)
        return 1;

    return 0;
}

void funzione_carica_p(tabPg_t *tabPg) {
    FILE *fin;

    /* verifica se presente file da leggere*/
    if (!apri_file(&fin))
        return;

    /* ciclo acquisizione lista senza sapere il numero di elementi noti */
    int cnt = 0;
    Pg_t p;
    struct nodePg_t x;
    link *x = &tabPg->headPg;
    link tail;

    while(leggi_personaggio(&p, fin)) {

        if (controllo_codice(p.codice, tabPg->headPg)) {
            tail = malloc(sizeof(struct nodePg_t)); // crea nuovo nodo
            *x = tail; // elemento precedente punta a nuovo nodo
            tail->p = p; // il nuovo nodo assume i valori appena letti
            x = &(tail->next); // x punta all'elmento next del nuovo nodo che verrà modificato successivamente
            cnt++;
        }
    }

    *x = NULL; // ultimo elemento non punata a niente
    tabPg->tailPg = tail; // punatatore ad ultimo elemento
    tabPg->nPg = cnt; // numero elementi nella lista
}


/**
 * CARICA OGGETTI
 */

/* letttura nuovo oggetto */
Inv_t nuovo_oggetto(FILE *f)
{
    Inv_t t;
    fscanf(f, "%s %s %d %d %d %d %d %d", t.nome, t.tipo, &t.stat.hp, &t.stat.mp, &t.stat.atk,
            &t.stat.def, &t.stat.mag, &t.stat.spr);

    return t;
}

void funzione_carica_o(tabInv_t *tabInv)
{
    FILE *fin ;

    /* verifica se presente file da leggere*/
    if (!apri_file(&fin))
        return;

    /* lettura numero personaggi */
    fscanf(fin, "%d", &tabInv->nInv);

    tabInv->vettInv = malloc(tabInv->nInv* sizeof(Inv_t));

    int i;
    for (i = 0; i < tabInv->nInv; i++)
        /* acquisizione nuovo oggetto */
        tabInv->vettInv[i] = nuovo_oggetto(f);

}


/**
 * AGGIUNGI PERSONAGGIO
 */

void funzione_aggiungi_p(tabPg_t *tabPg)
{
    Pg_t p;
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
    for (*x = &tabPg->headPg; x != &tabPg->tailPg; x = &(*x->next)) {
        if(strcmp(*x->p.codice, str) == 0) {
            app = *x;
            *x = *x->next; // punatatore a elemento next punta a elemento next di elemento puntato
            free(app);

            tabPg->nPg--;
            return;
        }
    }

    errore_comando("personaggio non trovato");
}


/** AGGIUNGI O ELIMINA OGGETTI */

link ricerca(tabPg_t *tabPg)
{
    link x;
    char str[N];

    printf("Inserire codice: ");
    scanf("%s", str);

    for (x = tabPg->headPg; flag && x != tabPg->headPg; x = x->next)
        if(strcmp(x->p.codice, str) == 0)
            return x;

    errore_comando("personaggio non trovato");
}

void stampa_oggetti(Inv_t *o, int n)
{
    int i;
    for (i = 0; i < n; i++)
        printf("%d) %s %s %d %d %d %d %d %d\n", i+1, o[i].nome, o[i].tipo, o[i].stat.hp, o[i].stat.mp,
               o[i].stat.atk, o[i].stat.def, o[i].stat.mag, o[i].stat.spr);
}

void funzione_aggiungi_o(tabPg_t *tabPg, tabInv_t *tabInv)
{
    if (tabInv->nInv == 0) {
        errore_comando("non ci sono oggetti in lista");
        return
    }

    link x = ricerca(tabPg);
    if (x->p.equip.inUso == 8) {
        errore_comando("il personaggio ha il numero massimo di oggetti disponibili");
        return
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

void funzione_elimina_o(tabPg_t *tabPg, tabInv_t *tabInv)
{
    link x = ricerca(tabPg);
    if (x->p.equip.inUso == 0) {
        errore_comando("il personaggio non possiede oggetti");
        return
    }

    stampa_oggetti(x->p.equip.vettEq, x->p.equip.inUso);

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
    for (i = n; i < x->p.equip.inUso-1, i++)
        x->p.equip.vettEq[n] = x->p.equip.vettEq[n+1];
    x->p.equip.inUso--;
}


void selezionaComando(comando_t e, tabPg_t *tabPg, tabInv_t *tabInv, int *f)
{
    switch (e) {
        case caricare_p:
            funzione_carica_p(tabPg);
            break;
        case caricare_o:
            funzione_carica_o(tabInv)
            break;
        case aggiungere_p:
            funzione_aggiungi_p(tabPg);
            break;
        case eliminare_p:
            funzione_eliminare_p(tabPg);
            break;
        case aggiungere_o:
            funzione_aggiungi_o(tabPg, tabInv);
            break;
        case eliminare_o:
            funzione_eliminare_o(tabPg, tabInv);
            break;
        case calcolo:
            break;
        case fine:
            printf("Fine esecuzione!\n");
            *f = 0;
            break;
        case errore:
        default:
            errore_comando("comando");
            break;
    }
}

void azzera(tabPg_t *tabPg, tabInv_t *tabInv)
{
    tabPg->nPg = 0;
    tabPg->headPg = NULL;
    tabPg->tailPg = NULL;
    tabInv->nInv = 0;
    tabInv->vettInv = NULL;
}

int main(void) {

    tabPg_t *tabPg;
    tabInv_t *tabInv;
    /* azzera valori dei wrapper degli elementi */
    azzera(tabPg, tabInv);

    comando_t e;
    int flag = 1;

    while (flag != 0) {
        e = leggiComando();
        selezionaComando(e, tabPg, tabInv, &flag);
    }


    return 0;
}