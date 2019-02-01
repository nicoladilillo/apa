#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "st.h"
#include "item.h"

#define N 30+1

typedef struct node* link;

struct node {
    Item p;
    link next;
};

struct graph_t {
    int E; // numero archi
    int V; // numero vertici
    ST st;
    link *ladj;
    link *DAG;
    link z; // sentinella
};

static link NEW(Item p, link next) {
    link x = malloc(sizeof *x);

    x->next = next;
    x->p = p;

    return x;
}

Graph GRAPHinit() {
    Graph G = (Graph)malloc(sizeof(Graph));
    G->E = 0;
    G->z = NEW(ITEMsetnull(), NULL);

    return G;
}

void GRAPHread(Graph G) {
    char name[N];
    int i;

    printf("Inserire nome file: ");
    scanf("%s", name);

    FILE *fp = NULL;
    fp = fopen(name, "r");
    if (fp == NULL) {
        printf("Errore apertura file\n");
        exit(-1);
    }

    // lettura numero vertici
    fscanf(fp, "%d\n", &G->V);
    // inizializzazione tabella di simboli
    G->st = STinit(G->V);
    // inizializzazione lista delle adicenze
    G->ladj = malloc(G->V*sizeof(link));

    // inserimento vertici in tabella di simboli
    for (i = 0; i < G->V; i++) {
        fscanf(fp, "%s\n", name);
        STinsert(G->st, name);
        G->ladj[i] = G->z;
    }

    char n1[N], n2[N];
    int wt, j;
    // lettura archi
    while (fscanf(fp, "%s %s %d\n", n1, n2, &wt) == 3) {
        i = STgetindex(G->st, n1);
        j = STgetindex(G->st, n2);

        // inserisci arco in lista delle adiacenze
        G->ladj[i] = NEW(ITEMcreate(j, wt),G->ladj[i]);
        G->E++;
    }
}

static void dfsR(Graph G, int *st, int *pre, int *post, int *time, int v, int w, int *f)
{
    link t;

    st[w] = v;
    pre[w]= (*time)++;
    for(t = G->ladj[w]; t != G->z; t = t->next) {
        if (!t->p.preso)
            printf("(%s, %s) ", STselect(G->st, w), STselect(G->st, t->p.v));
        else if(st[t->p.v] == -1)
            dfsR(G, st, pre, post, time, w, t->p.v, f);
        // identificazione arco backward per non far valere aciclicità
        else if (post[t->p.v] == -1)
            *f = 0;
    }

    post[w] = (*time)++;
}

static int isAcicliclo(Graph G) {
    int st[G->V], pre[G->V], post[G->V];
    int i;
    int flag = 1;
    int time = 0;

    for (i = 0; i < G->V; i++) {
        post[i] = pre[i] = st[i] = -1;
    }

    printf("G \\ { ");

    for (i = 0; i < G->V; i++)
        if (st[i] == -1)
            dfsR(G, st, pre, post, &time, i, i, &flag);

    printf("}\naciclico = ");
    return flag;
}

static void GRAPHcopy(Graph G) {
    int i;
    link x, *y;

    for (i = 0; i < G->V; i++)
        for (x = G->ladj[i], y = &G->DAG[i]; x != G->z; x = x->next) {
            *y = NEW(x->p, G->z);
            y = &(*y)->next;
        }
}

static void DAGcopy(Graph G) {
    int i;
    link x, y;

    for (i = 0; i < G->V; i++)
        for (x = G->ladj[i], y = G->DAG[i]; x != G->z; x = x->next, y = y->next)
            y->p.preso = x->p.preso;
}

static void combinazionisemplici(Graph G, int c, int cnt, link start, int v, int *f, int wt, int *wtmax, link z) {
    // terminazione
    if (c == cnt) {
        if(isAcicliclo(G)) {
            printf("si\n");
            *f = 0;
            if (wt > *wtmax) {
                DAGcopy(G);
                *wtmax = wt;
            }
        } else
            printf("no\n");
        return;
    }

    link x;
    for (x = start; !(x == z && (v + 1)== G->V); x = x->next) {
        while (x == z) {
            x = G->ladj[++v];
            if(x == z && (v + 1)== G->V)
                return;
        }

        // consideriamo quell'arco non preso
        x->p.preso = 0;
        // incrementiamo il contatore degli archi tolti
        combinazionisemplici(G, c, cnt + 1, x->next, v, f, wt+x->p.wt, wtmax, z);
        x->p.preso = 1;
    }
}

static void DAGstampa(Graph G) {
    int i;
    link x;

    printf("DAG finale:\n");
    printf("G \\ { ");
    for (i = 0; i < G->V; i++)
        for (x = G->DAG[i]; x != G->z; x = x->next)
            if (!x->p.preso)
                printf("(%s, %s) ", STselect(G->st, i), STselect(G->st, x->p.v));

            printf("}\n\n");
}

static void calculateDistance(link *DAG, int *maxdist, int *st, link z, int ss) {
    link x;
    int i;

    for (i = ss; i >= 0; i--)
        for (x =  DAG[st[i]]; x != z; x = x->next)
            if (x->p.preso)
                if (maxdist[x->p.v] < maxdist[st[i]] + x->p.wt)
                    maxdist[x->p.v] = maxdist[st[i]] + x->p.wt;

}

static void ordinamentoTopologico(link *DAG, int v, int *pre, int *st, int *time, link z) {
    pre[v] = 0;
    link x;
    for (x = DAG[v]; x != z; x = x->next)
        if(x->p.preso && pre[x->p.v] == -1)
            ordinamentoTopologico(DAG, x->p.v, pre, st, time, z);

    st[(*time)++] = v;
}

static void DAGmaxDistance(Graph G) {
    int v = G->V;
    int i, j;
    int maxdist[v];
    int pre[v], st[v];

    for (j = 0; j < v; j++) { pre[j] = st[j] = -1; };

    // ordinamento topologico
    int time = 0;
    for (i = 0; i < v; i++)
        if (pre[i] == -1)
            ordinamentoTopologico(G->DAG, i, pre, st, &time, G->z);

    // Calcolo distanza massima da ogni nodo
    for (i = v-1; i >= 0; i--) {
        for (j = 0; j < G->V; j++) { maxdist[j] = 0; };
        calculateDistance(G->DAG, maxdist, st, G->z, i);
        printf("Start: %s\n", STselect(G->st, st[i]));
        for (j = 0; j < G->V; j++)
            if (j != st[i]) {
                printf("\t-> %s [", STselect(G->st, j));
                if (maxdist[j] == 0)
                    printf("non raggiungibile");
                else
                    printf("%d", maxdist[j]);
                printf("]\n");
            }
    }
}

void GRAPHbecameDAG(Graph G) {
    int i;
    int flag = 1;
    int wt_max = 0;

    // inizializzazione lista delle adicenze del DAG finale
    G->DAG = malloc(G->V* sizeof(link));
    // inizializzazione
    for (i = 0; i < G->V; i++) G->DAG[i] = G->z;
    // copiamo la lista delle adiacenze del grafo nel nostro DAG finale
    GRAPHcopy(G);

    for (i = 0; i < G->V && flag; i++) {
        printf("Generazione set di cardinalita' %d\n", i);
        combinazionisemplici(G, i, 0, G->ladj[0], 0, &flag, 0, &wt_max, G->z);
    }

    DAGstampa(G);

    DAGmaxDistance(G);
}

