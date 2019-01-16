//
// Created by nicola on 12/01/19.
//

#ifndef ES01_COLLEZIONE_GIORNALIERA_H
#define ES01_COLLEZIONE_GIORNALIERA_H

#include "data.h"

typedef struct collezione *Collezione;

Collezione COLLEZIONE_Qinit();
void COLLEZIONE_Qinsert(Collezione bst, float valore, int numero, Data d);
void COLLEZIONE_QsearchData(Collezione bst);
void COLLEZIONE_QsearchIntervallDate(Collezione bst);
void COLLEZIONEsearchMinMax(Collezione bst);
void COLLEZIONEpartition(Collezione bst);

#endif //ES01_COLLEZIONE_GIORNALIERA_H
