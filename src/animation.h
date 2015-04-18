#ifndef THEA_animation_H
#define THEA_animation_H

#include <stdint.h>
#include <stdbool.h>
#include <ncurses.h>

#include <object.h>

typedef struct AnimationData
{
    int32_t    frame;
    ObjectList objlist;
} AnimationData;

typedef AnimationData *Animation;

Animation animation_init(ObjectList objlist);

void animation_free(Animation animation);

bool animation_animate(Animation animation, int frame, bool moved);

bool animation_move_towards(Animation animation, Coord coord);

#endif // THEA_animation_H
