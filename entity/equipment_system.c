#include "entity/entity.h"
#include "item/item.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Helper to safely get bonus from an item
static StatBonus GetItemBonus(Item* item) {
    StatBonus bonus = {0, 0, 0, 0, 0};
    if (item && item->type == ITEM_TYPE_EQUIPMENT) {
        bonus = item->data.equipment.bonus;
    }
    return bonus;
}

void Operator_UpdateFinalStats(Operator* op) {
    if (!op) return;

    StatBonus total_bonus = {0, 0, 0, 0, 0};
    
    StatBonus w_bonus = GetItemBonus(op->equipment.weapon);
    StatBonus a_bonus = GetItemBonus(op->equipment.armor);
    StatBonus acc_bonus = GetItemBonus(op->equipment.accessory);
    
    total_bonus.atk_bonus = w_bonus.atk_bonus + a_bonus.atk_bonus + acc_bonus.atk_bonus;
    total_bonus.def_bonus = w_bonus.def_bonus + a_bonus.def_bonus + acc_bonus.def_bonus;
    total_bonus.mr_bonus = w_bonus.mr_bonus + a_bonus.mr_bonus + acc_bonus.mr_bonus;
    total_bonus.acc_bonus = w_bonus.acc_bonus + a_bonus.acc_bonus + acc_bonus.acc_bonus;
    total_bonus.eva_bonus = w_bonus.eva_bonus + a_bonus.eva_bonus + acc_bonus.eva_bonus;

    printf("[Operator] Final Stats Calculated - ATK Bonus: %d, DEF Bonus: %d\n", 
            total_bonus.atk_bonus, total_bonus.def_bonus);
}

void Operator_EquipItem(Operator* op, int32_t inventory_slot) {
    if (!op || inventory_slot < 0 || inventory_slot >= MAX_INVENTORY_SLOTS) return;
    
    Item* item = op->inventory.slots[inventory_slot];
    if (!item || item->type != ITEM_TYPE_EQUIPMENT) {
        printf("[Operator] Slot %d does not contain equippable item.\n", inventory_slot);
        return;
    }
    
    EquipSlot slot = item->data.equipment.slot;
    Item* old_item = NULL;
    
    // 1. 기존 장비 저장
    switch (slot) {
        case EQUIP_SLOT_WEAPON:
            old_item = op->equipment.weapon;
            op->equipment.weapon = item;
            break;
        case EQUIP_SLOT_ARMOR:
            old_item = op->equipment.armor;
            op->equipment.armor = item;
            break;
        case EQUIP_SLOT_ACCESSORY:
            old_item = op->equipment.accessory;
            op->equipment.accessory = item;
            break;
        default:
            return;
    }
    
    // 2. 인벤토리에서 방금 장착한 아이템 제거 (순서를 먼저 해야 old_item이 제 자리를 찾음)
    op->inventory.slots[inventory_slot] = NULL;
    op->inventory.item_count--;
    
    // 3. 기존 장비를 인벤토리 빈 슬롯으로 회수
    if (old_item != NULL) {
        bool recovered = false;
        for (int32_t i = 0; i < MAX_INVENTORY_SLOTS; i++) {
            if (op->inventory.slots[i] == NULL) {
                op->inventory.slots[i] = old_item;
                op->inventory.item_count++;
                recovered = true;
                break;
            }
        }
        
        if (!recovered) {
            printf("[Operator] Inventory full! Old item %s was destroyed.\n", old_item->name);
            free(old_item);
        } else {
            printf("[Operator] Old item %s recovered to inventory.\n", old_item->name);
        }
    }
    
    printf("[Operator] Equipped %s to slot %d.\n", item->name, slot);
    Operator_UpdateFinalStats(op);
}
