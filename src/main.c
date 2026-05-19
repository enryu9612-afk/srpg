#include "raylib.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "entity.h"
#include "combat.h"
#include <stdlib.h>
#include <time.h>

#define MAX_MONSTERS 5

int main(void) {
    srand(time(NULL));
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Roguelike SRPG");
    SetTargetFPS(60);

    Map gameMap;
    InitMap(&gameMap);

    Player player;
    InitPlayer(&player, 5, 5);

    Entity monsters[MAX_MONSTERS];
    for (int i = 0; i < MAX_MONSTERS; i++) {
        int mx = (rand() % (MAP_WIDTH - 2)) + 1;
        int my = (rand() % (MAP_HEIGHT - 2)) + 1;
        // 벽이 아닌 곳에 배치
        while (gameMap.tiles[my][mx] == TILE_WALL) {
            mx = (rand() % (MAP_WIDTH - 2)) + 1;
            my = (rand() % (MAP_HEIGHT - 2)) + 1;
        }
        InitEntity(&monsters[i], ENTITY_MONSTER, mx, my, 'M');
    }

    while (!WindowShouldClose()) {
        // 1. 입력 처리 및 이동/전투
        int dx = 0, dy = 0;
        if (IsKeyPressed(KEY_UP))    dy = -1;
        if (IsKeyPressed(KEY_DOWN))  dy = 1;
        if (IsKeyPressed(KEY_LEFT))  dx = -1;
        if (IsKeyPressed(KEY_RIGHT)) dx = 1;

        if (dx != 0 || dy != 0) {
            int nextX = player.base.x + dx;
            int nextY = player.base.y + dy;
            
            // 이동하려는 칸에 몬스터가 있는지 확인
            int monsterIdx = -1;
            for (int i = 0; i < MAX_MONSTERS; i++) {
                if (monsters[i].is_alive && monsters[i].x == nextX && monsters[i].y == nextY) {
                    monsterIdx = i;
                    break;
                }
            }

            if (monsterIdx != -1) {
                // 전투 발생!
                ResolveCombat(&player.base, &monsters[monsterIdx], ATTACK_PHYSICAL);
                if (!monsters[monsterIdx].is_alive) {
                    // 몬스터 처치 시 이동 가능하게 하거나 추가 처리
                }
            } else {
                // 몬스터가 없으면 일반 이동
                MovePlayer(&player, dx, dy, &gameMap);
            }
        }

        // 2. 렌더링
        BeginDrawing();
        ClearBackground(BLACK);

        DrawMap(&gameMap);
        
        for (int i = 0; i < MAX_MONSTERS; i++) {
            if (monsters[i].is_alive) {
                DrawTile(monsters[i].x, monsters[i].y, monsters[i].symbol);
            }
        }
        
        DrawTile(player.base.x, player.base.y, player.base.symbol);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
