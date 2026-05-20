#include "combat.h"
#include "entity.h"
#include "ui.h"
#include "world_item.h"
#include "item.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

CombatResult CalculateDamage(Entity *attacker, Entity *defender, Equipment *eq, AttackType type, Operator *op_attacker) {
    CombatResult result = {0, 0, 0, 0.0f};
    float attacker_acc = attacker->acc;
    if (op_attacker) attacker_acc += (GetEyeBonus(op_attacker->eye) * 100.0f);
    float defender_eva = defender->eva;
    float final_hit_chance = attacker_acc - defender_eva;
    if (eq && eq->type == WEAPON_CROSSBOW) {
        int dist = abs(attacker->x - defender->x) + abs(attacker->y - defender->y);
        final_hit_chance -= (dist * 10.0f);
    }
    if ((rand() % 100) >= final_hit_chance) return result;
    result.is_hit = 1;
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
        float tongue_bonus = 1.0f;
        if (op_attacker) tongue_bonus = 1.0f + GetHandBonus(op_attacker->tongue);
        final_atk = attacker->atk * tongue_bonus;
    }
    result.final_atk_used = final_atk;
    float crit_chance = 0.05f; 
    if (op_attacker) crit_chance += GetHandBonus(op_attacker->hand);
    if (((float)rand() / RAND_MAX) < crit_chance) {
        result.is_critical = 1;
        result.damage = (int)(final_atk * 1.5f); 
    } else {
        if (type == ATTACK_PHYSICAL) {
            float target_def = defender->def;
            if (eq && eq->type == WEAPON_MACE) target_def *= 0.5f;
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
        if (defender->hp_cur == 0) {
            defender->is_alive = 0;
            
            // [추가] 적 처치 시 GDD 기반 아이템 드랍 로직
            // 일반 적: 6% 확률로 평범한 장비, 10골드
            // 엘리트 적: 100% 희귀 장비, 20~100골드
            // 보스 적: 100% 보스 장비, 200골드
            
            // 여기서는 기본적으로 '일반 적'으로 가정하여 드랍 구현
            if ((rand() % 100) < 6) {
                Item drop = CreateWeapon(WEAPON_SWORD, RARITY_COMMON, "Common Sword");
                SpawnWorldItem(defender->x, defender->y, drop);
            }
        }
        char dmgText[32];
        sprintf(dmgText, "%d", res.damage);
        Color textColor = res.is_critical ? RED : WHITE;
        AddFloatingText(defender->x, defender->y, dmgText, textColor);
    } else {
        AddFloatingText(defender->x, defender->y, "MISS", GRAY);
    }
}
