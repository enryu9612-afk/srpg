#include "shop.h"
#include "ui.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

ShopItem shop_inventory[10];
int shop_inventory_count = 0;

void InitShop() {
    shop_inventory_count = 3;
    
    Item sp_potion = {0};
    sp_potion.category = ITEM_CONSUMABLE;
    strcpy(sp_potion.name, "SP Potion");
    
    Item hp_potion = {0};
    hp_potion.category = ITEM_CONSUMABLE;
    strcpy(hp_potion.name, "HP Potion");
    
    Item atk_potion = {0};
    atk_potion.category = ITEM_CONSUMABLE;
    strcpy(atk_potion.name, "ATK Potion");
    
    shop_inventory[0].item = sp_potion;
    shop_inventory[0].price = 50;
    
    shop_inventory[1].item = hp_potion;
    shop_inventory[1].price = 30;
    
    shop_inventory[2].item = atk_potion;
    shop_inventory[2].price = 80;
}

void OpenShop(Party *party) {
    AddLog("--- Welcome to the Rest Floor Shop! ---");
    AddLog("Available Items:");
    for (int i = 0; i < shop_inventory_count; i++) {
        char logMsg[64];
        sprintf(logMsg, "%d. %s - %dg", i + 1, shop_inventory[i].item.name, shop_inventory[i].price);
        AddLog(logMsg);
    }
}

void BuyItemFromShop(Party *party, int shop_idx) {
    if (shop_idx < 0 || shop_idx >= shop_inventory_count) return;
    
    ShopItem *s_item = &shop_inventory[shop_idx];
    // 현재 활성화된 파티원의 골드를 사용
    int active_idx = party->current_unit_idx;
    if (party->members[active_idx].gold < s_item->price) {
        AddLog("Not enough gold!");
        return;
    }
    if (party->inventory_count >= MAX_INVENTORY) {
        AddLog("Inventory full!");
        return;
    }
    
    party->members[active_idx].gold -= s_item->price;
    party->inventory[party->inventory_count++] = s_item->item;
    
    char logMsg[128];
    sprintf(logMsg, "Bought %s for %dg! (Remaining: %dg)", 
            s_item->item.name, s_item->price, party->members[active_idx].gold);
    AddLog(logMsg);
}

void SellItemToShop(Party *party, int inv_idx) {
    if (inv_idx < 0 || inv_idx >= party->inventory_count) return;
    
    Item *item = &party->inventory[inv_idx];
    int sell_price = 0;
    if (item->category == ITEM_EQUIPMENT) {
        sell_price = (item->rarity + 1) * 20;
    } else {
        sell_price = 10;
    }
    
    party->members[0].gold += sell_price;
    for (int i = inv_idx; i < party->inventory_count - 1; i++) {
        party->inventory[i] = party->inventory[i+1];
    }
    party->inventory_count--;
    
    char logMsg[128];
    sprintf(logMsg, "Sold %s for %dg!", item->name, sell_price);
    AddLog(logMsg);
}
