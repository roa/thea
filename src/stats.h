#ifndef THEA_STATS_H
#define THEA_STATS_H

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#define MAX_CHARGED_SPEED 100

typedef struct StatsData {
    uint32_t hitpoints;
    uint32_t speed;
    uint32_t charged_speed;
    uint32_t min_damage;
} StatsData;

typedef StatsData *Stats;

Stats stats_init();

void stats_free(Stats *stats);

bool stats_is_turn(Stats stats);

void stats_charge_speed(Stats stats);

void stats_attack_dmg(Stats attacker, Stats defender);

bool stats_is_dead(Stats stats);

#endif // THEA_STATS_H
