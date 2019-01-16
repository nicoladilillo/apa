#include <stdio.h>

#include "utility.h"
#include "collezione_titoli.h"

typedef enum {
    acquisizsci, titolo, data, date, massimo, partizionamento, esci, errore
} comandi_t;

void funzione_inserisci(CollezioneTitoli collezioni)
{
    char nome[N];
    printf("\nInserire nome file: ");
    scanf("%s", nome);

    /* apertura file */
    FILE *fp = NULL;
    fp = fopen(nome, "r");
    if(fp == NULL) {
        printf("Errore apertura file!\n");
        return;
    }

    COLLEZIONE_TinsertQuotazioni(collezioni, fp);

}

comandi_t leggiComando()
{
    char *comandi[errore] = {
            "acquisizione", "cerca titolo", "quotazione data",
            "quotazioni in intervallo date", "quotazioni massime e minime",
            "partiziona albero", "esci"
    };

    int i, cmd;
    for (i = 0; i < errore; i++)
        printf("\n%d. %s", i+1, comandi[i]);
    printf("\nInserisci comando[1 - %d]: ", errore);
    scanf("%d", &cmd);
    cmd--;

    comandi_t c = acquisizsci;
    while (c < errore && c != cmd)
        c++;

    return c;
}

void elaborazioneComando(comandi_t c, int *flag, CollezioneTitoli collezioni)
{
    switch (c) {
        case acquisizsci:
            funzione_inserisci(collezioni);
            break;
        case titolo:
            COLLEZIONETsearchTitolo(collezioni);
            break;
        case data:
            COLLEZIONETsearchQuotazione(collezioni);
            break;
        case date:
            COLLEZIONETsearchQuotazioneDate(collezioni);
            break;
        case massimo:
            COLLEZIONETsearchMinMax(collezioni);
            break;
        case partizionamento:
            COLLEZIONETBilancia(collezioni);
            break;
        case esci:
            *flag = 0;
            break;
        case errore:
        default:
            printf("Errore comando");
            break;

    }
}

int main() {

    int flag = 1;
    comandi_t c;
    CollezioneTitoli collezione = COLLEZIONE_Tinit();

    while (flag) {
        c = leggiComando();
        elaborazioneComando(c, &flag, collezione);
    }

    return 0;
}