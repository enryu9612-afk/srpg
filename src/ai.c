#include "ai.h"
#include "map.h"
#include "ui.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

void ExecuteEnemyTurn(Entity *enemies, int enemy_count, Party *party, const Map *map) {
    for (int i = 0; i < enemy_count; i++) {
        Entity *enemy = &enemies[i];
        if (!enemy->is_alive) continue;

        // 1. 타겟 선정: 가장 가까운 오퍼레이터 찾기
        Entity *target = NULL;
        int min_dist = 9999;
        for (int j = 0; j < party->count; j++) {
            Entity *op = &party->members[j].base;
            if (!op->is_alive) continue;
            int dist = abs(enemy->x - op->x) + abs(enemy->y - op->y);
            if (dist < min_dist) {
                min_dist = dist;
                target = op;
            }
        }

        if (!target) continue;

        // 2. 행동 결정
        int range = 1; // 일반 적 기본 사거리
        int dist = abs(enemy->x - target->x) + abs(enemy->y - target->y);
        int dx = abs(enemy->x - target->x);
        int dy = abs(enemy->y - target->y);

        if (dist > 0 && dist <= range && (dx == 0 || dy == 0)) {
            // [공격]
            // 로그 기록을 위해 CalculateDamage를 먼저 호출하여 결과 확인
            CombatResult res = CalculateDamage(enemy, target, NULL, ATTACK_PHYSICAL, NULL);
            ResolveCombat(enemy, target, NULL, ATTACK_PHYSICAL, NULL);
            
            char logMsg[128];
            if (res.is_hit) {
                sprintf(logMsg, "Enemy '%c' attacked Unit %d! Dealt %d damage.", 
                        enemy->symbol, target->id, res.damage);
            } else {
                sprintf(logMsg, "Enemy '%c' attacked Unit %d but MISSED!", 
                        enemy->symbol, target->id);
            }
            AddLog(logMsg);
        } else {
            // [이동] 가장 가까운 오퍼레이터 방향으로 1칸 이동 (벽 충돌 판정 포함)
            int moveX = 0, moveY = 0;
            if (abs(target->x - enemy->x) > abs(target->y - enemy->y)) {
                moveX = (target->x > enemy->x) ? 1 : (target->x < enemy->x ? -1 : 0);
            } else {
                moveY = (target->y > enemy->y) ? 1 : (target->y < enemy->y ? -1 : 0);
            }

            int nextX = enemy->x + moveX;
            int nextY = enemy->y + moveY;

            if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
                if (map->tiles[nextY][nextX] != TILE_WALL) {
                    enemy->x = nextX;
                    enemy->y = nextY;
                } else {
                    if (moveX != 0) {
                        int altY = (target->y > enemy->y) ? 1 : (target->y < enemy->y ? -1 : 0);
                        if (enemy->y + altY >= 0 && enemy->y + altY < MAP_HEIGHT &&
                            map->tiles[enemy->y + altY][enemy->x] != TILE_WALL) {
                            enemy->y += altY;
                        }
                    } else if (moveY != 0) {
                        int altX = (target->x > enemy->x) ? 1 : (target->x < enemy->x ? -1 : 0);
                        if (enemy->x + altX >= 0 && enemy->x + altX < MAP_WIDTH &&
                            map->tiles[enemy->y][enemy->x + altX] != TILE_WALL) {
                            enemy->x += altX;
                        }
                    }
                }
            }
        }
    }
}
