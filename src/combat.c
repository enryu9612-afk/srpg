#include "combat.h"
#include "entity.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int GetWeaponRange(WeaponType type) {
    switch (type) {
        case WEAPON_SPEAR: return 2;    // 사거리 +1
        case WEAPON_CROSSBOW: return 5; // 사거리 +4
        default: return 1;
    }
}

int CanAttack(Entity *attacker, Entity *target, Equipment *eq) {
    int range = 1;
    if (eq) {
        range = GetWeaponRange(eq->type);
    }

    int dx = abs(attacker->x - target->x);
    int dy = abs(attacker->y - target->y);
    
    // 대각선 공격 불가 (GDD 명세)
    if (dx != 0 && dy != 0) return 0;

    // 맨해튼 거리 계산
    int dist = dx + dy;
    return (dist > 0 && dist <= range);
}

CombatResult CalculateDamage(Entity *attacker, Entity *defender, Equipment *eq, AttackType type, Operator *op_attacker) {
    CombatResult result = {0, 0, 0, 0.0f};

    // 1. 명중 판정: (공격자 명중률) - (피격자 회피율)
    float attacker_acc = attacker->acc;
    if (op_attacker) {
        attacker_acc += (GetEyeBonus(op_attacker->eye) * 100.0f);
    }

    float defender_eva = defender->eva;
    // (추후: defender가 Operator인 경우 귀 보너스 적용 로직 추가)

    float final_hit_chance = attacker_acc - defender_eva;

    // 석궁 특수 능력: 적과의 거리 1칸당 명중률 -10% (GDD 명세)
    if (eq && eq->type == WEAPON_CROSSBOW) {
        int dist = abs(attacker->x - defender->x) + abs(attacker->y - defender->y);
        final_hit_chance -= (dist * 10.0f);
    }

    if ((rand() % 100) >= final_hit_chance) {
        return result; // Miss
    }
    result.is_hit = 1;

    // 2. 공격력 계산
    float final_atk = attacker->atk;

    if (type == ATTACK_PHYSICAL) {
        float heart_bonus = 1.0f;
        float hand_bonus = 1.0f;
        
        if (op_attacker) {
            heart_bonus = 1.0f + GetHeartBonus(op_attacker->heart);
            hand_bonus = 1.0f + GetHandBonus(op_attacker->hand);
        }

        float eq_atk = eq ? eq->base_atk : 0.0f;
        final_atk = (attacker->atk * heart_bonus) + (eq_atk * hand_bonus);
    } else {
        // 마법 공격: 혀 스탯 기반 보너스 (GDD 명세)
        float tongue_bonus = 1.0f;
        if (op_attacker) {
            tongue_bonus = 1.0f + GetHandBonus(op_attacker->tongue);
        }
        final_atk = attacker->atk * tongue_bonus;
    }
    result.final_atk_used = final_atk;

    // 3. 치명타 판정: 기본 5% + 손 보너스 (합연산)
    float crit_chance = 0.05f; 
    if (op_attacker) {
        crit_chance += GetHandBonus(op_attacker->hand);
    }

    if (((float)rand() / RAND_MAX) < crit_chance) {
        result.is_critical = 1;
        result.damage = (int)(final_atk * 1.5f); 
    } else {
        if (type == ATTACK_PHYSICAL) {
            // 메이스 특수 능력: 적 방어력 50% 무시 (GDD 명세)
            float target_def = defender->def;
            if (eq && eq->type == WEAPON_MACE) {
                target_def *= 0.5f;
            }
            result.damage = (int)final_atk - (int)target_def;
        } else {
            result.damage = (int)(final_atk * (1.0f - (defender->magic_res / 100.0f)));
        }
        if (result.damage < 1) result.damage = 1;
    }

    return result;
}

void ResolveCombat(Entity *attacker, Entity *defender, Equipment *eq, AttackType type, Operator *op_attacker) {
    CombatResult res = CalculateDamage(attacker, defender, eq, type, op_attacker);
    if (res.is_hit) {
        defender->hp_cur -= res.damage;
        if (defender->hp_cur < 0) defender->hp_cur = 0;
        if (defender->hp_cur == 0) defender->is_alive = 0;
    }
}
