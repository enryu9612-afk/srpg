#ifndef WORLD_ITEM_H
#define WORLD_ITEM_H

#include "item.h"
#include "player.h"

#define MAX_WORLD_ITEMS 20

typedef struct {
    Item item;
    int x, y;
    bool active;
} WorldItem;

extern WorldItem world_items[MAX_WORLD_ITEMS];

void SpawnWorldItem(int x, int y, Item item);
void UpdateWorldItems(); // 필요 시 수명 관리
void PickUpItem(Party *party, int x, int y);
void DrawWorldItems(GameCamera cam);

#endif
