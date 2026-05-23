#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "map/map.h"
#include "ui/ui.h"
 
void test_enemy_spawn_logic() {
    printf("[Test] Verifying Enemy Spawn Logic... ");
    Map* map = Map_Create(60, 30);
    Map_Generate(map, 12345);
    
    // Player at (0,0) - if it's walkable, or first floor tile
    int32_t px = 0, py = 0;
    bool found = false;
    for(int y=0; y<30 && !found; y++) {
        for(int x=0; x<60 && !found; x++) {
            if(Map_IsWalkable(map, x, y)) {
                px = x; py = y; found = true;
            }
        }
    }
    
    int32_t ex, ey;
    bool spawn_success = Map_FindRoomSpawnPos(map, px, py, &ex, &ey);
    
    // Check 1: Did it spawn?
    assert(spawn_success == true);
    
    // Check 2: Is it in a different room?
    // Find player's room
    int32_t p_room = -1;
    for(int i=0; i<map->room_count; i++) {
        if(px >= map->rooms[i].bounds.x && px < map->rooms[i].bounds.x + map->rooms[i].bounds.w &&
           py >= map->rooms[i].bounds.y && py < map->rooms[i].bounds.y + map->rooms[i].bounds.h) {
            p_room = map->rooms[i].id;
            break;
        }
    }
    
    // Find enemy's room
    int32_t e_room = -1;
    for(int i=0; i<map->room_count; i++) {
        if(ex >= map->rooms[i].bounds.x && ex < map->rooms[i].bounds.x + map->rooms[i].bounds.w &&
           ey >= map->rooms[i].bounds.y && ey < map->rooms[i].bounds.y + map->rooms[i].bounds.h) {
            e_room = map->rooms[i].id;
            break;
        }
    }
    
    assert(p_room != e_room);
    printf("PASS (Enemy spawned in room %d, Player in room %d)\n", e_room, p_room);
    
    Map_Destroy(map);
}
 
int main() {
    printf("=== SRPG Logic Verification ===\n");
    test_enemy_spawn_logic();
    printf("==============================\n");
    printf("ALL TESTS PASSED\n");
    return 0;
}
