//
// Created by nicola on 12/01/19.
//

#ifndef ES01_CLASSE_TITOLI_H
#define ES01_CLASSE_TITOLI_H

typedef struct classeTitolo *ClasseTitolo;

ClasseTitolo TITOLOinit(char *nome);
char *TITOLOgetnome(ClasseTitolo c);
int TITOLOgreater(ClasseTitolo c, char *nome);
int TITOLOlower(ClasseTitolo c, char *nome);
int TITOLOequal(ClasseTitolo c, char *nome);
void TITOLOaddQuotazione(ClasseTitolo c, FILE *fin, int max);
void TITOLOsearchQuotazione(ClasseTitolo c);
void TITOLOsearchQuotazioni(ClasseTitolo c);
void TITOLOsearchMinMax(ClasseTitolo c);
void TITOLOpartizione(ClasseTitolo c);


#endif //ES01_CLASSE_TITOLI_H
