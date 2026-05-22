#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "entity/entity.h"
#include "map/map.h"
#include "battle/battle.h"

void test_battle_execute() {
    printf("[Test] Battle_ExecuteAttack... ");
    Operator player;
    Operator_Init(&player, 1, 0, 0);
    player.attack = 20;
    player.accuracy = 100;
    
    Enemy enemy;
    Enemy_Init(&enemy, 101, 1, 0, 1); // Adjacent
    enemy.defense = 5;
    enemy.evasion = 0;
    enemy.hp = 50;

    // Case 1: Normal Attack (Adjacent)
    // Range check (1) -> Hit check (100-0=100 > 50) -> Damage (20-5=15)
    int32_t result = Battle_ExecuteAttack(&player, (Entity*)&enemy);
    assert(result == 15);
    assert(enemy.hp == 35);
    
    // Case 2: Out of Range
    enemy.base.x = 10;
    result = Battle_ExecuteAttack(&player, (Entity*)&enemy);
    assert(result == -1);
    
    // Case 3: Miss
    enemy.base.x = 1;
    player.accuracy = 40; // 40-0 = 40 <= 50 (Miss)
    result = Battle_ExecuteAttack(&player, (Entity*)&enemy);
    assert(result == 0);
    
    printf("PASS\n");
}

void test_spawn_logic() {
    printf("[Test] Map_FindSafeSpawnPos... ");
    Map* map = Map_Create(10, 10);
    // Fill with walls except for (0,0) and (1,0)
    for(int i=0; i<100; i++) map->tiles[i] = TILE_WALL;
    map->tiles[0] = TILE_FLOOR; // (0,0)
    map->tiles[1] = TILE_FLOOR; // (1,0)
    
    int32_t sx, sy;
    bool found = Map_FindSafeSpawnPos(map, 0, 0, &sx, &sy);
    assert(found == true);
    assert(sx == 1 && sy == 0);
    
    // Test no walkable tiles
    map->tiles[1] = TILE_WALL;
    found = Map_FindSafeSpawnPos(map, 0, 0, &sx, &sy);
    assert(found == false);
    
    Map_Destroy(map);
    printf("PASS\n");
}

int main() {
    printf("=== SRPG Integration Headless Test ===\n");
    test_battle_execute();
    test_spawn_logic();
    printf("====================================\n");
    printf("ALL INTEGRATION TESTS PASSED\n");
    return 0;
}
