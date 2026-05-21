#include "player.h"
#include "item.h"
#include <stdlib.h>
#include <string.h>

void InitPlayer(PlayerUnit *player) {
    InitEntity(&player->base, 1, ENTITY_PLAYER, 10, 10, '@', 1);
    
    player->eye = 0;
    player->ear = 0;
    player->tongue = 0;
    player->hand = 0;
    player->heart = 0;
    player->gold = 100; // 초기 골드
    
    player->weapon = NULL;
    player->armor = NULL;
    player->accessory = NULL;

    player->skill_count = 1;
    strcpy(player->skills[0].name, "Smite I");
    player->skills[0].sp_cost = 5;
    player->skills[0].multiplier = 1.1f;
    player->skills[0].level = 1;
}

void MovePlayer(PlayerUnit *player, int dx, int dy, const Map *map) {
    int nextX = player->base.x + dx;
    int nextY = player->base.y + dy;
    if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
        if (map->tiles[nextY][nextX] != TILE_WALL) {
            player->base.x = nextX;
            player->base.y = nextY;
        }
    }
}

bool EquipItem(PlayerUnit *player, Item *item) {
    if (!item || item->category != ITEM_EQUIPMENT) return false;
    
    if (item->equip_stats.is_weapon) {
        player->weapon = item;
    } else if (item->equip_stats.is_armor) {
        player->armor = item;
    } else if (item->equip_stats.is_accessory) {
        player->accessory = item;
    } else {
        return false;
    }
    return true;
}

void InitParty(Party *party) {
    party->count = MAX_PARTY_SIZE;
    party->current_unit_idx = 0;
    party->inventory_count = 0;
    for(int i = 0; i < MAX_PARTY_SIZE; i++) {
        InitPlayer(&party->members[i]);
        // 파티원별 시작 위치 차별화
        party->members[i].base.x = 10 + i;
        party->members[i].base.y = 10;
    }
}

PlayerUnit* GetActiveUnit(Party *party) {
    return &party->members[party->current_unit_idx];
}

void MoveActiveUnit(Party *party, int dx, int dy, const Map *map) {
    PlayerUnit *active = GetActiveUnit(party);
    MovePlayer(active, dx, dy, map);
}
