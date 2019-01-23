#include <stdio.h>

#include "graph.h"

int main() {

    Graph G = GRAPHinit();

    GRAPHread(G);

    GRAPHbecameDAG(G);

    return 0;
}