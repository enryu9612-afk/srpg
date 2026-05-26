#include "entity/entity.h"
#include "item/item.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_equipment_system() {
    printf("[Test] Equipment System...\n");
    
    Operator op;
    Operator_Init(&op, 1, 0, 0);
    memset(&op.equipment, 0, sizeof(op.equipment));
    memset(&op.inventory, 0, sizeof(op.inventory));
    
    // 1. Create a powerful sword
    Item* sword = Item_Create(10, "Dragon Slayer", ITEM_TYPE_EQUIPMENT, ITEM_GRADE_LEGENDARY);
    StatBonus sword_bonus = {50, 0, 0, 5, 0}; // ATK +50, ACC +5
    Item_SetEquipment(sword, EQUIP_SLOT_WEAPON, sword_bonus);
    
    // Add to inventory
    Inventory_Add(&op.inventory, sword);
    
    // 2. Equip the sword
    Operator_EquipItem(&op, 0); // Slot 0
    
    assert(op.equipment.weapon == sword);
    printf("Equip Success: %s equipped to weapon slot.\n", op.equipment.weapon->name);
    
    // 3. Create armor
    Item* armor = Item_Create(11, "Steel Plate", ITEM_TYPE_EQUIPMENT, ITEM_GRADE_RARE);
    StatBonus armor_bonus = {0, 30, 10, 0, -5}; // DEF +30, MR +10, EVA -5
    Item_SetEquipment(armor, EQUIP_SLOT_ARMOR, armor_bonus);
    
    Inventory_Add(&op.inventory, armor);
    Operator_EquipItem(&op, 1); // Slot 1
    
    assert(op.equipment.armor == armor);
    printf("Equip Success: %s equipped to armor slot.\n", op.equipment.armor->name);
    
    printf("Equipment System Test Passed!\n");
}

int main() {
    test_equipment_system();
    printf("ALL EQUIPMENT TESTS PASSED!\n");
    return 0;
}
