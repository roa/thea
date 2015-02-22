#ifndef THEA_HELPER_H
#define THEA_HELPER_H

#include <stdlib.h>
#include <stdint.h>
#include <point.h>

#define DICE(_n) (random() % _n)

#define MAYBE_CONTINUE(_chance) \
    if (DICE(100) < _chance)    \
        continue;

#define CREATE_UNIQUE(_x, _y) \
    (uint64_t) (0 | (uint64_t) _x | (uint64_t) _y << 32)

#define CREATE_SCENE_X(_unique) \
    (uint32_t) (_unique & 0xFFFFFFFF)

#define CREATE_SCENE_Y(_unique) \
    (uint32_t) ((_unique >> 32) & 0xFFFFFFFF)

#define CREATE_MAP_TYPE(_unique) \
    _unique == 0 ? 0 : DICE(NUM_MAP_TYPES)

uint32_t inverse_exit(uint32_t exit);

#endif // THEA_HELPER_H
