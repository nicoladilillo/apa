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
    int hp, mp, atk, def, mag, spr;
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
    caricare_p, caricare_o, aggiungere_p, eliminare_p,
    aggiungere_o, eliminare_o, calcolo, fine, errore
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

int apri_file(char *str, FILE **fp)
{
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

link nuovo_nodoPg(FILE *f, link hd)
{
    link p = malloc(sizeof(struct nodePg_t));

    fscanf(f, "%s %s %s %d %d %d %d %d %d\n", p->p.codice, p->p.nome, p->p.classe, &p->p.stat.hp,
            &p->p.stat.mp, &p->p.stat.atk, &p->p.stat.def, &p->p.stat.mag, &p->p.stat.spr);

    if (!controllo_codice(p->p.codice, hd)) {
        free(p);
        return NULL;
    }

    return p;
}

void funzione_carica_p(tabPg_t *tabPg, int *f) {
    FILE *fin;
    char nome[L];

    printf("Inserire nome file: ");
    scanf("%s", nome);

    /* verifica se presente file da leggere*/
    if (!apri_file(nome, &fin))
        return;

    /* lettura numero personaggi */
    fscanf(fin, "%d", &tabPg->nPg);

    /* ciclo acquisizione lista */
    link *x;
    int i, cnt = 0;
    for (i = 0, x = &tabPg->headPg; i < tabPg->nPg; i++) {
        *x = nuovo_nodoPg(fin, tabPg->headPg);

        /* modiifchiamo il valore next dell'elemento precedente solo
         * se il controllo del codice del nuovo elemeno è andato a buon fine */
        if (*x != NULL) {
            /* quando aggiungiamo l'ultimo elemento modifichiamo il valore del
            * puntatore alla coda */
            if (i == tabPg->nPg - 1)
                tabPg->tailPg = *x;

            /* modifichiamo punattore valore campo next al campo next appena letto */
            x = &(*x)->next;
        } else {
            /* contiamo quanti valori abbiamo scartato*/
            cnt++;
        }

    }

    /* ultimo elemento punta a NULL */
    *x = NULL;

    /* aggiorna il valore di personaggi presenti */
    tabPg->nPg += cnt;

    /* indica che abbiamo caricato i personaggi in lista */
    *f = 2;
}


void selezionaComando(comando_t e, tabPg_t *tabPg, tabInv_t *tabInv, int *f)
{
    switch (e) {
        case caricare_p:
            funzione_carica_p(tabPg, f);
            break;
        case caricare_o:
            break;
        case aggiungere_p:
            break;
        case aggiungere_o:
            break;
        case eliminare_p:
            break;
        case eliminare_o:
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

int main() {

    tabPg_t *tabPg;
    tabInv_t *tabInv;

    comando_t e;
    int flag = 1;

    while (flag != 0) {
        e = leggiComando();
        selezionaComando(e, tabPg, tabInv, &flag);
    }


    return 0;
}