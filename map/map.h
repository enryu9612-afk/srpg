#ifndef MAP_MAP_H
#define MAP_MAP_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    TILE_WALL = 0,
    TILE_FLOOR,
    TILE_DOOR
} TileType;

typedef struct {
    int32_t width;
    int32_t height;
    uint8_t *tiles;
} Map;

// Core API
Map* Map_Create(int32_t width, int32_t height);
void Map_Destroy(Map* map);
bool Map_Generate(Map* map, uint32_t seed);
bool Map_IsWalkable(Map* map, int32_t x, int32_t y);

// Debug/Testing API
void Map_PrintToConsole(Map* map);

#endif // MAP_MAP_H
