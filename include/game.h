#ifndef GAME_H
#define GAME_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define UI_HEIGHT 150
#define GAME_HEIGHT (SCREEN_HEIGHT - UI_HEIGHT)

#define TILE_SIZE 20

// 이름을 GameCamera로 변경하여 raylib의 Camera와 충돌 방지
typedef struct {
    int x;
    int y;
} GameCamera;

void DrawTile(int x, int y, char symbol, Color color, GameCamera cam);

#endif
