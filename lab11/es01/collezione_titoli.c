//
// Created by nicola on 12/01/19.
//

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "utility.h"
#include "collezione_titoli.h"
#include "classe_titoli.h"

typedef struct nodo* link;

struct collezionetitoli {
    int n;
    link h;
};

struct nodo {
    ClasseTitolo p;
    link next;
};

CollezioneTitoli COLLEZIONE_Tinit()
{
    CollezioneTitoli c = malloc(sizeof(CollezioneTitoli));
    c->n = 0;
    c->h = NULL;

    return c;
}

static link NEW(char *str, link h)
{
    link x = malloc(sizeof(link));
    x->next = h;

    x->p = TITOLOinit(str);

    return x;
}

/* inserisci nuovo titolo in lista */
static link COLLEZIONE_Tinsert(CollezioneTitoli c, char *nome)
{
    link x, p;
    c->n++;

    if (c->h == NULL || TITOLOlower(c->h->p, nome)) {
        c->h = NEW(nome, c->h);
        return c->h;
    }

    for (x = c->h, p = x->next; p != NULL && (TITOLOgreater(x->p, nome) || TITOLOequal(x->p, nome)); x = p, p = p->next);
    x->next = NEW(nome, p);
    return x->next;
}

/* ritorna Titolo cercato e nel caso non fosse presente lo inserisce*/
static link searchAndInsert(CollezioneTitoli  c, char *nome)
{
    link x;

    for (x = c->h; x != NULL && (TITOLOgreater(x->p, nome) || TITOLOequal(x->p, nome)); x = x->next)
        if (TITOLOequal(x->p, nome))
            return x;

    return COLLEZIONE_Tinsert(c, nome);
}

/* ritorna Titolo cercato */
static link search(link h)
{
    char nome[N];
    printf("Inserire nome titolo: ");
    scanf("%s", nome);

    link x;
    for (x = h; x != NULL && (TITOLOgreater(x->p, nome)); x = x->next);
    /* se il seguente titolo non è uguale allora il titolo non è presente in lista */
    if (TITOLOequal(x->p, nome))
        return x;

    return NULL;
}

/* inserire quotazione in un titolo */
void COLLEZIONE_TinsertQuotazioni(CollezioneTitoli c, FILE *f)
{
    char nome[N];
    int n;
    link x;

    while(fscanf(f, "%s %d", nome, &n) > 0) {
        x = searchAndInsert(c, nome);
        TITOLOaddQuotazione(x->p, f, n);
    }
}

/* cerca titolo */
void COLLEZIONETsearchTitolo(CollezioneTitoli c)
{
    link x = search(c->h);
    if (x != NULL)
        printf("%s\n", TITOLOgetnome(x->p));
    else
        printf("Nessun titolo presente con quel nome\n");
}

/* cerca quotzione titolo in una certa data */
void COLLEZIONETsearchQuotazione(CollezioneTitoli c)
{
    link x = search(c->h);
    if (x != NULL)
        TITOLOsearchQuotazione(x->p);
    else
        printf("Nessun titolo presente con quel nome\n");
}

/* cerca quotzione titolo in una certo interallo di date */
void COLLEZIONETsearchQuotazioneDate(CollezioneTitoli c)
{
    link x = search(c->h);
    if (x != NULL)
        TITOLOsearchQuotazioni(x->p);
    else
        printf("Nessun titolo presente con quel nome\n");
}

/* cerca quotazioni minime e massime di un titolo */
void COLLEZIONETsearchMinMax(CollezioneTitoli c)
{
    link x = search(c->h);
    if (x != NULL)
        TITOLOsearchMinMax(x->p);
    else
        printf("Nessun titolo presente con quel nome\n");
}

/* cerca quotazioni minime e massime di un titolo */
void COLLEZIONETBilancia(CollezioneTitoli c)
{
    link x = search(c->h);
    if (x != NULL)
        TITOLOpartizione(x->p);
    else
        printf("Nessun titolo presente con quel nome\n");
}