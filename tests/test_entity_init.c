#include "entity/entity.h"
#include <stdio.h>
#include <assert.h>

int main() {
    printf("--- Entity Initialization Test ---\n");

    Operator op;
    Operator_Init(&op, 1, 10, 10);

    printf("Level 1 Stats:\n");
    printf("HP: %d, ATK: %d, DEF: %d\n", op.hp, op.attack, op.defense);
    
    // Verify Level 1 defaults
    assert(op.level == 1);
    assert(op.hp == 100);
    assert(op.attack == 10);
    assert(op.special_stats.eye == 1);

    printf("\nUpdating level to 5...\n");
    Operator_SetLevel(&op, 5);

    printf("Level 5 Stats:\n");
    printf("HP: %d, ATK: %d, DEF: %d\n", op.hp, op.attack, op.defense);
    
    // Expected: 10 * (1.2^4) = 10 * 2.0736 = 20.736 -> 20
    printf("Expected ATK: ~20, Actual: %d\n", op.attack);

    printf("\n--- Test Complete: SUCCESS ---\n");
    return 0;
}
