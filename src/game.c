#include "game.h"

void DrawTile(int x, int y, char symbol) {
    // 그리드 좌표 (x, y)를 픽셀 좌표로 변환
    int pixelX = x * TILE_SIZE;
    int pixelY = y * TILE_SIZE;

    // 문자를 출력 (심볼을 문자열로 변환하여 출력)
    DrawText(TextFormat("%c", symbol), pixelX, pixelY, TILE_SIZE, RAYWHITE);
}
