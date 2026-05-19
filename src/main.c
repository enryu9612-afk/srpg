#include "game.h"
#include <stdio.h>

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Roguelike SRPG - Render Verification");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        // 3x3 크기의 # 문자 사각형 그리기 테스트
        for (int x = 10; x < 13; x++) {
            for (int y = 10; y < 13; y++) {
                DrawTile(x, y, '#');
            }
        }

        DrawText("Rendering 3x3 grid test", 10, 10, 20, GRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
