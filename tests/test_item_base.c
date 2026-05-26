#include "item/item.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_item_creation() {
    printf("[Test] Item Creation...\n");
    Item* sword = Item_Create(1, "Steel Sword", ITEM_TYPE_EQUIPMENT, ITEM_GRADE_COMMON);
    assert(sword != NULL);
    assert(sword->id == 1);
    assert(strcmp(sword->name, "Steel Sword") == 0);
    assert(sword->type == ITEM_TYPE_EQUIPMENT);
    assert(sword->grade == ITEM_GRADE_COMMON);
    printf("Success!\n");
}

void test_inventory_basic() {
    printf("[Test] Inventory Basic...\n");
    Inventory inv = {0};
    Item* item1 = Item_Create(2, "Potion", ITEM_TYPE_CONSUMABLE, ITEM_GRADE_COMMON);
    Item* item2 = Item_Create(3, "Shield", ITEM_TYPE_EQUIPMENT, ITEM_GRADE_RARE);
    
    assert(Inventory_Add(&inv, item1) == true);
    assert(Inventory_Add(&inv, item2) == true);
    assert(inv.item_count == 2);
    assert(inv.slots[0] == item1);
    assert(inv.slots[1] == item2);
    
    Inventory_Remove(&inv, 0);
    assert(inv.item_count == 1);
    assert(inv.slots[0] == NULL);
    assert(inv.slots[1] == item2);
    printf("Success!\n");
}

int main() {
    test_item_creation();
    test_inventory_basic();
    printf("ALL ITEM BASE TESTS PASSED!\n");
    return 0;
}
