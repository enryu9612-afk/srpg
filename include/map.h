#ifndef MAP_H
#define MAP_H

#include "game.h"

#define MAP_WIDTH 40
#define MAP_HEIGHT 25

typedef enum {
    TILE_FLOOR = ' ',
    TILE_WALL = '#',
    TILE_DOOR = '+',
    TILE_STAIRS = '>'
} TileType;

typedef struct {
    TileType tiles[MAP_HEIGHT][MAP_WIDTH];
} Map;

void InitMap(Map *map);
void DrawMap(const Map *map);

#endif
