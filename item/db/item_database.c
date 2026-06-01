#include "../item.h"
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

// 도감 데이터 (Static Item Database)
const ItemTemplate g_item_database[] = {
    {
        1001, "Iron Sword", ITEM_TYPE_EQUIPMENT, ITEM_GRADE_COMMON,
        .data.equipment = { EQUIP_SLOT_WEAPON, {10, 0, 0, 0, 0}, 5 }
    },
    {
        1002, "Steel Plate", ITEM_TYPE_EQUIPMENT, ITEM_GRADE_COMMON,
        .data.equipment = { EQUIP_SLOT_ARMOR, {0, 10, 2, 0, -1}, 10 }
    },
    {
        2001, "Small Potion", ITEM_TYPE_CONSUMABLE, ITEM_GRADE_COMMON,
        .data.consumable = { 20, 5 }
    },
    {
        3001, "Dragon Slayer", ITEM_TYPE_EQUIPMENT, ITEM_GRADE_LEGENDARY,
        .data.equipment = { EQUIP_SLOT_WEAPON, {50, 0, 0, 5, 0}, 8 }
    }
};

const int32_t g_item_database_count = sizeof(g_item_database) / sizeof(ItemTemplate);

// 템플릿에서 실제 아이템 객체를 생성하는 함수
Item* Item_CreateFromTemplate(uint32_t id) {
    for (int i = 0; i < g_item_database_count; i++) {
        if (g_item_database[i].id == id) {
            Item* item = (Item*)malloc(sizeof(Item));
            if (!item) return NULL;
            // 템플릿 데이터 복사
            memcpy(item, &g_item_database[i], sizeof(Item));
            return item;
        }
    }
    return NULL;
}
