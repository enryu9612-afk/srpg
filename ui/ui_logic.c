#include <stdint.h>
#include <stddef.h>
#include "ui/ui.h"

void UI_MoveInventorySlot(int32_t* slot, int direction) {
    if (slot == NULL) return;

    switch (direction) {
        case 0: // Left
            if (*slot % 6 != 0) (*slot)--;
            break;
        case 1: // Right
            if (*slot % 6 != 5) (*slot)++;
            break;
        case 2: // Up
            if (*slot >= 6) (*slot) -= 6;
            break;
        case 3: // Down
            if (*slot + 6 < 24) (*slot) += 6;
            break;
    }
}
