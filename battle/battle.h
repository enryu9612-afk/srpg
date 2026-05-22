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
    bool is_combat_active;
} BattleState;

extern BattleState g_battle_state;

// 전투 핵심 로직
int32_t Battle_CalculatePhysicalDamage(Operator* attacker, Operator* defender);
int32_t Battle_CalculateMagicDamage(Operator* attacker, Operator* defender);
bool Battle_CheckHit(Operator* attacker, Operator* defender);
bool Battle_CheckRange(Entity* attacker, Entity* defender, int32_t range);

// 전투 실행 (결과: 0=Miss, >0=Damage)
int32_t Battle_ExecuteAttack(Operator* attacker, Entity* target);

// 턴 관리
void Battle_NextTurn(void);
void Battle_Init(void);

#endif // BATTLE_BATTLE_H
