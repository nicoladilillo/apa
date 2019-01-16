#ifndef ES01_ITEM_H
#define ES01_ITEM_H

#include <stdio.h>

#define N 50+1
#define D 5 // possiamo avere massimo 5 elementi in una diagonale
#define B 3 // numero di diagonali da calcolare

typedef enum{
    transazione, avanti, indietro
} tipologia_t;

typedef enum{
    spalle, frontale
} direzione_t;

typedef struct {
    char nome[N+1];
    tipologia_t tipologia;
    direzione_t ingresso, uscita;
    int precedenza, finale;
    float valore;
    int difficolta;
    int fissato[B]; // servirà per le disposiszione ripetute
} Item_t;

typedef struct {
    Item_t *item;
    int n;
    int DD;
    float DP;
} Item_wrappper;

Item_wrappper ITEMcarica(FILE *fin);
void ITEMdealloca(Item_wrappper p);
int ITEMislibero(Item_t p, int i);
void ITEMfissa(Item_t *p, int i);
void ITEMlibera(Item_t *p, int i);
int ITEMisfrontale(Item_t p);
int ITEMisfinale(Item_t p);
int ITEMprimo(Item_t p);
int ITEMavanti(Item_t p);
int ITEMindietro(Item_t p);
int ITEMsequenza(Item_t p1, Item_t p2);

#endif //ES01_ITEM_H
