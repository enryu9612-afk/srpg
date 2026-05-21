#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "item.h"

#define MAX_INVENTORY 10
#define MAX_PARTY_SIZE 3

// GDD의 '오퍼레이터' 정의: 기본 엔티티 스탯 + 5대 기관 스탯 + 스킬
typedef Operator PlayerUnit; 

typedef struct {
    PlayerUnit members[MAX_PARTY_SIZE];
    int count;
    int current_unit_idx;
    Item inventory[MAX_INVENTORY];
    int inventory_count;
} Party;

void InitParty(Party *party);
PlayerUnit* GetActiveUnit(Party *party);
void MoveActiveUnit(Party *party, int dx, int dy, const Map *map);
void InitPlayer(PlayerUnit *player);
void MovePlayer(PlayerUnit *player, int dx, int dy, const Map *map);
bool EquipItem(PlayerUnit *player, Item *item);

#endif
