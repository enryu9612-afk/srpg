#ifndef GROWTH_GROWTH_H
#define GROWTH_GROWTH_H

#include "entity/entity.h"

typedef enum {
    STAT_EYE = 0,
    STAT_EAR,
    STAT_TONGUE,
    STAT_HAND,
    STAT_HEART,
    STAT_COUNT
} StatType;

// Growth API
bool Growth_AddExp(Operator* op, int32_t amount);
bool Growth_InvestStat(Operator* op, StatType type, int32_t points);
void Growth_ApplyStatBonuses(Operator* op);

#endif // GROWTH_GROWTH_H
