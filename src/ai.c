#include "ai.h"
#include "map.h"
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
            // [공격] GDD 명세: 스킬 사정거리 내에 오퍼레이터가 존재하면 공격
            ResolveCombat(enemy, target, NULL, ATTACK_PHYSICAL, NULL);
            printf("Enemy attacked Operator! Damage dealt.\n");
        } else {
            // [이동] 가장 가까운 오퍼레이터 방향으로 1칸 이동 (벽 충돌 판정 포함)
            int moveX = 0, moveY = 0;
            if (abs(target->x - enemy->x) > abs(target->y - enemy->y)) {
                moveX = (target->x > enemy->x) ? 1 : (target->x < enemy->x ? -1 : 0);
            } else {
                moveY = (target->y > enemy->y) ? 1 : (target->y < enemy->y ? -1 : 0);
            }

            // 벽 충돌 및 타일 체크
            int nextX = enemy->x + moveX;
            int nextY = enemy->y + moveY;

            if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
                if (map->tiles[nextY][nextX] != TILE_WALL) {
                    enemy->x = nextX;
                    enemy->y = nextY;
                } else {
                    // 벽에 막혔을 때의 대안 경로 탐색 (단순 우회)
                    // X축 이동이 불가능하면 Y축으로 시도
                    if (moveX != 0) {
                        int altY = (target->y > enemy->y) ? 1 : (target->y < enemy->y ? -1 : 0);
                        if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
                            // 여전히 벽이면 포기
                            if (map->tiles[enemy->y + altY][enemy->x] != TILE_WALL) {
                                enemy->y += altY;
                            }
                        }
                    } else if (moveY != 0) {
                        // Y축 이동이 불가능하면 X축으로 시도
                        int altX = (target->x > enemy->x) ? 1 : (target->x < enemy->x ? -1 : 0);
                        if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
                            if (map->tiles[enemy->y][enemy->x + altX] != TILE_WALL) {
                                enemy->x += altX;
                            }
                        }
                    }
                }
            }
        }
    }
}
