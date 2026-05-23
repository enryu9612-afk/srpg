#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "battle/battle.h"
#include "entity/entity.h"
#include "map/map.h"

void test_ai_obstacle_avoidance() {
    printf("Testing AI Obstacle Avoidance... ");
    Map* map = Map_Create(10, 10);
    // Create a wall at (1, 0)
    map->tiles[0 * 10 + 1] = 0; // TILE_WALL = 0
    
    Operator op;
    Operator_Init(&op, 1, 2, 0); // Player at (2, 0)
    
    Enemy en;
    Enemy_Init(&en, 101, 0, 0, 1); // Enemy at (0, 0)
    
    // Enemy wants to move to (1, 0) but it's a wall.
    // It should try to move to (0, 1) if possible.
    Battle_UpdateEnemyAI(&en, &op, map);
    
    // Enemy should not be at (1, 0)
    assert(en.base.x != 1 || en.base.y != 0);
    // Enemy should have moved to (0, 1) or stayed at (0, 0) but not penetrated wall
    printf("PASS\n");
    Map_Destroy(map);
}

void test_status_effect_burn() {
    printf("Testing Burn Status Effect... ");
    Operator op;
    Operator_Init(&op, 1, 0, 0);
    op.max_hp = 100;
    op.hp = 100;
    op.statuses[0].type = STATUS_BURN;
    op.statuses[0].duration = 3;
    
    Battle_UpdateStatusEffects(&op.base);
    
    // 100 * 0.05 = 5 damage. HP should be 95.
    assert(op.hp == 95);
    assert(op.statuses[0].duration == 2);
    printf("PASS\n");
}

int main() {
    printf("--- Starting Phase 4 AI & Status Tests ---\n");
    test_ai_obstacle_avoidance();
    test_status_effect_burn();
    printf("--- All Phase 4 Tests Passed Successfully! ---\n");
    return 0;
}
