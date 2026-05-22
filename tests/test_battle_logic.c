#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "entity/entity.h"
#include "battle/battle.h"

// Helper to create a dummy operator with specific stats
void SetupOperator(Operator* op, int32_t atk, int32_t def, int32_t acc, int32_t eva, int32_t mres) {
    Operator_Init(op, 0, 0, 0);
    op->attack = atk;
    op->defense = def;
    op->accuracy = acc;
    op->evasion = eva;
    op->magic_res = mres;
}

void test_physical_damage() {
    printf("[Test] Physical Damage... ");
    Operator atk, def;
    
    // Case 1: Normal damage (20 - 5 = 15)
    SetupOperator(&atk, 20, 0, 0, 0, 0);
    SetupOperator(&def, 0, 5, 0, 0, 0);
    assert(Battle_CalculatePhysicalDamage(&atk, &def) == 15);
    
    // Case 2: Zero damage (10 - 15 = -5 -> 0)
    SetupOperator(&atk, 10, 0, 0, 0, 0);
    SetupOperator(&def, 0, 15, 0, 0, 0);
    assert(Battle_CalculatePhysicalDamage(&atk, &def) == 0);
    
    printf("PASS\n");
}

void test_magic_damage() {
    printf("[Test] Magic Damage... ");
    Operator atk, def;
    
    // Case 1: 20% Resistance (100 * (1 - 0.2) = 80)
    SetupOperator(&atk, 100, 0, 0, 0, 0);
    SetupOperator(&def, 0, 0, 0, 0, 20);
    assert(Battle_CalculateMagicDamage(&atk, &def) == 80);
    
    // Case 2: 100% Resistance (100 * (1 - 1.0) = 0)
    SetupOperator(&def, 0, 0, 0, 0, 100);
    assert(Battle_CalculateMagicDamage(&atk, &def) == 0);
    
    // Case 3: 0% Resistance (100 * (1 - 0) = 100)
    SetupOperator(&def, 0, 0, 0, 0, 0);
    assert(Battle_CalculateMagicDamage(&atk, &def) == 100);
    
    printf("PASS\n");
}

void test_hit_evasion() {
    printf("[Test] Hit/Evasion... ");
    Operator atk, def;
    
    // Current implementation: (Acc - Eva) > 50 hits
    // Case 1: Hit (80 - 20 = 60 > 50)
    SetupOperator(&atk, 0, 0, 80, 0, 0);
    SetupOperator(&def, 0, 0, 0, 20, 0);
    assert(Battle_CheckHit(&atk, &def) == true);
    
    // Case 2: Miss (70 - 30 = 40 <= 50)
    SetupOperator(&atk, 0, 0, 70, 0, 0);
    SetupOperator(&def, 0, 0, 0, 30, 0);
    assert(Battle_CheckHit(&atk, &def) == false);
    
    // Case 3: Boundary (50 + 1 = 51 > 50)
    SetupOperator(&atk, 0, 0, 60, 0, 0);
    SetupOperator(&def, 0, 0, 0, 9, 0); // 60-9 = 51
    assert(Battle_CheckHit(&atk, &def) == true);
    
    // Case 4: Boundary (50 = 50 <= 50)
    SetupOperator(&def, 0, 0, 0, 10, 0); // 60-10 = 50
    assert(Battle_CheckHit(&atk, &def) == false);
    
    printf("PASS\n");
}

void test_range() {
    printf("[Test] Range... ");
    Entity e1 = { .x = 10, .y = 10 };
    Entity e2 = { .x = 12, .y = 11 }; // Dist = |12-10| + |11-10| = 3
    
    assert(Battle_CheckRange(&e1, &e2, 3) == true);
    assert(Battle_CheckRange(&e1, &e2, 2) == false);
    printf("PASS\n");
}

void test_turn_system() {
    printf("[Test] Turn System... ");
    Battle_Init();
    assert(g_battle_state.current_turn == BATTLE_TURN_PLAYER);
    
    Battle_NextTurn();
    assert(g_battle_state.current_turn == BATTLE_TURN_ENEMY);
    
    Battle_NextTurn();
    assert(g_battle_state.current_turn == BATTLE_TURN_PLAYER);
    printf("PASS\n");
}

int main() {
    printf("=== SRPG Battle Logic Headless Test ===\n");
    test_physical_damage();
    test_magic_damage();
    test_hit_evasion();
    test_range();
    test_turn_system();
    printf("=======================================\n");
    printf("ALL BATTLE LOGIC TESTS PASSED\n");
    return 0;
}
