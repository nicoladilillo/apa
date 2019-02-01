
#include <stdio.h>

#include "item.h"

Item ITEMsetnull() {
    Item t;

    t.preso = -1;
    t.v = -1;
    t.wt = -1;

    return t;
}

Item ITEMcreate(int v, int wt) {
    Item t;

    t.preso = 1;
    t.wt = wt;
    t.v = v;

    return t;
}
