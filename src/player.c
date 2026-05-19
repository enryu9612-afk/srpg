#include "player.h"

void InitPlayer(Player *player, int startX, int startY) {
    InitEntity(&player->base, ENTITY_PLAYER, startX, startY, '@');
    player->exp = 0;
    player->gold = 0;
}

void MovePlayer(Player *player, int dx, int dy, const Map *map) {
    int nextX = player->base.x + dx;
    int nextY = player->base.y + dy;

    if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
        if (map->tiles[nextY][nextX] != TILE_WALL) {
            player->base.x = nextX;
            player->base.y = nextY;
        }
    }
}
