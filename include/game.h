#ifndef GAME_H
#define GAME_H

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define UI_HEIGHT 150
#define GAME_HEIGHT (SCREEN_HEIGHT - UI_HEIGHT)

#define TILE_SIZE 20

// 카메라 오프셋 구조체
typedef struct {
    int x;
    int y;
} Camera;

void DrawTile(int x, int y, char symbol, Camera cam);

#endif
