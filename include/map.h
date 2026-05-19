#ifndef MAP_H
#define MAP_H

#include "game.h"

// 화면보다 훨씬 큰 맵 크기 설정
#define MAP_WIDTH 100
#define MAP_HEIGHT 100

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
void DrawMap(const Map *map, Camera cam);

#endif
