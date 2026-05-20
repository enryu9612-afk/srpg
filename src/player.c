#include "player.h"
#include "item.h"
#include <stdlib.h>
#include <string.h>

void InitParty(Party *party) {
    party->count = 0;
    party->current_unit_idx = 0;
    party->inventory_count = 0;

    for(int i = 0; i < MAX_PARTY_MEMBERS; i++) {
        InitEntity(&party->members[i].base, i + 1, ENTITY_PLAYER, 10 + i, 10, '@', 1);
        
        party->members[i].eye = 0;
        party->members[i].ear = 0;
        party->members[i].tongue = 0;
        party->members[i].hand = 0;
        party->members[i].heart = 0;
        
        party->members[i].weapon = NULL;
        party->members[i].armor = NULL;
        party->members[i].accessory = NULL;

        party->members[i].skill_count = 1;
        strcpy(party->members[i].skills[0].name, "Smite I");
        party->members[i].skills[0].sp_cost = 5;
        party->members[i].skills[0].multiplier = 1.1f;
        party->members[i].skills[0].level = 1;
        
        party->count++;
    }
}

bool EquipItem(PlayerUnit *unit, Item *item) {
    if (!item || item->category != ITEM_EQUIPMENT) return false;
    
    if (item->equip_stats.is_weapon) {
        unit->weapon = item;
    } else if (item->equip_stats.is_armor) {
        unit->armor = item;
    } else if (item->equip_stats.is_accessory) {
        unit->accessory = item;
    } else {
        return false;
    }
    return true;
}

// (나머지 MoveActiveUnit, GetActiveUnit 등 유지)
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
