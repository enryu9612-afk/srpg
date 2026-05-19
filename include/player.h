#ifndef PLAYER_H
#define PLAYER_H

#include "map.h"

typedef struct {
    int x;
    int y;
    char symbol;
} Player;

void InitPlayer(Player *player, int startX, int startY);
void MovePlayer(Player *player, int dx, int dy, const Map *map);

#endif
