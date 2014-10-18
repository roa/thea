#ifndef THEA_HELPER_H
#define THEA_HELPER_H

#include <stdlib.h>

#define DICE(_n) (random() % _n)

#define MAYBE_CONTINUE(_chance) \
    if (DICE(100) < _chance)    \
        continue;

#endif
