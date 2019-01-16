//
// Created by nicola on 12/01/19.
//

#include <stdlib.h>

#include "quotazione_giornaliera.h"

Quotazione QUOTAZIONEsetNull()
{
    Quotazione x;
    x.q = 0;
    x.n = 0;
    return x;
}

int QUOTAZIONEconfronto(Quotazione q1, Quotazione q2)
{
    return DATAlower(q1.d, q2.d);
}

Quotazione QUOTAZIONEelabora(Quotazione q, float v, int n)
{
    q.q = ((q.q*(float)q.n)+(v*(float)n))/((float)(q.n+n));
    q.n += n;

    return q;
}

Quotazione QUOTAZIONEinit(float v, int n, Data d)
{
    Quotazione q;

    q.q = v/((float)n);
    q.d = d;
    q.n = n;
    q.min = q.q;
    q.max = q.q;

    return q;
}