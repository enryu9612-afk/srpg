#include "shop.h"
#include "ui.h"
#include "raylib.h"
#include <stdio.h>
#include <string.h>

ShopItem shop_inventory[10];
int shop_inventory_count = 0;

void InitShop() {
    shop_inventory_count = 3;
    
    // GDD: 소모품 (SP 회복, 공격력 증가, 체력 회복 등)
    // 임시로 아이템 시스템의 Item 구조체를 활용하여 생성
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
    // 휴식층에서 호출될 상점 UI 로직
    // 실제로는 main 루프에서 STATE_SHOP 등으로 관리해야 하지만, 
    // 여기서는 개념적 구현과 로그/UI 출력을 담당
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
    
    // 골드 확인 (파티 첫 번째 유닛의 골드 기준)
    if (party->members[0].gold < s_item->price) {
        AddLog("Not enough gold!");
        return;
    }
    
    // 인벤토리 공간 확인
    if (party->inventory_count >= MAX_INVENTORY) {
        AddLog("Inventory full!");
        return;
    }
    
    party->members[0].gold -= s_item->price;
    party->inventory[party->inventory_count++] = s_item->item;
    
    char logMsg[128];
    sprintf(logMsg, "Bought %s for %dg!", s_item->item.name, s_item->price);
    AddLog(logMsg);
}

void SellItemToShop(Party *party, int inv_idx) {
    if (inv_idx < 0 || inv_idx >= party->inventory_count) return;
    
    Item *item = &party->inventory[inv_idx];
    int sell_price = 0;
    
    // 등급 및 카테고리에 따른 판매가 책정 (단순 구현)
    if (item->category == ITEM_EQUIPMENT) {
        sell_price = (item->rarity + 1) * 20;
    } else {
        sell_price = 10;
    }
    
    party->members[0].gold += sell_price;
    
    // 인벤토리에서 제거 (뒤에서 앞으로 당기기)
    for (int i = inv_idx; i < party->inventory_count - 1; i++) {
        party->inventory[i] = party->inventory[i+1];
    }
    party->inventory_count--;
    
    char logMsg[128];
    sprintf(logMsg, "Sold %s for %dg!", item->name, sell_price);
    AddLog(logMsg);
}
