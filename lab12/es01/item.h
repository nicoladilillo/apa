#ifndef ES01_ITEM_H
#define ES01_ITEM_H

typedef struct {
    int v;
    int wt;
    int preso; // serve per rimozione archi nella ricerca del DAG
} Item;

Item ITEMsetnull();
Item ITEMcreate(int v, int wt);

#endif //ES01_ITEM_H
