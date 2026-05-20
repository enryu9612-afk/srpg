#include "game.h"
#include "raylib.h"

void DrawTile(int x, int y, char symbol, Color color, GameCamera cam) {
    // 세계 좌표 -> 화면 좌표 변환 (카메라 오프셋 적용)
    int screenX = (x - cam.x) * TILE_SIZE;
    int screenY = (y - cam.y) * TILE_SIZE;

    // 화면 범위 밖의 타일은 그리지 않음 (최적화)
    if (screenX < 0 || screenX >= SCREEN_WIDTH || screenY < 0 || screenY >= GAME_HEIGHT) {
        return;
    }

    // 가변폭 폰트 뭉침 방지를 위해 x 좌표에 +2 픽셀 여백 추가
    DrawText(TextFormat("%c", symbol), screenX + 2, screenY, TILE_SIZE, color);
}
