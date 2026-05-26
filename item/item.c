#include "item.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
