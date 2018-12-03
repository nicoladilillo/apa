#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include "oggetti.h"
#include "personaggi.h"

typedef enum {
    caricare_p, caricare_o, aggiungere_p, eliminare_p, aggiungere_o, eliminare_o, calcolo,
    ricerca_p, ricerca_p_statistiche, ricerca_o, fine, errore
} comando_t;


/**
 * PARTE MENU'
 */

comando_t leggiComando(void)
{
    char *t[errore] = {
            "caricare_p", "caricare_o", "aggiungere_p", "eliminare_p",
            "aggiungere_o", "eliminare_o", "calcolo", "ricerca_p",
            "ricerca_p_statistiche", "ricerca_o", "fine"
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


/**
 * FUNZIONE FINE
 */

void funzione_fine(tabPg_t *tabPg, tabInv_t *tabInv)
{
    /* Disalloca personaggi */
    link app, x = tabPg->headPg;
    while(x != NULL)
    {
        app = x->next;
        free(x);
        x = app;
    }
    free(tabPg);

    /* Disalloca inventario */
    free(tabInv->vettInv);
    free(tabInv);
}


/**
 * MENU'
 */

void selezionaComando(comando_t e, tabPg_t *tabPg, tabInv_t *tabInv, int *f)
{
    switch (e) {
        case caricare_p:
            funzione_carica_p(tabPg);
            break;
        case caricare_o:
            funzione_carica_o(tabInv);
            break;
        case aggiungere_p:
            funzione_aggiungi_p(tabPg);
            break;
        case eliminare_p:
            funzione_elimina_p(tabPg);
            break;
        case aggiungere_o:
            funzione_aggiungi_o(tabPg, tabInv);
            break;
        case eliminare_o:
            funzione_elimina_o(tabPg);
            break;
        case calcolo:
            funzione_calcola_statistiche(tabPg);
            break;
        case ricerca_p:
            funzione_ricerca_p(tabPg);
            break;
        case ricerca_p_statistiche:
            funzione_ricerca_p_statistiche(tabPg);
            break;
        case ricerca_o:
            ricerca_oggetto(tabInv);
            break;
        case fine:
            funzione_fine(tabPg, tabInv);
            printf("Fine esecuzione!\n");
            *f = 0;
            break;
        case errore:
        default:
            errore_comando("comando");
            break;
    }
}

/* azzera valori dei wrapper degli elementi */
void azzera(tabPg_t *tabPg, tabInv_t *tabInv)
{
    tabPg->nPg = 0;
    tabPg->headPg = NULL;
    tabPg->tailPg = NULL;
    tabInv->nInv = 0;
    tabInv->vettInv = NULL;
}

int main(void) {

    tabPg_t *tabPg = malloc(sizeof *tabPg);
    tabInv_t *tabInv = malloc(sizeof *tabInv);
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