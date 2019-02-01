#include "utility.h"

FILE *apriFile(char *n, char *cmd) {
    FILE *fp = NULL;
    fp = fopen(n, cmd);
    if (fp == NULL) {
        printf("Errore apertura file\n");
        exit(-1);
    }

    return fp;
}
