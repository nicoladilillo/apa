//
// Created by nicola on 12/01/19.
//

#include <stdlib.h>
#include <stdio.h>

#include "collezione_giornaliera.h"
#include "quotazione_giornaliera.h"

#define S 2 // soglia

typedef struct BSTnode* link;
struct BSTnode { Quotazione item; link r; link l; int nodi};
struct collezione { link root; link z; };

/* creazione nuovo nodo albero */
static link NEW(Quotazione q, link r, link l)
{
    link x = malloc(sizeof*x);
    x->item = q;
    x->r = r;
    x->l = l;

    return x;
}

/* inizializziamo la nostra collezione di quotazioni giornaliere */
Collezione COLLEZIONE_Qinit()
{
    Collezione bst = malloc(sizeof(Collezione));
    bst->root = bst->z = NEW(QUOTAZIONEsetNull(), NULL, NULL);


    return bst;
}

static link changeMinMax(link h, link new)
{
    if (new == NULL)
        return h;

    if (h->item.min > new->item.min)
        h->item.min = new->item.min;

    if (h->item.max < new->item.max)
        h->item.max = new->item.max;

    return h;
}

/* inserimento ricorsivo */
static link Insert(Quotazione q, link h, link z)
{
    if (h == z) {
        return NEW(q, z, z);
    }

    if (DATAlower(q.d, h->item.d)) {
        h->l = Insert(q, h->l, z);
        changeMinMax(h, h->l);
    }  else {
        h->r = Insert(q, h->r, z);
        changeMinMax(h, h->r);
    }

    (h->nodi)++;

    return h;
}

static link searchData(Data d, link h, link z)
{
    if(h == z)
        return NULL;

    if(DATAlower(d, h->item.d))
        return searchData(d, h->l, z);
    else if(DATAgreater(d, h->item.d))
        return searchData(d, h->r, z);

    return  h;
}

static link modifyQuotazione(Data d, link h, link z, float v, int n, int *x)
{
    if(h == z)
        return NULL;

    /* confrontiamo se i nodi di ritorno abbiamo min o max migliorati */
    if(DATAlower(d, h->item.d))
        return changeMinMax(h, modifyQuotazione(d, h->l, z, v, n, x));
    else if(DATAgreater(d, h->item.d))
        return changeMinMax(h, modifyQuotazione(d, h->r, z, v, n, x));
    /* se abbiamo trovato il nostro nodo */
    else if (DATAequal(d, h->item.d)) {
        h->item = QUOTAZIONEelabora(h->item, v, n);
        /* se è una foglia modifica entrambi valori di min e max */
        if (h->l == z && h->r == z)
            h->item.min = h->item.max = h->item.q;
        else {
            if (h->item.min > h->item.q)
                h->item.min = h->item.q;
            if (h->item.max < h->item.q)
                h->item.max = h->item.q;
        }
        *x = 1;
    }

    return  h;
}

/* inserimento nodo nell'albero */
void COLLEZIONE_Qinsert(Collezione bst, float valore, int numero, Data d)
{
    int flag = 0;
    modifyQuotazione(d, bst->root, bst->z, valore, numero, &flag);

    if (flag == 0) {
        // inizializzaione quotazione da inserire
        Quotazione q = QUOTAZIONEinit(valore, numero, d);
        bst->root = Insert(q, bst->root, bst->z);
    }
}

void COLLEZIONE_QsearchData(Collezione bst)
{
    Data d;
    printf("inserire data [aaa/mm/gg]: ");
    scanf("%d/%d/%d", &d.a, &d.m, &d.g);

    link x = searchData(d, bst->root, bst->z);
    if (x != NULL)
        printf("Quotazione: %f\n", x->item.q);
    else
        printf("Data non trovata\n");
}

static void searchDateIntervalli(Data d_min, Data d_max, link h, link z, link *min, link *max)
{
    if(h == z)
        return;

    /* se la data è compresa o uguale all'intervallo scelto */
    if ((DATAgreater(d_min, h->item.d) && DATAlower(d_max, h->item.d)) ||
        (DATAequal(d_min, h->item.d) || DATAequal(d_max, h->item.d))) {
        if (*max == z) {
            *min = *max = h;
        } else {
            if ((*min)->item.q > h->item.q)
                *min = h;
            if ((*max)->item.q < h->item.q)
                *max = h;
        }

        searchDateIntervalli(d_min, d_max, h->l, z, min, max);
        searchDateIntervalli(d_min, d_max, h->r, z, min, max);
        return;
    }

    if (DATAgreater(h->item.d, d_max))
        searchDateIntervalli(d_min, d_max, h->l, z, min, max);

    if (DATAlower(h->item.d, d_min))
        searchDateIntervalli(d_min, d_max, h->r, z, min, max);
}

