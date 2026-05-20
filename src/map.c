#include "map.h"
#include "game.h"
#include "raylib.h"

void InitMap(Map *map) {
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            if (x == 0 || x == MAP_WIDTH - 1 || y == 0 || y == MAP_HEIGHT - 1) {
                map->tiles[y][x] = TILE_WALL;
            } else {
                map->tiles[y][x] = TILE_FLOOR;
            }
        }
    }
}

void DrawMap(const Map *map, GameCamera cam) {
    // 화면에 보이는 영역만 계산하여 렌더링 (Viewport Optimization)
    int startX = cam.x;
    int startY = cam.y;
    int endX = cam.x + (SCREEN_WIDTH / TILE_SIZE) + 1;
    int endY = cam.y + (GAME_HEIGHT / TILE_SIZE) + 1;

    // 맵 경계 제한
    if (startX < 0) startX = 0;
    if (startY < 0) startY = 0;
    if (endX > MAP_WIDTH) endX = MAP_WIDTH;
    if (endY > MAP_HEIGHT) endY = MAP_HEIGHT;

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            DrawTile(x, y, map->tiles[y][x], WHITE, cam);
        }
    }
}
