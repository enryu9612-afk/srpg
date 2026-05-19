#include "combat.h"
#include <stdlib.h>
#include <stdio.h>

CombatResult CalculateDamage(Entity *attacker, Entity *defender, AttackType type) {
    CombatResult result = {0, 0, 0};

    // 1. 명중 판정 (명중률 - 회피율)
    int hit_chance = attacker->hit - defender->eva;
    if ((rand() % 100) < hit_chance) {
        result.is_hit = 1;
    } else {
        return result; // Miss
    }

    // 2. 데미지 계산
    if (type == ATTACK_PHYSICAL) {
        // 물리: 공격력 - 방어력 (최소 1)
        result.damage = attacker->atk - defender->def;
        if (result.damage < 1) result.damage = 1;
    } else {
        // 마법: 마법공격력 * (1 - 마법저항/100)
        // 정수 연산을 위해 100을 곱함
        result.damage = (attacker->matk * (100 - defender->mdef)) / 100;
        if (result.damage < 1) result.damage = 1;
    }

    return result;
}

void ResolveCombat(Entity *attacker, Entity *defender, AttackType type) {
    CombatResult res = CalculateDamage(attacker, defender, type);
    
    if (res.is_hit) {
        defender->hp_cur -= res.damage;
        if (defender->hp_cur < 0) defender->hp_cur = 0;
        if (defender->hp_cur == 0) defender->is_alive = 0;
    }
}
