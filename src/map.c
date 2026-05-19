#include "map.h"

void InitMap(Map *map) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            if (x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1) {
                map->tiles[y][x] = TILE_WALL;
            } else {
                map->tiles[y][x] = TILE_FLOOR;
            }
        }
    }
}

void DrawMap(const Map *map, Camera cam) {
    for (int y = 0; y < MAP_HEIGHT; y++) {
        for (int x = 0; x < MAP_WIDTH; x++) {
            DrawTile(x, y, (char)map->tiles[y][x], cam);
        }
    }
}
