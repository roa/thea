#ifndef THEA_MONSTER_H
#define THEA_MONSTER_H

#include <stdbool.h>
#include <stdint.h>

#include <animation.h>
#include <coord.h>
#include <logger.h>
#include <map.h>
#include <object.h>
#include <point.h>
#include <stats.h>

typedef struct MonsterData {
    Animation animation;
    int32_t   frame;
    int32_t   x;
    int32_t   y;
    Stats     stats;
} MonsterData;

typedef MonsterData *Monster;

Monster monster_init(const char *name, uint32_t x, uint32_t y);

void monster_free(Monster *monster);

void monster_move(Monster monster, int32_t x, int32_t y);

bool monster_animate(Monster monster, int32_t frame, bool moved);

typedef struct MonsterGroupData {
    Monster *monsters;
    uint8_t  size;
} MonsterGroupData;

typedef MonsterGroupData *MonsterGroup;

MonsterGroup monster_group_init(uint8_t size);

void monster_group_free(MonsterGroup *group);

Monster monster_group_get(MonsterGroup group, uint8_t num);

Coord monster_group_pos(MonsterGroup group, int8_t num);

#endif // THEA_MONSTER_H
