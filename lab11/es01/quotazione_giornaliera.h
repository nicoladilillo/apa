//
// Created by nicola on 12/01/19.
//

#ifndef ES01_QUOTAZIONE_GIORNALIERA_H
#define ES01_QUOTAZIONE_GIORNALIERA_H

#include "data.h"

typedef struct {
    Data d;
    float q;
    int n;
    float min, max;
} Quotazione;

Quotazione QUOTAZIONEsetNull();
Quotazione QUOTAZIONEinit(float v, int n, Data d);
int QUOTAZIONEconfronto(Quotazione q1, Quotazione q2);
Quotazione QUOTAZIONEelabora(Quotazione q, float v, int n);

#endif //ES01_QUOTAZIONE_GIORNALIERA_H
