#include <stdio.h>
#include <stdlib.h>

#include "graph.h"


int main(int argc, char **argv) {

    Graph g = GRAPHcreate(argv[1]);

    GRAPHprint_vertici(g);
    GRAPHvertici_adiacenti(g);
    GRAPHinitlafj(g);
    GRAPHvertici_adiacenti_lafj(g);

    return 0;
}