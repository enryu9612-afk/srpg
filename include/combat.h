#ifndef COMBAT_H
#define COMBAT_H

#include "entity.h"
#include "player.h"
#include "item.h"

typedef enum {
    ATTACK_PHYSICAL,
    ATTACK_MAGIC
} AttackType;

typedef struct {
    int damage;
    int is_hit;
    int is_critical;
    float final_atk_used; 
} CombatResult;

int GetWeaponRange(WeaponType type);
int CanAttack(Entity *attacker, Entity *target, Item *eq);
CombatResult CalculateDamage(Entity *attacker, Entity *defender, Item *eq, AttackType type, Operator *op_attacker);
void ResolveCombat(Entity *attacker, Entity *defender, Item *eq, AttackType type, Operator *op_attacker);

#endif
