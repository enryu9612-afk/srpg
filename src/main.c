#include "raylib.h"
#include "game.h"
#include "map.h"
#include "player.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Roguelike SRPG");
    SetTargetFPS(60);

    Map gameMap;
    InitMap(&gameMap);

    Player player;
    InitPlayer(&player, 5, 5); // 시작 위치 (5, 5)

    while (!WindowShouldClose()) {
        // 1. 입력 처리 (Input)
        if (IsKeyPressed(KEY_UP))    MovePlayer(&player, 0, -1, &gameMap);
        if (IsKeyPressed(KEY_DOWN))  MovePlayer(&player, 0, 1, &gameMap);
        if (IsKeyPressed(KEY_LEFT))  MovePlayer(&player, -1, 0, &gameMap);
        if (IsKeyPressed(KEY_RIGHT)) MovePlayer(&player, 1, 0, &gameMap);

        // 2. 렌더링 (Rendering)
        BeginDrawing();
        ClearBackground(BLACK);

        DrawMap(&gameMap);
        DrawTile(player.x, player.y, player.symbol);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