static float Max(link h)
{
    return h->item.max;
}

static float Min(link h) {
    return h->item.min;
}

void COLLEZIONE_QsearchIntervallDate(Collezione bst)
{
    Data d1, d2;
    printf("inserire prima data [aaa/mm/gg]: ");
    scanf("%d/%d/%d", &d1.a, &d1.m, &d1.g);
    printf("inserire seconda data [aaa/mm/gg]: ");
    scanf("%d/%d/%d", &d2.a, &d2.m, &d2.g);

    link min, max;
    // assegnazione elementi di partenza
    min = max = bst->z;
    searchDateIntervalli(d1, d2, bst->root, bst->z, &min, &max);

    if(max == bst->z)
        printf("Non abbiamo date in questi intervalli\n");
    else {
        printf("Valore minimo e' %f in data %d/%d/%d\n", min->item.q, min->item.d.g, min->item.d.m, min->item.d.a);
        printf("Valore massimo e' %f in data %d/%d/%d\n", max->item.q, max->item.d.g, max->item.d.m, max->item.d.a);
    }

}


void COLLEZIONEsearchMinMax(Collezione bst)
{
    if (bst->root == bst->z) {
        printf("Non sono presenti quotazioni\n");
        return;
    }

    float max = Max(bst->root);
    float min = Min(bst->root);

    printf("Valore minimo e' %f\n", min);
    printf("Valore massimo e' %f\n", max);
}

static float max(float v1, float v2, float v3)
{
    if ((v1 > v2) && (v1 > v3))
        return v1;
    else if((v2 > v3))
        return v2;
    else
        return v3;
}

float static min(float v1, float v2, float v3)
{
    if ((v1 < v2) && (v1 < v3))
        return v1;
    else if((v2 < v3))
        return v2;
    else
        return v3;
}

static void update(link x, link h)
{
    x->nodi = h->nodi;
    h->nodi = h->r->nodi + h->l->nodi+1;

    h->item.max = max(h->item.q, h->l->item.max, h->r->item.max);
    h->item.min = max(h->item.q, h->l->item.min, h->r->item.min);

    x->item.max = max(x->item.q, x->l->item.max, x->r->item.max);
    h->item.min = max(x->item.q, x->l->item.min, x->r->item.min);
}

static link rotR(link h)
{
    link x = h->l;
    h->l = x->r;
    x->r = h;

    update(x, h);

    return x;
}

static link rotL(link h)
{
    link x = h->r;
    h->r = x->l;
    x->l = h;

    update(x, h);

    return x;
}

static link partition(link h, int r)
{
    int t = h->l->nodi;
    if (t > r) {
        h->l = partition(h->l, r);
        h = rotR(h);
    }
    if (t < r) {
        h->r = partition(h->r, r-t-1);
        h = rotL(h);
    }

    return h;
}

static link FindMinMaxCammino(link h, link z, int *min, int *max, int n, int *tot)
{
    // terminazione
    if (h == z)
        return h;

    if (h->l == z && h->r == z) {
        (*tot)++;
        if (n > 0) {
            if (n < *min)
                *min = n;
            if (n > *max)
                *max = n;
        }
        return h;
    }

    FindMinMaxCammino(h->l, z, min, max, n+1, tot);
    FindMinMaxCammino(h->r, z, min, max, n+1, tot);

    return h;
}

void COLLEZIONEpartition(Collezione bst)
{
    int min, max;
    min = 10000;
    max = 0;
    int tot = 0;
    FindMinMaxCammino(bst->root, bst->z, &min, &max, 0, &tot);
    if (tot == 1)
        min = 0;
    /* se sopra soglia effettua partizione */
    if (min == 0 || (float)max/min > S) {
        int t;
        t = (bst->root->nodi)/2; // nodo che diventerà nuova radice
        bst->root = partition(bst->root, t);
    } else
        printf("L'albero ha un rapporto cammino massimo/cammino minimo sotto la soglia\n");

}