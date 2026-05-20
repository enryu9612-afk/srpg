#ifndef SHOP_H
#define SHOP_H

#include "item.h"
#include "player.h"

typedef struct {
    Item item;
    int price;
} ShopItem;

extern ShopItem shop_inventory[];
extern int shop_inventory_count;

void InitShop(); // 상점 상품 목록 초기화
void OpenShop(Party *party);
void BuyItemFromShop(Party *party, int shop_idx);
void SellItemToShop(Party *party, int inv_idx);

#endif
