#ifndef AI_H
#define AI_H

#include "entity.h"
#include "player.h"
#include "combat.h"
#include "map.h"

// 적군 턴 실행 (맵 정보를 받아 벽 충돌 판정 수행)
void ExecuteEnemyTurn(Entity *enemies, int enemy_count, Party *party, const Map *map);

#endif
