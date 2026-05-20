#ifndef COMBAT_H
#define COMBAT_H

#include "entity.h"
#include "player.h"

typedef enum {
    ATTACK_PHYSICAL,
    ATTACK_MAGIC
} AttackType;

typedef enum {
    WEAPON_SWORD,
    WEAPON_SPEAR,
    WEAPON_CROSSBOW,
    WEAPON_STAFF,
    WEAPON_MACE
} WeaponType;

typedef struct {
    WeaponType type;
    int rarity; // 0: 조잡, 1: 평범, 2: 고급, 3: 희귀, 4: 유일
    float base_atk;
    float base_def;
    float magic_res;
} Equipment;

typedef struct {
    int damage;
    int is_hit;
    int is_critical;
    float final_atk_used; 
} CombatResult;

// 무기별 기본 사거리 반환
int GetWeaponRange(WeaponType type);

// 공격 가능 여부 확인 (사거리 체크)
int CanAttack(Entity *attacker, Entity *target, Equipment *eq);

CombatResult CalculateDamage(Entity *attacker, Entity *defender, Equipment *eq, AttackType type, Operator *op_attacker);
void ResolveCombat(Entity *attacker, Entity *defender, Equipment *eq, AttackType type, Operator *op_attacker);

#endif
