//
// Created by nicola on 31/01/19.
//

#ifndef ESAME_GRAPH_H
#define ESAME_GRAPH_H

typedef struct graph *Graph;

Graph GRAPHinit();
Graph GRAPHload(char *name);
int GRAPHgetIndex(Graph G, char *label);
int GRAPHcheckKernel(Graph G, int *v, int n);
void GRAPHminKernel(Graph G, char *nome);
void GRAPHfree(Graph G);

#endif //ESAME_GRAPH_H
