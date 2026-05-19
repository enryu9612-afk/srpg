#include "player.h"

void InitPlayer(Player *player, int startX, int startY) {
    player->x = startX;
    player->y = startY;
    player->symbol = '@'; // 로그라이크 전통의 플레이어 심볼
}

void MovePlayer(Player *player, int dx, int dy, const Map *map) {
    int nextX = player->x + dx;
    int nextY = player->y + dy;

    // 맵 경계 체크 및 충돌 판정 (벽이면 이동 불가)
    if (nextX >= 0 && nextX < MAP_WIDTH && nextY >= 0 && nextY < MAP_HEIGHT) {
        if (map->tiles[nextY][nextX] != TILE_WALL) {
            player->x = nextX;
            player->y = nextY;
        }
    }
}
