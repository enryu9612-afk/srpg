#include "battle.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

BattleState g_battle_state = { BATTLE_TURN_PLAYER, false };

void Battle_Init(void) {
    g_battle_state.current_turn = BATTLE_TURN_PLAYER;
    g_battle_state.is_combat_active = false;
}

// --- Pure Calculation Logic ---

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
    int32_t dx = abs(attacker->x - defender->x);
    int32_t dy = abs(attacker->y - defender->y);
    return (dx <= range && dy <= range); // Chebyshev Distance (Square/Octagonal)
}

// --- Combat Execution ---

int32_t Battle_ExecuteAttack(Operator* attacker, Entity* target) {
    if (!attacker || !target) return 0;
    if (!Battle_CheckRange(&attacker->base, target, 1)) return -1;
    if (target->type != ENTITY_TYPE_ENEMY) return -2;
    
    Enemy* enemy = (Enemy*)target;
    if (!Battle_CheckHit(attacker->accuracy, enemy->evasion)) return 0;
    
    int32_t damage = Battle_CalculatePhysicalDamage(attacker->attack, enemy->defense);
    enemy->hp -= damage;
    if (enemy->hp < 0) enemy->hp = 0;
    return damage;
}

int32_t Battle_ExecuteEnemyAttack(Enemy* attacker, Operator* target) {
    if (!attacker || !target) return 0;
    if (!Battle_CheckRange(&attacker->base, &target->base, 1)) return -1;
    if (!Battle_CheckHit(attacker->accuracy, target->evasion)) return 0;
    
    int32_t damage = Battle_CalculatePhysicalDamage(attacker->attack, target->defense);
    target->hp -= damage;
    if (target->hp < 0) target->hp = 0;
    return damage;
}

// --- AI Behavioral Logic ---

void Battle_UpdateEnemyAI(Enemy* enemy, Operator* player, Map* map) {
    if (!enemy || !player || !map) return;

    // 1. Attack if in range
    if (Battle_CheckRange(&enemy->base, &player->base, 1)) {
        Battle_ExecuteEnemyAttack(enemy, player);
        return;
    }

    // 2. Move toward player with basic obstacle avoidance
    int32_t dx = player->base.x - enemy->base.x;
    int32_t dy = player->base.y - enemy->base.y;

    int32_t move_x = 0, move_y = 0;
    if (abs(dx) > abs(dy)) {
        move_x = (dx > 0) ? 1 : -1;
        move_y = 0;
    } else {
        move_x = 0;
        move_y = (dy > 0) ? 1 : -1;
    }

    int32_t next_x = enemy->base.x + move_x;
    int32_t next_y = enemy->base.y + move_y;

    // Collision Check (Wall)
    bool blocked = !Map_IsWalkable(map, next_x, next_y);

    if (blocked) {
        // Try orthogonal move for avoidance
        if (move_x != 0) {
            // Try moving Y instead
            int32_t try_y = (dy > 0) ? 1 : -1;
            int32_t try_next_y = enemy->base.y + try_y;
            if (Map_IsWalkable(map, enemy->base.x, try_next_y) && 
                !(enemy->base.x == player->base.x && try_next_y == player->base.y)) {
                enemy->base.y = try_next_y;
            }
        } else {
            // Try moving X instead
            int32_t try_x = (dx > 0) ? 1 : -1;
            int32_t try_next_x = enemy->base.x + try_x;
            if (Map_IsWalkable(map, try_next_x, enemy->base.y) && 
                !(try_next_x == player->base.x && enemy->base.y == player->base.y)) {
                enemy->base.x = try_next_x;
            }
        }
    } else {
        // Check if moving into player
        if (next_x == player->base.x && next_y == player->base.y) {
            // Do not move into player's cell, just stay and attack (Attack is handled by Battle_UpdateEnemyAI step 1)
        } else {
            enemy->base.x = next_x;
            enemy->base.y = next_y;
        }
    }
}

// --- Status Effect Logic ---

void Battle_UpdateStatusEffects(Entity* entity) {
    if (!entity) return;

    Operator* op = (entity->type == ENTITY_TYPE_PLAYER) ? (Operator*)entity : NULL;
    Enemy* en = (entity->type == ENTITY_TYPE_ENEMY) ? (Enemy*)entity : NULL;
    
    StatusEffect* effects = (op) ? op->statuses : en->statuses;
    int32_t max_hp = (op) ? op->max_hp : en->max_hp;
    int32_t* hp = (op) ? &op->hp : &en->hp;

    for (int i = 0; i < 5; i++) {
        if (effects[i].type == STATUS_NONE) continue;

        if (effects[i].type == STATUS_BURN) {
            int32_t damage = (int32_t)(max_hp * 0.05f);
            *hp -= damage;
            if (*hp < 0) *hp = 0;
            printf("[Status] Burn! Lost %d HP\n", damage);
        }

        effects[i].duration--;
        if (effects[i].duration <= 0) {
            effects[i].type = STATUS_NONE;
        }
    }
}

void Battle_NextTurn(void) {
    g_battle_state.current_turn = (g_battle_state.current_turn == BATTLE_TURN_PLAYER) ? BATTLE_TURN_ENEMY : BATTLE_TURN_PLAYER;
    printf("[Battle] Turn changed to: %s\n", 
           (g_battle_state.current_turn == BATTLE_TURN_PLAYER) ? "Player" : "Enemy");
}
