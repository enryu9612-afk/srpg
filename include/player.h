#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"

#define MAX_PARTY_MEMBERS 3

// GDD의 '오퍼레이터' 정의: 기본 엔티티 스탯 + 5대 기관 스탯
typedef Operator PlayerUnit; 

typedef struct {
    PlayerUnit members[MAX_PARTY_MEMBERS];
    int current_unit_idx;
    int count;
} Party;

void InitParty(Party *party);
void MoveActiveUnit(Party *party, int dx, int dy, const Map *map);
PlayerUnit* GetActiveUnit(Party *party);

#endif
