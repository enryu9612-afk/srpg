#include "item.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

Item* Item_Create(uint32_t id, const char* name, ItemType type, ItemGrade grade) {
    Item* item = (Item*)malloc(sizeof(Item));
    if (!item) return NULL;
    
    item->id = id;
    strncpy(item->name, name, 63);
    item->name[63] = '\0';
    item->type = type;
    item->grade = grade;
    
    return item;
}

void Item_SetEquipment(Item* item, EquipSlot slot, StatBonus bonus) {
    if (!item || item->type != ITEM_TYPE_EQUIPMENT) return;
    item->data.equipment.slot = slot;
    item->data.equipment.bonus = bonus;
}

void Item_SetConsumable(Item* item, int32_t heal, int32_t sp) {
    if (!item || item->type != ITEM_TYPE_CONSUMABLE) return;
    item->data.consumable.heal_amount = heal;
    item->data.consumable.sp_restore = sp;
}

bool Inventory_Add(Inventory* inv, Item* item) {
    if (!inv || !item) return false;
    
    for (int32_t i = 0; i < MAX_INVENTORY_SLOTS; i++) {
        if (inv->slots[i] == NULL) {
            inv->slots[i] = item;
            inv->item_count++;
            return true;
        }
    }
    return false;
}

void Inventory_Remove(Inventory* inv, int32_t slot_index) {
    if (!inv || slot_index < 0 || slot_index >= MAX_INVENTORY_SLOTS) return;
    
    if (inv->slots[slot_index]) {
        inv->slots[slot_index] = NULL;
        inv->item_count--;
    }
}
