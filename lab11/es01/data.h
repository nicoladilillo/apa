//
// Created by nicola on 12/01/19.
//

#ifndef ES01_DATA_H
#define ES01_DATA_H

typedef struct { int g, m, a } Data;

int DATAequal(Data d1, Data d2);
int DATAgreater(Data d1, Data d2);
int DATAlower(Data d1, Data d2);

#endif //ES01_DATA_H
