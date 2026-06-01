#include "map.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define MAX_ROOMS 64

// Internal helper: get tile at (x, y) with boundary check
static uint8_t* get_tile_safe(Map* map, int32_t x, int32_t y) {
    if (!map || x < 0 || x >= map->width || y < 0 || y >= map->height) return NULL;
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
    
    map->rooms = (Room*)malloc(MAX_ROOMS * sizeof(Room));
    if (!map->rooms) {
        free(map->tiles);
        free(map);
        return NULL;
    }
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

static Point Map_CarveRoom(Map* map, Rect rect) {
    // Ensure room is at least 3x3
    int32_t rw = rect.w - 2;
    int32_t rh = rect.h - 2;
    if (rw < 3 || rh < 3) return (Point){rect.x + rect.w/2, rect.y + rect.h/2};

    int32_t rx = rect.x + 1 + (rand() % (rect.w - rw - 2 > 0 ? rect.w - rw - 2 : 1));
    int32_t ry = rect.y + 1 + (rand() % (rect.h - rh - 2 > 0 ? rect.h - rh - 2 : 1));

    int32_t final_w = 3 + (rand() % (rw - 2 > 0 ? rw - 2 : 1));
    int32_t final_h = 3 + (rand() % (rh - 2 > 0 ? rh - 2 : 1));

    for (int32_t y = ry; y < ry + final_h; y++) {
        for (int32_t x = rx; x < rx + final_w; x++) {
            uint8_t* tile = get_tile_safe(map, x, y);
            if (tile) *tile = TILE_FLOOR;
        }
    }
    
    if (map->room_count < MAX_ROOMS) {
        map->rooms[map->room_count].bounds = (Rect){rx, ry, final_w, final_h};
        map->rooms[map->room_count].id = map->room_count;
        map->room_count++;
    }

    return (Point){rx + final_w / 2, ry + final_h / 2};
}

static void Map_CarveCorridor(Map* map, int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
    int32_t cx = x1;
    int32_t cy = y1;

    while (cx != x2) {
        uint8_t* tile = get_tile_safe(map, cx, cy);
        if (tile) *tile = TILE_FLOOR;
        cx += (x2 > cx) ? 1 : -1;
    }
    while (cy != y2) {
        uint8_t* tile = get_tile_safe(map, cx, cy);
        if (tile) *tile = TILE_FLOOR;
        cy += (y2 > cy) ? 1 : -1;
    }
}

static Point Map_BspSplit(Map* map, Rect rect, int32_t depth) {
    if (depth <= 0 || rect.w <= 6 || rect.h <= 6) {
        return Map_CarveRoom(map, rect);
    }

    bool split_h = (rand() % 2 == 0);
    if (rect.w > rect.h && (rand() % 10 > 3)) split_h = false;
    if (rect.h > rect.w && (rand() % 10 > 3)) split_h = true;

    if (split_h) {
        int32_t split_y = rect.y + 3 + (rand() % (rect.h - 6 > 0 ? rect.h - 6 : 1));
        Rect top = {rect.x, rect.y, rect.w, split_y - rect.y};
        Rect bottom = {rect.x, split_y, rect.w, rect.h - (split_y - rect.y)};
        
        Point p1 = Map_BspSplit(map, top, depth - 1);
        Point p2 = Map_BspSplit(map, bottom, depth - 1);
        Map_CarveCorridor(map, p1.x, p1.y, p2.x, p2.y);
        return (p1.x != 0 || p1.y != 0) ? p1 : p2;
    } else {
        int32_t split_x = rect.x + 3 + (rand() % (rect.w - 6 > 0 ? rect.w - 6 : 1));
        Rect left = {rect.x, rect.y, split_x - rect.x, rect.h};
        Rect right = {split_x, rect.y, rect.w - (split_x - rect.x), rect.h};
        
        Point p1 = Map_BspSplit(map, left, depth - 1);
        Point p2 = Map_BspSplit(map, right, depth - 1);
        Map_CarveCorridor(map, p1.x, p1.y, p2.x, p2.y);
        return (p1.x != 0 || p1.y != 0) ? p1 : p2;
    }
}

bool Map_Generate(Map* map, uint32_t seed) {
    if (!map) return false;
    srand(seed);

    Rect root = {1, 1, map->width - 2, map->height - 2};
    Map_BspSplit(map, root, 5);

    return true;
}

bool Map_IsWalkable(Map* map, int32_t x, int32_t y) {
    uint8_t* tile = get_tile_safe(map, x, y);
    return (tile && *tile != TILE_WALL);
}

void Map_PrintToConsole(Map* map) {
    if (!map) return;
    for (int32_t y = 0; y < map->height; y++) {
        for (int32_t x = 0; x < map->width; x++) {
            uint8_t* tile = get_tile_safe(map, x, y);
            printf("%c", (tile && *tile == TILE_WALL) ? '#' : '.');
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
            if (Map_IsWalkable(map, ex, ey)) {
                *out_x = ex;
                *out_y = ey;
                return true;
            }
        }
    }
    return false;
}

bool Map_FindRoomSpawnPos(Map* map, int32_t px, int32_t py, int32_t* out_x, int32_t* out_y) {
    if (!map || !out_x || !out_y || map->room_count == 0) return false;

    int32_t player_room_id = -1;
    for (int32_t i = 0; i < map->room_count; i++) {
        Room* r = &map->rooms[i];
        if (px >= r->bounds.x && px < r->bounds.x + r->bounds.w &&
            py >= r->bounds.y && py < r->bounds.y + r->bounds.h) {
            player_room_id = r->id;
            break;
        }
    }

    if (map->room_count < 2) return false;
    
    int32_t target_room_idx = rand() % map->room_count;
    int32_t attempts = 0;
    while (map->rooms[target_room_idx].id == player_room_id && attempts < 10) {
        target_room_idx = rand() % map->room_count;
        attempts++;
    }

    Room* target = &map->rooms[target_room_idx];
    int32_t rx = target->bounds.x + (rand() % target->bounds.w);
    int32_t ry = target->bounds.y + (rand() % target->bounds.h);
    
    if (Map_IsWalkable(map, rx, ry)) {
        *out_x = rx;
        *out_y = ry;
        return true;
    }

    return false;
}
