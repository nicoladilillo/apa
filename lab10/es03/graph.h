#ifndef ES03_GRAPH_H
#define ES03_GRAPH_H

// typedef struct {int v; int w; int wt} Edge;
typedef struct graph *Graph;

Graph GRAPHcreate(char *str);
void GRAPHprint_vertici(Graph G);
void GRAPHvertici_adiacenti(Graph G);
void GRAPHinitlafj(Graph G);
void GRAPHvertici_adiacenti_lafj(Graph G);

#endif //ES03_GRAPH_H
