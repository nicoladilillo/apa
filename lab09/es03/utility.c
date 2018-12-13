#include <stdio.h>

#include "utility.h"

void errore_comando(char *str)
{
    printf("Errore %s!\n", str);
}

int apri_file(FILE **fp)
{
    char nome[L];

    printf("Inserire nome file: ");
    scanf("%s", nome);

    *fp = NULL;
    *fp = fopen(nome, "r");
    if (*fp == NULL) {
        errore_comando("apertura file");
        return 0;
    } else
        return 1;
}
