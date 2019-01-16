//
// Created by nicola on 12/01/19.
//

#ifndef ES01_COLLEZIONE_TITOLI_H
#define ES01_COLLEZIONE_TITOLI_H

typedef struct collezionetitoli *CollezioneTitoli;
CollezioneTitoli COLLEZIONE_Tinit();
void COLLEZIONE_TinsertQuotazioni(CollezioneTitoli c, FILE *f);
void COLLEZIONETsearchTitolo(CollezioneTitoli c);
void COLLEZIONETsearchQuotazione(CollezioneTitoli c);
void COLLEZIONETsearchQuotazioneDate(CollezioneTitoli c);
void COLLEZIONETsearchMinMax(CollezioneTitoli c);
void COLLEZIONETBilancia(CollezioneTitoli c);

#endif //ES01_COLLEZIONE_TITOLI_H
