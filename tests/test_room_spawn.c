#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include "map/map.h"
 
void test_room_spawn() {
    printf("[Test] Map_FindRoomSpawnPos... ");
    Map* map = Map_Create(60, 30);
    Map_Generate(map, 12345);
    
    // Place player in the first room
    int32_t px = map->rooms[0].bounds.x + 1;
    int32_t py = map->rooms[0].bounds.y + 1;
    
    int32_t sx, sy;
    bool found = Map_FindRoomSpawnPos(map, px, py, &sx, &sy);
    assert(found == true);
    
    // Verify spawn is NOT in the same room as player
    bool in_same_room = false;
    Room* player_room = &map->rooms[0];
    if (sx >= player_room->bounds.x && sx < player_room->bounds.x + player_room->bounds.w &&
        sy >= player_room->bounds.y && sy < player_room->bounds.y + player_room->bounds.h) {
        in_same_room = true;
    }
    
    assert(in_same_room == false);
    
    Map_Destroy(map);
    printf("PASS\n");
}
 
int main() {
    printf("=== Map Room Spawn Test ===\n");
    test_room_spawn();
    printf("ALL TESTS PASSED\n");
    return 0;
}
