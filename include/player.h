#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "item.h"

#define MAX_PARTY_MEMBERS 3
#define MAX_INVENTORY 10

typedef struct {
    Entity base;
    int exp;
    int gold;
    
    // 장착 중인 장비
    Item *weapon;
    Item *armor;
    Item *accessory;
} PlayerUnit;

typedef struct {
    PlayerUnit members[MAX_PARTY_MEMBERS];
    Item inventory[MAX_INVENTORY];
    int inventory_count;
    int current_unit_idx;
    int count;
} Party;

void InitParty(Party *party);
void MoveActiveUnit(Party *party, int dx, int dy, const Map *map);
PlayerUnit* GetActiveUnit(Party *party);
bool EquipItem(PlayerUnit *unit, Item *item);

#endif
