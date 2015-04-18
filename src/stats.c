#include <stats.h>

Stats
stats_init()
{
    Stats stats = calloc(sizeof *stats, 1);
    stats->hitpoints = 1;
    stats->speed = 1;
    stats->min_damage = 1;
    return stats;
}

void
stats_free(Stats *stats)
{
    free(*stats);
}

bool
stats_is_turn(Stats stats)
{
    if (stats_is_dead(stats))
        return false;
    if (stats->charged_speed >= MAX_CHARGED_SPEED)
        return true;
    return false;
}

void
stats_charge_speed(Stats stats)
{
    stats->charged_speed += stats->speed;
}

void
stats_attack_dmg(Stats attacker, Stats defender)
{
    if (defender->hitpoints >= attacker->min_damage)
        defender->hitpoints -= attacker->min_damage;
    else
        defender->hitpoints = 0;
}

bool
stats_is_dead(Stats stats)
{
    if (stats->hitpoints == 0)
        return true;
    return false;
}
