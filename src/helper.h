#ifndef THEA_HELPER_H
#define THEA_HELPER_H

#include <point.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <curses.h>

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

#define INCREMENT_FRAME(_sm)  \
    do {                      \
        if (_sm->frame == 59) \
        {                     \
            _sm->frame = 0;   \
            _sm->moved = 0;   \
        }                     \
        else                  \
            _sm->frame += 1;  \
    }  while (0)

uint32_t inverse_exit(uint32_t exit);

uint64_t get_micro_time();

char* create_num_string(const char *str, int32_t num);

char* create_dyn_string(const char *str);

uint8_t get_screen_ratio();

size_t width_as_string(int32_t num);

#endif // THEA_HELPER_H
