#include <stdio.h>
#include <stdlib.h>

#include "utility.h"
#include "st.h"
#include "graph.h"

typedef struct node* link;

struct node {
    int v;
    link next;
};

struct graph {
    int V;
    int E;
    ST st;
    link *ladj;
    link z;
};

static link NEW(int v, link next) {

    link x = malloc(sizeof(link));

    x->v = v;
    x->next = next;

    return x;
}

Graph GRAPHinit() {

    Graph g = malloc(sizeof(Graph));
    g->E = g->V = 0;
    g->st = STinit();
    g->z = NEW(-1, NULL);

    return g;
}

Graph GRAPHload(char *name) {

    FILE *fp = apriFile(name, "r");
    Graph G = GRAPHinit();

    char label1[N], label2[N];
    int i, j;
    while(fscanf(fp, "%s %s\n", label1, label2) == 2) {
        i = STsearch(G->st, label1);
        if (i == -1)
            STinsert(G->st, label1);

        j = STsearch(G->st, label2);
        if (j == -1)
            STinsert(G->st, label2);
        G->E++;
    }
    fclose(fp);
    G->V = STcount(G->st);

    G->ladj = malloc(G->V* sizeof(link));
    // inizializzazione nodo sentinella
    for (i = 0; i < G->V; i++) G->ladj[i] = G->z;

    int k;
    FILE *fin = apriFile(name, "r");
    for (k = 0; k < G->E; k++) {
        fscanf(fin, "%s %s\n", label1, label2);
        i = STsearch(G->st, label1);
        j = STsearch(G->st, label2);
        G->ladj[i] = NEW(j, G->ladj[i]); // inserimento in testa
    }

    return G;
}

int GRAPHgetIndex(Graph G, char *label) {
    return STsearch(G->st, label);
}

/*
 * controlliamo che ogni vertici esterno al kernel possieda almeno un
 * arco entrante con estremo un vertice appartenente al kernel
 */
static int checkView(Graph G, int *v, int n) {

    int i;
    link x;
    int m[G->V];

    for (i = 0; i < G->V; i++)
        m[i] = 0;

    // marchiamo i vertici raggiunti dai nostri nodi all'interno del kernel
    for (i = 0; i < n; i++) {
        m[v[i]] = 1;
        for (x = G->ladj[v[i]]; x != NULL; x = x->next)
            m[x->v] = 1;
    }

    // controlliamo che siano marcati tutti i vertici
    for (i = 0; i < n; i++)
        if (!m[i])
            return 0;

    return 1;
}

int GRAPHcheckKernel(Graph G, int *v, int n) {

    int i, j;
    link x;

    for (i = 0; i < n; i++) {
        for (x = G->ladj[v[i]]; x != G->z; x = x->next) {
            for (j = 0; j < n; j++)
                if (x->v == v[j]) return 0;
        }
    }

    if(!checkView(G, v, n))
        return 0;
    
    return 1;
}

static int promising(Graph G, int *v, int pos, int w) {

    int i;
    link x;

    // il nostro vertice considerato non possiede archi verso nessun nodo all'interno del kernel
    for (x = G->ladj[w]; x != G->z; x = x->next)
        for (i = 0; i < pos; i++)
            if (v[i] == x->v) return 0;

    // nessun nodo all'interno del kernel possiede archi verso il nostro vettore considerato
    for (i = 0; i < pos; i++)
        for (x = G->ladj[v[i]]; x != G->z; x = x->next)
            if (w == x->v) return 0;

    return 1;
}

static int comb_semplici(Graph G, int pos, int start, int *v, int max) {

    if (pos >= max) {
        if (checkView(G, v, max))
            return 1;
        return 0;
    }

    int i;
    for (i = start; i < G->V; i++) {
        if (promising(G, v, pos, i)) {
            // aggiungiamo il nostro vertice all'ipotetico insieme kernel
            v[pos] = i;
            if (comb_semplici(G, pos + 1, i + 1, v, max))
                return 1;
        }
    }

    return 0;
}

static int isKernel(int *v, int n, int w) {
    int i;

    for (i = 0; i < n; i++)
        if (w == v[i])
            return 1;

    return 0;
}

static void dfsR(Graph G, int *st, int *max, int cnt, int w, int *v, int n) {
    st[w] = 0;
    if (isKernel(v, n, w)) {
        cnt++;
        if (cnt > *max)
            *max = cnt;
    }
    link x;
    for (x = G->ladj[w]; x != G->z; x = x->next)
        if (st[x->v] == -1)
            dfsR(G, st, max, cnt, x->v, v, n);

    st[w] = -1;
}

static int CamminiMinimi (Graph G, int *v, int n) {
    int max = 0;
    int st[G->V];
    int i;
    for (i = 0; i < G->V; i++) st[i] = -1;

    for (i = 0; i < G->V; i++)
        dfsR(G, st, &max, 0, i, v, n);

    return max;
}

static void stampa(Graph G, int *v, int n, char *name, int max) {
    FILE *fout = apriFile(name, "w");

    int i;
    for (i = 0; i < n; i++)
        fprintf(fout, "%s\n", STsearchByIndex(G->st, v[i]));
    fprintf(fout, "Massimo nodi kernel in cammino: %d", max);
}

void GRAPHminKernel(Graph G, char *nome) {
    int i;
    int v[G->V];
    for (i = G->V; i > 0; i--) {
        if (comb_semplici(G, 0, 0, v, i)) {
            stampa(G, v, i, nome, CamminiMinimi(G, v, i));
            return;
        }
    }

    printf("Non è presente alcun kernel\n");
}

void GRAPHfree(Graph G) {
    link x, delete;
    int i;
    for (i = 0; i < G->V; i++)
        for (delete = G->ladj[i]; delete != G->z; delete = x ) {
            x = delete->next;
            free(delete);
        }
    free(G->ladj);
    free(G->z);
    STfree(G->st);
    free(G);
}

