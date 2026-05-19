#ifndef COMBAT_H
#define COMBAT_H

#include "entity.h"

typedef enum {
    ATTACK_PHYSICAL,
    ATTACK_MAGIC
} AttackType;

typedef struct {
    int damage;
    int is_hit;
    int is_critical;
} CombatResult;

CombatResult CalculateDamage(Entity *attacker, Entity *defender, AttackType type);
void ResolveCombat(Entity *attacker, Entity *defender, AttackType type);

#endif
