//
// Created by nicola on 12/01/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utility.h"
#include "classe_titoli.h"
#include "collezione_giornaliera.h"
#include "data.h"

struct classeTitolo {
    char nome[N];
    Collezione c;
};

ClasseTitolo TITOLOinit(char *nome)
{
    ClasseTitolo c = malloc(sizeof(ClasseTitolo));
    strcpy(c->nome, nome);
    c->c = COLLEZIONE_Qinit();

    return c;
}

char *TITOLOgetnome(ClasseTitolo c)
{
    return c->nome;
}

int TITOLOgreater(ClasseTitolo c, char *nome)
{
    if (strcmp(c->nome, nome) < 0)
        return 1;
    return 0;
}

int TITOLOlower(ClasseTitolo c, char *nome)
{
    if (strcmp(c->nome, nome) > 0)
        return 1;
    return 0;
}

int TITOLOequal(ClasseTitolo c, char *nome)
{
    if (strcmp(c->nome, nome) == 0)
        return 1;
    return 0;
}

/* aggiungiamo quotazini nella nostra collezione di quotazioni
 * o aggiorniamo quotazioni gia presenti */
void TITOLOaddQuotazione(ClasseTitolo c, FILE *fin, int max)
{
    Data d;
    float v;
    int n;

    int i;
    for (i = 0; i < max; i++)
    {
        fscanf(fin, "%d/%d/%d %*d:%*d %f %d\n", &d.a, &d.m, &d.g, &v, &n);
        COLLEZIONE_Qinsert(c->c, v, n, d);

    }
}

/* ricerca a quotazioe di un titolo in una certa data */
void TITOLOsearchQuotazione(ClasseTitolo c)
{
    COLLEZIONE_QsearchData(c->c);
}

/* ricerca a quotazione massima e mimima di un titolo*/
void TITOLOsearchMinMax(ClasseTitolo c)
{
    COLLEZIONEsearchMinMax(c->c);
}

/* ricerca a quotazioe di un titolo in una certa intervallo di date */
void TITOLOsearchQuotazioni(ClasseTitolo c)
{
    COLLEZIONE_QsearchIntervallDate(c->c);
}

void TITOLOpartizione(ClasseTitolo c)
{
    COLLEZIONEpartition(c->c);
}
