#include "combat.h"
#include "entity.h"
#include "ui.h"
#include "item.h"
#include "world_item.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int GetWeaponRange(WeaponType type) {
    switch (type) {
        case WEAPON_SPEAR: return 2;
        case WEAPON_CROSSBOW: return 5;
        default: return 1;
    }
}

int CanAttack(Entity *attacker, Entity *target, Item *eq) {
    int range = 1;
    if (eq && eq->category == ITEM_EQUIPMENT) {
        range = GetWeaponRange(eq->equip_stats.w_type);
    }

    int dx = abs(attacker->x - target->x);
    int dy = abs(attacker->y - target->y);
    if (dx != 0 && dy != 0) return 0;

    int dist = dx + dy;
    return (dist > 0 && dist <= range);
}

CombatResult CalculateDamage(Entity *attacker, Entity *defender, Item *eq, AttackType type, Operator *op_attacker) {
    CombatResult result = {0, 0, 0, 0.0f};

    float attacker_acc = attacker->acc;
    if (op_attacker) attacker_acc += (GetEyeBonus(op_attacker->eye) * 100.0f);
    float defender_eva = defender->eva;
    float final_hit_chance = attacker_acc - defender_eva;

    if (eq && eq->category == ITEM_EQUIPMENT && eq->equip_stats.w_type == WEAPON_CROSSBOW) {
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
        float eq_atk = (eq && eq->category == ITEM_EQUIPMENT) ? eq->equip_stats.atk_bonus : 0.0f;
        final_atk = (attacker->atk * heart_bonus) + (eq_atk * hand_bonus);
    } else {
        // Staff(매직) 공격: Tongue 스탯 레벨에 따른 ATK 보너스 적용
        // Tongue은 SP비용 감소 스탯이므로 데미지 배율엔 미적용
        float tongue_atk_bonus = 1.0f;
        final_atk = attacker->atk * tongue_atk_bonus;
    }
    result.final_atk_used = final_atk;

    float crit_chance = 0.05f; 
    if (op_attacker) crit_chance += GetHandBonus(op_attacker->hand);
    if (((float)rand() / RAND_MAX) < crit_chance) {
        result.is_critical = 1;
        // 크리티컬: 1.5배 데미지에도 방어력/마법저항 적용
        if (type == ATTACK_PHYSICAL) {
            float target_def = defender->def;
            if (eq && eq->category == ITEM_EQUIPMENT && eq->equip_stats.w_type == WEAPON_MACE) {
                target_def *= 0.5f;
            }
            result.damage = (int)(final_atk * 1.5f) - (int)target_def;
        } else {
            result.damage = (int)(final_atk * 1.5f * (1.0f - (defender->magic_res / 100.0f)));
        }
        if (result.damage < 1) result.damage = 1;
    } else {
        if (type == ATTACK_PHYSICAL) {
            float target_def = defender->def;
            if (eq && eq->category == ITEM_EQUIPMENT && eq->equip_stats.w_type == WEAPON_MACE) {
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

void ResolveCombat(Entity *attacker, Entity *defender, Item *eq, AttackType type, Operator *op_attacker) {
    CombatResult res = CalculateDamage(attacker, defender, eq, type, op_attacker);
    if (res.is_hit) {
        defender->hp_cur -= res.damage;
        if (defender->hp_cur < 0) defender->hp_cur = 0;
        if (defender->hp_cur == 0) {
            defender->is_alive = 0;
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
