#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "entity/entity.h"
#include "map/map.h"
#include "battle/battle.h"

void test_operator_init() {
    printf("[Test] Verifying Operator Init... ");
    Operator op;
    Operator_Init(&op, 1, 0, 0);
    
    assert(op.sp == 20);
    assert(op.max_sp == 20);
    assert(op.is_alive == true);
    assert(op.level == 1);
    printf("PASS\n");
}

void test_map_logic() {
    printf("[Test] Verifying Map Generation... ");
    Map* map = Map_Create(60, 30);
    bool success = Map_Generate(map, 12345);
    
    assert(success == true);
    // Check if at least one wall and one floor exist
    bool found_wall = false;
    bool found_floor = false;
    for(int i=0; i < map->width * map->height; i++) {
        if(map->tiles[i] == TILE_WALL) found_wall = true;
        if(map->tiles[i] == TILE_FLOOR) found_floor = true;
    }
    assert(found_wall && found_floor);
    Map_Destroy(map);
    printf("PASS\n");
}

void test_battle_formulas() {
    printf("[Test] Verifying Battle Formulas... ");
    Operator attacker, defender;
    Operator_Init(&attacker, 1, 0, 0);
    Operator_Init(&defender, 2, 1, 1);
    
    attacker.attack = 20;
    defender.defense = 5;
    assert(Battle_CalculatePhysicalDamage(&attacker, &defender) == 15);
    
    attacker.attack = 10;
    defender.magic_res = 20;
    // 10 * (1 - 0.2) = 8
    assert(Battle_CalculateMagicDamage(&attacker, &defender) == 8);
    
    printf("PASS\n");
}

void test_battle_range() {
    printf("[Test] Verifying Battle Range... ");
    Entity e1 = { .x = 0, .y = 0 };
    Entity e2 = { .x = 2, .y = 1 }; // Dist = 3
    
    assert(Battle_CheckRange(&e1, &e2, 3) == true);
    assert(Battle_CheckRange(&e1, &e2, 2) == false);
    printf("PASS\n");
}

int main() {
    printf("=== SRPG Emergency Fix Verification Suite ===\n");
    test_operator_init();
    test_map_logic();
    test_battle_formulas();
    test_battle_range();
    printf("============================================\n");
    printf("ALL TESTS PASSED SUCCESSFULLY\n");
    return 0;
}
