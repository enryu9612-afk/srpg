#include "world_item.h"
#include "ui.h"
#include "raylib.h"
#include "game.h"
#include "map.h"
#include <stdio.h>
#include <string.h>

WorldItem world_items[MAX_WORLD_ITEMS] = {0};

void SpawnWorldItem(int x, int y, Item item) {
    for (int i = 0; i < MAX_WORLD_ITEMS; i++) {
        if (!world_items[i].active) {
            world_items[i].item = item;
            world_items[i].x = x;
            world_items[i].y = y;
            world_items[i].active = true;
            return;
        }
    }
}

void PickUpItem(Party *party, int x, int y) {
    for (int i = 0; i < MAX_WORLD_ITEMS; i++) {
        if (world_items[i].active && world_items[i].x == x && world_items[i].y == y) {
            Item item = world_items[i].item;
            
            // 인벤토리 공간 확인
            if (party->inventory_count < MAX_INVENTORY) {
                party->inventory[party->inventory_count++] = item;
                
                char logMsg[128];
                sprintf(logMsg, "Picked up: %s [%s]!", item.name, 
                        item.rarity == RARITY_RARE ? "RARE" : 
                        item.rarity == RARITY_UNIQUE ? "UNIQUE" : "COMMON");
                AddLog(logMsg);
                
                world_items[i].active = false;
            } else {
                AddLog("Inventory full!");
            }
            return;
        }
    }
}

void DrawWorldItems(GameCamera cam) {
    for (int i = 0; i < MAX_WORLD_ITEMS; i++) {
        if (world_items[i].active) {
            // 아이템 등급에 따라 색상을 다르게 표시
            Color itemColor = WHITE;
            if (world_items[i].item.rarity == RARITY_RARE) itemColor = GOLD;
            else if (world_items[i].item.rarity == RARITY_UNIQUE) itemColor = PURPLE;
            else if (world_items[i].item.rarity == RARITY_ADVANCED) itemColor = SKYBLUE;
            
            DrawTile(world_items[i].x, world_items[i].y, '?', itemColor, cam);
        }
    }
}
