#ifndef ES01_GRAPH_H
#define ES01_GRAPH_H

typedef struct graph_t* Graph;

Graph GRAPHinit();
void GRAPHread(Graph G);
void GRAPHbecameDAG(Graph G);

#endif //ES01_GRAPH_H
