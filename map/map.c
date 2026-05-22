#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
 
// Internal helper: get tile at (x, y)
static uint8_t* get_tile(Map* map, int32_t x, int32_t y) {
    return &map->tiles[y * map->width + x];
}
 
Map* Map_Create(int32_t width, int32_t height) {
    Map* map = (Map*)malloc(sizeof(Map));
    if (!map) return NULL;
 
    map->width = width;
    map->height = height;
    map->tiles = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    if (!map->tiles) {
        free(map);
        return NULL;
    }
    
    map->rooms = (Room*)malloc(32 * sizeof(Room)); // Max 32 rooms for depth 4-5
    map->room_count = 0;
 
    memset(map->tiles, TILE_WALL, width * height);
    return map;
}
 
void Map_Destroy(Map* map) {
    if (map) {
        if (map->tiles) free(map->tiles);
        if (map->rooms) free(map->rooms);
        free(map);
    }
}
 
static void Map_CarveRoom(Map* map, Rect rect) {
    // Create a room slightly smaller than the partition to leave walls
    int32_t rw = rect.w - 2;
    int32_t rh = rect.h - 2;
    if (rw < 3 || rh < 3) return;
 
    int32_t rx = rect.x + 1 + (rand() % (rect.w - rw - 2 > 0 ? rect.w - rw - 2 : 1));
    int32_t ry = rect.y + 1 + (rand() % (rect.h - rh - 2 > 0 ? rect.h - rh - 2 : 1));
 
    // Randomize room size slightly
    int32_t final_w = 3 + (rand() % (rw - 2));
    int32_t final_h = 3 + (rand() % (rh - 2));
 
    for (int32_t y = ry; y < ry + final_h && y < map->height - 1; y++) {
        for (int32_t x = rx; x < rx + final_w && x < map->width - 1; x++) {
            if (x > 0 && y > 0) *get_tile(map, x, y) = TILE_FLOOR;
        }
    }
    
    // Register room for spawn logic
    if (map->room_count < 32) {
        map->rooms[map->room_count].bounds = (Rect){rx, ry, final_w, final_h};
        map->rooms[map->room_count].id = map->room_count;
        map->room_count++;
    }
}
 
static void Map_CarveCorridor(Map* map, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    // L-shaped corridor
    int32_t cx = x1;
    int32_t cy = y1;
 
    while (cx != x2) {
        if (cx > 0 && cx < map->width - 1 && cy > 0 && cy < map->height - 1) {
            *get_tile(map, cx, cy) = TILE_FLOOR;
        }
        cx += (x2 > cx) ? 1 : -1;
    }
    while (cy != y2) {
        if (cx > 0 && cx < map->width - 1 && cy > 0 && cy < map->height - 1) {
            *get_tile(map, cx, cy) = TILE_FLOOR;
        }
        cy += (y2 > cy) ? 1 : -1;
    }
}
 
static void Map_BspSplit(Map* map, Rect rect, int32_t depth) {
    if (depth <= 0|| rect.w <= 6 || rect.h <= 6) {
        Map_CarveRoom(map, rect);
        return;
    }
 
    bool split_h = (rand() % 2 == 0);
    if (rect.w > rect.h && (rand() % 10 > 3)) split_h = false;
    if (rect.h > rect.w && (rand() % 10 > 3)) split_h = true;
 
    if (split_h) {
        int32_t split_y = rect.y + 3 + (rand() % (rect.h - 6));
        Rect top = {rect.x, rect.y, rect.w, split_y - rect.y};
        Rect bottom = {rect.x, split_y, rect.w, rect.h - (split_y - rect.y)};
        
        Map_BspSplit(map, top, depth - 1);
        Map_BspSplit(map, bottom, depth - 1);
        
        // Connect center of top and bottom
        Map_CarveCorridor(map, top.x + top.w/2, top.y + top.h/2, bottom.x + bottom.w/2, bottom.y + bottom.h/2);
    } else {
        int32_t split_x = rect.x + 3 + (rand() % (rect.w - 6));
        Rect left = {rect.x, rect.y, split_x - rect.x, rect.h};
        Rect right = {split_x, rect.y, rect.w - (split_x - rect.x), rect.h};
        
        Map_BspSplit(map, left, depth - 1);
        Map_BspSplit(map, right, depth - 1);
        
        // Connect center of left and right
        Map_CarveCorridor(map, left.x + left.w/2, left.y + left.h/2, right.x + right.w/2, right.y + right.h/2);
    }
}
 
bool Map_Generate(Map* map, uint32_t seed) {
    if (!map) return false;
    srand(seed);
 
    Rect root = {1, 1, map->width - 2, map->height - 2};
    Map_BspSplit(map, root, 4); // Split 4 times (16 potential rooms)
 
    return true;
}
 
bool Map_IsWalkable(Map* map, int32_t x, int32_t y) {
    if (x < 0 || x >= map->width || y < 0 || y >= map->height) return false;
    return *get_tile(map, x, y) != TILE_WALL;
}
 
void Map_PrintToConsole(Map* map) {
    if (!map) return;
    for (int32_t y = 0; y < map->height; y++) {
        for (int32_t x = 0; x < map->width; x++) {
            printf("%c", (map->tiles[y * map->width + x] == TILE_WALL) ? '#' : '.');
        }
        printf("\n");
    }
}
 
bool Map_FindSafeSpawnPos(Map* map, int32_t px, int32_t py, int32_t* out_x, int32_t* out_y) {
    if (!map || !out_x || !out_y) return false;
 
    for (int dy = -1; dy <= 1; dy++) {
        for (int dx = -1; dx <= 1; dx++) {
            if (dx == 0 && dy == 0) continue;
            int32_t ex = px + dx;
            int32_t ey = py + dy;
            if (ex >= 0 && ex < map->width && ey >= 0 && ey < map->height) {
                if (Map_IsWalkable(map, ex, ey)) {
                    *out_x = ex;
                    *out_y = ey;
                    return true;
                }
            }
        }
    }
    return false;
}
 
bool Map_FindRoomSpawnPos(Map* map, int32_t px, int32_t py, int32_t* out_x, int32_t* out_y) {
    if (!map || !out_x || !out_y || map->room_count == 0) return false;
 
    // 1. Find which room the player is currently in
    int32_t player_room_id = -1;
    for (int32_t i = 0; i < map->room_count; i++) {
        Room* r = &map->rooms[i];
        if (px >= r->bounds.x && px < r->bounds.x + r->bounds.w &&
            py >= r->bounds.y && py < r->bounds.y + r->bounds.h) {
            player_room_id = r->id;
            break;
        }
    }
 
    // 2. Pick a random room that is NOT the player's current room
    if (map->room_count < 2) return false; // Need at least 2 rooms to spawn elsewhere
    
    int32_t target_room_idx = rand() % map->room_count;
    int32_t attempts = 0;
    while (map->rooms[target_room_idx].id == player_room_id && attempts < 10) {
        target_room_idx = rand() % map->room_count;
        attempts++;
    }
 
    // 3. Pick a random floor tile inside the target room
    Room* target = &map->rooms[target_room_idx];
    int32_t rx = target->bounds.x + (rand() % target->bounds.w);
    int32_t ry = target->bounds.y + (rand() % target->bounds.h);
    
    if (Map_IsWalkable(map, rx, ry)) {
        *out_x = rx;
        *out_y = ry;
        return true;
    }
 
    return false; // Failed to find walkable tile in chosen room
}
