#ifndef BATTLE_BATTLE_H
#define BATTLE_BATTLE_H
 
#include "entity/entity.h"
#include "map/map.h"
#include <stdbool.h>
 
typedef enum {
    BATTLE_TURN_PLAYER,
    BATTLE_TURN_ENEMY
} BattleTurn;
 
typedef struct {
    BattleTurn current_turn;
    bool is_combat_active; // true: strict turn-based, false: free exploration
} BattleState;
 
extern BattleState g_battle_state;
 
// 전투 핵심 로직
int32_t Battle_CalculatePhysicalDamage(int32_t attack, int32_t defense);
int32_t Battle_CalculateMagicDamage(int32_t attack, int32_t magic_res);
bool Battle_CheckHit(int32_t accuracy, int32_t evasion);
bool Battle_CheckRange(Entity* attacker, Entity* defender, int32_t range);
int32_t Battle_ExecuteAttack(Operator* attacker, Entity* target);
int32_t Battle_ExecuteEnemyAttack(Enemy* attacker, Operator* target);

// AI 및 상태이상
void Battle_UpdateEnemyAI(Enemy* enemy, Operator* player, Map* map);
void Battle_UpdateStatusEffects(Entity* entity);

void Battle_NextTurn(void);
 
#endif // BATTLE_BATTLE_H
