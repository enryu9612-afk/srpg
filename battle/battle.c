#include "battle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

BattleState g_battle_state = { BATTLE_TURN_PLAYER, false };

void Battle_Init(void) {
    g_battle_state.current_turn = BATTLE_TURN_PLAYER;
    g_battle_state.is_combat_active = false;
}

// --- Pure Calculation Logic (S-Design Standard) ---

int32_t Battle_CalculatePhysicalDamage(int32_t attack, int32_t defense) {
    return MAX(0, attack - defense);
}

int32_t Battle_CalculateMagicDamage(int32_t attack, int32_t magic_res) {
    float multiplier = 1.0f - ((float)magic_res / 100.0f);
    return MAX(0, (int32_t)((float)attack * multiplier));
}

bool Battle_CheckHit(int32_t accuracy, int32_t evasion) {
    int32_t final_accuracy = accuracy - evasion;
    if (final_accuracy >= 100) return true;
    if (final_accuracy <= 0) return false;
    return (rand() % 100) < final_accuracy;
}

bool Battle_CheckRange(Entity* attacker, Entity* defender, int32_t range) {
    if (!attacker || !defender) return false;
    int32_t dist = abs(attacker->x - defender->x) + abs(attacker->y - defender->y);
    return dist <= range;
}

// --- Combat Execution ---

int32_t Battle_ExecuteAttack(Operator* attacker, Entity* target) {
    if (!attacker || !target) return 0;

    // 1. Range check (Default range 1)
    if (!Battle_CheckRange(&attacker->base, target, 1)) {
        return -1; // Range Error
    }

    // 2. Target validation
    if (target->type != ENTITY_TYPE_ENEMY) {
        return -2; // Invalid Target
    }
    Enemy* enemy = (Enemy*)target;

    // 3. Hit determination
    if (!Battle_CheckHit(attacker->accuracy, enemy->evasion)) {
        return 0; // Miss
    }

    // 4. Damage calculation (Physical)
    int32_t damage = Battle_CalculatePhysicalDamage(attacker->attack, enemy->defense);

    // 5. Apply damage
    enemy->hp -= damage;
    if (enemy->hp < 0) enemy->hp = 0;
    
    return damage;
}

int32_t Battle_ExecuteEnemyAttack(Enemy* attacker, Operator* target) {
    if (!attacker || !target) return 0;

    // 1. Range check (Default range 1)
    if (!Battle_CheckRange(&attacker->base, &target->base, 1)) {
        return -1; // Range Error
    }

    // 2. Hit determination
    if (!Battle_CheckHit(attacker->accuracy, target->evasion)) {
        return 0; // Miss
    }

    // 3. Damage calculation (Physical)
    int32_t damage = Battle_CalculatePhysicalDamage(attacker->attack, target->defense);

    // 4. Apply damage
    target->hp -= damage;
    if (target->hp < 0) target->hp = 0;

    return damage;
}

void Battle_NextTurn(void) {
    g_battle_state.current_turn = (g_battle_state.current_turn == BATTLE_TURN_PLAYER) ? BATTLE_TURN_ENEMY : BATTLE_TURN_PLAYER;
    printf("[Battle] Turn changed to: %s\n", 
           (g_battle_state.current_turn == BATTLE_TURN_PLAYER) ? "Player" : "Enemy");
}
