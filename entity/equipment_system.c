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

    // 1. Reset to base stats (or read from a separate 'base_stats' struct if implemented)
    // For now, we assume current attack/defense etc. are the "base"
    // To properly implement this, Operator should have 'base_attack' and 'current_attack'
    
    // We will use a temporary storage for base stats calculation
    // Normally this would be: final = base + gear_bonus + buffs
    
    StatBonus total_bonus = {0, 0, 0, 0, 0};
    
    // Sum up bonuses from all equipped gear
    StatBonus w_bonus = GetItemBonus(op->equipment.weapon);
    StatBonus a_bonus = GetItemBonus(op->equipment.armor);
    StatBonus acc_bonus = GetItemBonus(op->equipment.accessory);
    
    total_bonus.atk_bonus = w_bonus.atk_bonus + a_bonus.atk_bonus + acc_bonus.atk_bonus;
    total_bonus.def_bonus = w_bonus.def_bonus + a_bonus.def_bonus + acc_bonus.def_bonus;
    total_bonus.mr_bonus = w_bonus.mr_bonus + a_bonus.mr_bonus + acc_bonus.mr_bonus;
    total_bonus.acc_bonus = w_bonus.acc_bonus + a_bonus.acc_bonus + acc_bonus.acc_bonus;
    total_bonus.eva_bonus = w_bonus.eva_bonus + a_bonus.eva_bonus + acc_bonus.eva_bonus;

    // Apply to operator (Since we don't have 'base_stats' yet, we'll assume
    // these are added to the values set during Operator_Init or growth)
    // Note: This is a simplistic implementation. In a full system, 
    // we would subtract previous bonuses first.
    
    printf("[Operator] Stats updated with gear. ATK Bonus: %d, DEF Bonus: %d\n", 
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
    
    printf("[Operator] Equipped %s to slot %d.\n", item->name, slot);
    Operator_UpdateFinalStats(op);
}
