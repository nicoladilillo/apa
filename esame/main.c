#include <stdio.h>
#include <stdlib.h>

#include "utility.h"
#include "graph.h"

int ReadKernel(char *name, int **v, Graph G) {
    int i, n = 0;
    int *m;
    FILE *fp = apriFile(name, "r");

    char label[N];
    while (fscanf(fp, "%s\n", label) == 1)
        n++;
    fclose(fp);

    FILE *fin = apriFile(name, "r");
    m = malloc(n* sizeof(int));
    for (i = 0; i < n; i++) {
        fscanf(fin, "%s\n", label);
        m[i] = GRAPHgetIndex(G, label);
    }
    fclose(fin);

    *v = m;
    return n;
}

int main(int argc, char **argv) {

    if (argc != 4) {
        printf("Errore parametri iniziali!\n");
        exit(-1);
    }

    Graph G = GRAPHload(argv[1]);

    int *v;
    int n = ReadKernel(argv[2], &v, G);

    if (GRAPHcheckKernel(G, v, n))
        printf("La combinazione e' un kernel\n");
    else
        printf("La combinazione non e' un kernel\n");
    free(v);

    GRAPHminKernel(G, argv[3]);

    GRAPHfree(G);

    return 0;
}