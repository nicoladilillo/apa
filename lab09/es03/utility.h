#ifndef ES03_UTILITY_H
#define ES03_UTILITY_H

#define N 50+1 // 50 caratteri più tappo
#define L 20+1 // per comandi generali

/* settupla di statische personaggio e oggetti */
typedef struct {
    int hp, mp, atk, def, mag, spr;
} stat_t;

void errore_comando(char *str);
int apri_file(FILE **fp);

#endif //ES03_UTILITY_H
