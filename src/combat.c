#include "combat.h"
#include "entity.h"
#include "ui.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

CombatResult CalculateDamage(Entity *attacker, Entity *defender, Equipment *eq, AttackType type, Operator *op_attacker) {
    CombatResult result = {0, 0, 0, 0.0f};

    // 1. 명중 판정
    float attacker_acc = attacker->acc;
    if (op_attacker) {
        attacker_acc += (GetEyeBonus(op_attacker->eye) * 100.0f);
    }

    float defender_eva = defender->eva;
    // 상태이상 반영: 병약(SICKLY) 시 회피율 감소? (GDD에 명시되지 않았으나 확장 가능)
    
    float final_hit_chance = attacker_acc - defender_eva;

    if (eq && eq->type == WEAPON_CROSSBOW) {
        int dist = abs(attacker->x - defender->x) + abs(attacker->y - defender->y);
        final_hit_chance -= (dist * 10.0f);
    }

    if ((rand() % 100) >= final_hit_chance) {
        return result;
    }
    result.is_hit = 1;

    // 2. 공격력 계산 (상태이상 반영)
    float final_atk = attacker->atk;
    
    // [상태이상] 분노(RAGE): 공격력 증가, 방어력 감소
    for(int i = 0; i < MAX_STATUS_EFFECTS; i++) {
        if(attacker->statuses[i].type == STATUS_RAGE) {
            final_atk *= 1.2f; // 임시: 공격력 20% 증가
            break;
        }
    }

    if (type == ATTACK_PHYSICAL) {
        float heart_bonus = 1.0f;
        float hand_bonus = 1.0f;
        if (op_attacker) {
            heart_bonus = 1.0f + GetHeartBonus(op_attacker->heart);
            hand_bonus = 1.0f + GetHandBonus(op_attacker->hand);
        }
        float eq_atk = eq ? eq->base_atk : 0.0f;
        final_atk = (attacker->atk * heart_bonus) + (eq_atk * hand_bonus);
        
        // 분노 상태일 때 추가 보너스 적용 (물리 공격력 기준)
        for(int i = 0; i < MAX_STATUS_EFFECTS; i++) {
            if(attacker->statuses[i].type == STATUS_RAGE) {
                final_atk *= 1.2f;
                break;
            }
        }
    } else {
        float tongue_bonus = 1.0f;
        if (op_attacker) {
            tongue_bonus = 1.0f + GetHandBonus(op_attacker->tongue);
        }
        final_atk = attacker->atk * tongue_bonus;
    }
    result.final_atk_used = final_atk;

    // 3. 치명타 판정
    float crit_chance = 0.05f; 
    if (op_attacker) {
        crit_chance += GetHandBonus(op_attacker->hand);
    }

    if (((float)rand() / RAND_MAX) < crit_chance) {
        result.is_critical = 1;
        result.damage = (int)(final_atk * 1.5f); 
    } else {
        if (type == ATTACK_PHYSICAL) {
            float target_def = defender->def;
            
            // [상태이상] 분노(RAGE): 방어력 감소
            for(int i = 0; i < MAX_STATUS_EFFECTS; i++) {
                if(defender->statuses[i].type == STATUS_RAGE) {
                    target_def *= 0.7f; // 30% 감소
                    break;
                }
            }
            // [상태이상] 병약(SICKLY): 방어력 감소
            for(int i = 0; i < MAX_STATUS_EFFECTS; i++) {
                if(defender->statuses[i].type == STATUS_SICKLY) {
                    target_def *= 0.7f; // 30% 감소
                    break;
                }
            }

            if (eq && eq->type == WEAPON_MACE) {
                target_def *= 0.5f;
            }
            result.damage = (int)final_atk - (int)target_def;
        } else {
            float target_magic_res = defender->magic_res;
            // [상태이상] 병약(SICKLY): 마법저항 감소
            for(int i = 0; i < MAX_STATUS_EFFECTS; i++) {
                if(defender->statuses[i].type == STATUS_SICKLY) {
                    target_magic_res *= 0.7f;
                    break;
                }
            }
            result.damage = (int)(final_atk * (1.0f - (target_magic_res / 100.0f)));
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
        
        char dmgText[32];
        sprintf(dmgText, "%d", res.damage);
        Color textColor = res.is_critical ? RED : WHITE;
        AddFloatingText(defender->x, defender->y, dmgText, textColor);
    } else {
        AddFloatingText(defender->x, defender->y, "MISS", GRAY);
    }
}
