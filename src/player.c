#include "player.h"
#include <stdlib.h>

void InitParty(Party *party) {
    party->count = 0;
    party->current_unit_idx = 0;

    // 테스트용으로 파티원 3명 초기화
    for(int i = 0; i < MAX_PARTY_MEMBERS; i++) {
        // 1. 기본 엔티티 초기화 (ID: i+1, 레벨 1)
        InitEntity(&party->members[i].base, i + 1, ENTITY_PLAYER, 10 + i, 10, '@', 1);
        
        // 2. 오퍼레이터 전용 5대 기관 스탯 초기화 (모두 0)
        party->members[i].eye = 0;
        party->members[i].ear = 0;
        party->members[i].tongue = 0;
        party->members[i].hand = 0;
        party->members[i].heart = 0;
        
        party->count++;
    }
}

void MoveActiveUnit(Party *party, int dx, int dy, const Map *map) {
    PlayerUnit *unit = GetActiveUnit(party);
    int nextX = unit->base.x + dx;
    int nextY = unit->base.y + dy;

    if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
        if (map->tiles[nextY][nextX] != TILE_WALL) {
            unit->base.x = nextX;
            unit->base.y = nextY;
        }
    }
}

PlayerUnit* GetActiveUnit(Party *party) {
    return &party->members[party->current_unit_idx];
}
