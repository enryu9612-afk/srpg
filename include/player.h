#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

typedef struct {
    Entity base; // 엔티티 공통 속성 상속 (Composition)
    // 플레이어 전용 속성 (예: 경험치, 골드 등) 추가 예정
    int exp;
    int gold;
} Player;

void InitPlayer(Player *player, int startX, int startY);
void MovePlayer(Player *player, int dx, int dy, const Map *map);

#endif
