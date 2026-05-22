#include "battle.h"
#include <stdio.h>
#include <stdlib.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

BattleState g_battle_state = { BATTLE_TURN_PLAYER, false };

void Battle_Init(void) {
    g_battle_state.current_turn = BATTLE_TURN_PLAYER;
    g_battle_state.is_combat_active = false;
}

int32_t Battle_CalculatePhysicalDamage(Operator* attacker, Operator* defender) {
    if (!attacker || !defender) return 0;
    int32_t damage = attacker->attack - defender->defense;
    return MAX(0, damage);
}

int32_t Battle_CalculateMagicDamage(Operator* attacker, Operator* defender) {
    if (!attacker || !defender) return 0;
    // Magic Damage: Attack * (1 - Magic Resistance / 100)
    float multiplier = 1.0f - ((float)defender->magic_res / 100.0f);
    int32_t damage = (int32_t)((float)attacker->attack * multiplier);
    return MAX(0, damage);
}

bool Battle_CheckHit(Operator* attacker, Operator* defender) {
    if (!attacker || !defender) return false;
    // Final Accuracy: Attacker Accuracy - Defender Evasion
    int32_t final_accuracy = attacker->accuracy - defender->evasion;
    
    // Simplification: if accuracy > 50, it hits (TBD: replace with rand() check)
    return final_accuracy > 50;
}

bool Battle_CheckRange(Entity* attacker, Entity* defender, int32_t range) {
    if (!attacker || !defender) return false;
    
    // Manhattan Distance: |x1-x2| + |y1-y2|
    int32_t dist = abs(attacker->x - defender->x) + abs(attacker->y - defender->y);
    return dist <= range;
}

void Battle_NextTurn(void) {
    if (g_battle_state.current_turn == BATTLE_TURN_PLAYER) {
        g_battle_state.current_turn = BATTLE_TURN_ENEMY;
    } else {
        g_battle_state.current_turn = BATTLE_TURN_PLAYER;
    }
    printf("[Battle] Turn changed to: %s\\n", 
           (g_battle_state.current_turn == BATTLE_TURN_PLAYER) ? "Player" : "Enemy");
}

int32_t Battle_ExecuteAttack(Operator* attacker, Entity* target) {
    if (!attacker || !target) return 0;

    // 1. 사거리 확인 (기본 사거리 1)
    if (!Battle_CheckRange(&attacker->base, target, 1)) {
        return -1; // Range Error
    }

    // 2. 타겟이 Enemy인지 확인하고 캐스팅
    if (target->type != ENTITY_TYPE_ENEMY) {
        return -2; // Invalid Target
    }
    Enemy* enemy = (Enemy*)target;

    // 3. 명중 판정 (현재는 단순화하여 Operator vs Operator 기준으로 구현됨. 
    // Enemy의 Evasion을 적용하기 위해 임시로 Enemy를 Operator로 간주하거나 
    // 별도의 Enemy_GetEvasion 함수가 필요함. 여기서는 Enemy 구조체의 evasion 필드를 직접 사용)
    
    // 명중률 계산을 위해 임시 Operator 구조체 생성 (타겟의 스탯을 복제)
    Operator dummy_defender = {0};
    dummy_defender.evasion = enemy->evasion;
    
    if (!Battle_CheckHit(attacker, &dummy_defender)) {
        return 0; // Miss
    }

    // 4. 데미지 계산 (물리 공격)
    // Enemy의 defense 사용
    Operator dummy_defender_stats;
    dummy_defender_stats.defense = enemy->defense;
    int32_t damage = Battle_CalculatePhysicalDamage(attacker, &dummy_defender_stats);

    // 5. HP 적용
    enemy->hp -= damage;
    if (enemy->hp <= 0) {
        enemy->hp = 0;
        enemy->base.type = ENTITY_TYPE_NPC; // 사망 처리 (또는 별도 상태)
    }

    return damage;
}
