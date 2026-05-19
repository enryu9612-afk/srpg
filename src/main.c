#include "raylib.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "entity.h"

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Roguelike SRPG - Strategic View");
    SetTargetFPS(60);

    Map gameMap;
    InitMap(&gameMap);

    Player player;
    InitPlayer(&player, 10, 10);

    Camera cam = {0, 0};

    while (!WindowShouldClose()) {
        // 1. 입력 처리
        if (IsKeyPressed(KEY_UP))    MovePlayer(&player, 0, -1, &gameMap);
        if (IsKeyPressed(KEY_DOWN))  MovePlayer(&player, 0, 1, &gameMap);
        if (IsKeyPressed(KEY_LEFT))  MovePlayer(&player, -1, 0, &gameMap);
        if (IsKeyPressed(KEY_RIGHT)) MovePlayer(&player, 1, 0, &gameMap);

        // 2. 카메라 업데이트 (플레이어를 화면 중앙에 배치)
        // 중앙 좌표: (화면폭/타일크기)/2
        cam.x = player.base.x - (SCREEN_WIDTH / (2 * TILE_SIZE));
        cam.y = player.base.y - (GAME_HEIGHT / (2 * TILE_SIZE));

        // 카메라 맵 경계 제한 (Clamping)
        if (cam.x < 0) cam.x = 0;
        if (cam.y < 0) cam.y = 0;
        if (cam.x > MAP_WIDTH - (SCREEN_WIDTH / TILE_SIZE)) cam.x = MAP_WIDTH - (SCREEN_WIDTH / TILE_SIZE);
        if (cam.y > MAP_HEIGHT - (GAME_HEIGHT / TILE_SIZE)) cam.y = MAP_HEIGHT - (GAME_HEIGHT / TILE_SIZE);

        // 3. 렌더링
        BeginDrawing();
        ClearBackground(BLACK);

        // 상단: 게임 화면
        DrawMap(&gameMap, cam);
        DrawTile(player.base.x, player.base.y, player.base.symbol, cam);

        // 하단: UI 조작창
        DrawRectangle(0, GAME_HEIGHT, SCREEN_WIDTH, UI_HEIGHT, DARKGRAY);
        DrawRectangleLines(0, GAME_HEIGHT, SCREEN_WIDTH, UI_HEIGHT, WHITE);
        DrawText("Unit: Player | HP: 100/100 | ATK: I | DEF: I", 20, GAME_HEIGHT + 20, 20, RAYWHITE);
        DrawText("Command: [Arrow Keys] Move | [TBD] Attack", 20, GAME_HEIGHT + 50, 20, LIGHTGRAY);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
