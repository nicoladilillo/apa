
#include <stdio.h>
#include <stdlib.h>

#include "graph.h"
#include "st.h"

typedef struct node *link;

struct node {
    int v; // chiave vertice
    int wt; // peso arco con quel vertice
    link next;
};

struct graph {
    int V; // numero vertici
    int E; // numero archi
    int **madj; // matrice delle adiacenze
    ST st;
    link *ladj;
};

static FILE *apri_file(char *str)
{
    FILE *fp = NULL;
    fp = fopen(str, "r");
    if (fp == NULL) {
        printf("Errore apertura file!\n");
        exit(-1);
    }
}

static int **MATRICinit(int v)
{
    int **m = malloc(v* sizeof(int *));
    int i;
    for (i = 0; i < v; i++)
        m[i] = calloc(v, sizeof(int));

    return m;
}

Graph GRAPHcreate(char *str)
{
    FILE *f = apri_file(str);
    Graph G = malloc(sizeof(Graph));

    G->st = STinit(); // creazione tabella di simboli
    G->E = 0; // inizializzazione archi a zero

    char str1[D+1], str2[D+1], rete1[D+1], rete2[D+1];
    int flusso;

    /* contare vertici e riempire tabella di simboli */
    while(fscanf(f, "%s %s %s %s %*d", str1, rete1, str2, rete2) == 4) {
        STcheck(G->st, str1, rete1);
        STcheck(G->st, str2, rete2);
        G->E++; // incremento archi
    }
    fclose(f);

    f = apri_file(str);
    G->V = STget(G->st); // numero vertici pari a numero valori in tabella simboli
    G->madj = MATRICinit(G->V); // iniziallizazzione matrice adiacenze

    while(fscanf(f, "%s %*s %s %*s %d", str1, str2, &flusso) > 0) {
        G-> madj[STsearch(G->st, str1)][STsearch(G->st, str2)] = flusso;
        G->madj[STsearch(G->st, str2)][STsearch(G->st, str1)] = flusso;
    }
    fclose(f);

    // stampa matrice delle adiacenze
    /*int i, j;
    for (i = 0; i < G->V; i++) {
        for (j = 0; j < G->V; j++)
            printf("%d ", G->madj[i][j]);
        printf("\n");
    }*/

    return G;
}

void GRAPHprint_vertici(Graph G)
{
    int i, j;

    for (i = 0; i < G->V; i++) {
        printf("%s-> ", STprint(G->st, i));
        for (j = 0; j < G->V; j++)
            if (G->madj[i][j] > 0)
                printf("%s ", STprint(G->st, j));
            printf("\n");
    }
}

void GRAPHvertici_adiacenti(Graph G)
{
    /*char v1[D+1], v2[D+1], v3[D+1];
    printf("Inserire nome vertici: ");
    scanf("%s %s %s", v1, v2, v3);*/
    char *v3 = "Uriel";
    char *v2 = "Cassiel";
    char *v1 = "Tatooine";

    // chiave di ogni vertici identificato dal nome
    int n1 = STsearch(G->st, v1);
    int n2 = STsearch(G->st, v2);
    int n3 = STsearch(G->st, v3);

    if ((G->madj[n1][n2] > 0 || G->madj[n1][n3] > 0 ) && (G->madj[n3][n1]  > 0 || G->madj[n3][n2] > 0))
        printf("I tre vertici SONO CONNESSI\n");
    else
        printf("I tre vertici non sono connessi\n");
}

/* crea nuovo nodo */
static link NEW(int v, int wt, link next)
{
    link x;

    x = malloc(sizeof(link));
    x->v = v;
    x->wt = wt;
    x->next = next;

    return x;
}

/* stampa lista adiacenze */
static void GRAPGprintlafj(link *lafj, int v, ST st)
{
    int i;
    link x;

    for (i = 0; i < v; i++) {
        printf("%s-> ", STprint(st, i));
        for (x = lafj[i]; x != NULL; x = x->next)
            printf("%s ", STprint(st, x->v));
        printf("\n");
    }
}

/* inizializza lista adiacenze partendo dalla matrice */
void GRAPHinitlafj(Graph G)
{
    G->ladj = malloc(G->V * sizeof(link));

    int i, j, cnt;
    link app;
    for (i = 0; i < G->V; i++) {
        cnt = 0;
        for (j = 0; j < G->V; j++)
            if (G->madj[i][j] > 0) {
                if (cnt == 0) {
                    G->ladj[i] = NEW(j, G->madj[i][j], NULL);
                    app = G->ladj[i];
                } else {
                    app->next = NEW(j, G->madj[i][j], NULL);
                    app = app->next;
                }
                cnt++;
            }
    }

    //GRAPGprintlafj(G->ladj, G->V, G->st);
}

static int check(link *list, int v1, int v2)
{
    link app;
    for (app = list[v1]; app != NULL; app = app->next)
        if (app->v == v2)
            return 1;

    return 0;
}

void GRAPHvertici_adiacenti_lafj(Graph G) {
    /*char v1[D+1], v2[D+1], v3[D+1];
    printf("Inserire nome vertici: ");
    scanf("%s %s %s", v1, v2, v3);*/
    char *v3 = "Uriel";
    char *v2 = "Igni";
    char *v1 = "Cassiel";

    // chiave di ogni vertici identificato dal nome
    int n1 = STsearch(G->st, v1);
    int n2 = STsearch(G->st, v2);
    int n3 = STsearch(G->st, v3);

    if ((check(G->ladj, n1, n3) || check(G->ladj, n1, n2)) && (check(G->ladj, n3, n1) || check(G->ladj, n3, n2)))
        printf("I tre vertici SONO CONNESSI\n");
    else
        printf("I tre vertici non sono connessi\n");
}