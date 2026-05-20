#include "raylib.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "entity.h"
#include "combat.h"
#include "ai.h"
#include "ui.h"
#include "skill.h"
#include "floor.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef enum {
    STATE_MOVE,
    STATE_TARGETING,
    STATE_ENEMY_TURN
} ControlState;

int main(void) {
    srand(time(NULL)); // 랜덤 시드 초기화
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Roguelike SRPG - Strategic Combat");
    SetTargetFPS(60);

    // 층 관리자 도입
    FloorManager fm;
    InitFloorManager(&fm);

    Party party;
    InitParty(&party);

    // 현재 층의 적들을 관리할 배열
    Entity enemies[10]; 
    
    // 적 생성 함수 (임시)
    void SpawnEnemies(Entity *enemies, int count, Map *map) {
        for(int i = 0; i < count; i++) {
            int rx, ry;
            do {
                rx = rand() % MAP_WIDTH;
                ry = rand() % MAP_HEIGHT;
            } while(map->tiles[ry][rx] != TILE_FLOOR);
            InitEntity(&enemies[i], 100 + i, ENTITY_MONSTER, rx, ry, 'M', 1);
        }
    }
    SpawnEnemies(enemies, 5, &fm.current_map);

    GameCamera cam = {0, 0};
    ControlState current_state = STATE_MOVE;
    
    int targetX = 0, targetY = 0;
    Entity *selected_target = NULL;
    Equipment default_sword = {WEAPON_SWORD, 1, 10.0f, 0.0f, 0.0f};

    while (!WindowShouldClose()) {
        float delta = GetFrameTime();
        PlayerUnit *active = GetActiveUnit(&party);

        if (IsKeyPressed(KEY_TAB)) {
            party.current_unit_idx = (party.current_unit_idx + 1) % party.count;
            current_state = STATE_MOVE;
        }

        if (current_state == STATE_MOVE) {
            if (IsKeyPressed(KEY_UP))    MoveActiveUnit(&party, 0, -1, &fm.current_map);
            if (IsKeyPressed(KEY_DOWN))  MoveActiveUnit(&party, 0, 1, &fm.current_map);
            if (IsKeyPressed(KEY_LEFT))  MoveActiveUnit(&party, -1, 0, &fm.current_map);
            if (IsKeyPressed(KEY_RIGHT)) MoveActiveUnit(&party, 1, 0, &fm.current_map);

            if (IsKeyPressed(KEY_Z)) {
                current_state = STATE_TARGETING;
                targetX = active->base.x;
                targetY = active->base.y;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                for(int i = 0; i < party.count; i++) UpdateStatusEffects(&party->members[i].base);
                for(int i = 0; i < 10; i++) UpdateStatusEffects(&enemies[i]);
                current_state = STATE_ENEMY_TURN;
                AddLog("--- Enemy Turn Starts ---");
            }
            
            // [추가] 계단/출구 도달 시 다음 층으로 이동 (임시: 맵 오른쪽 하단 끝에 도달 시)
            if (active->base.x >= MAP_WIDTH - 2 && active->base.y >= MAP_HEIGHT - 2) {
                TransitionToNextFloor(&fm, &party);
                SpawnEnemies(enemies, 5, &fm.current_map);
                AddLog("Descending to the next floor...");
            }
        } else if (current_state == STATE_TARGETING) {
            if (IsKeyPressed(KEY_UP))    targetY--;
            if (IsKeyPressed(KEY_DOWN))  targetY++;
            if (IsKeyPressed(KEY_LEFT))  targetX--;
            if (IsKeyPressed(KEY_RIGHT)) targetX++;

            if (targetX < 0) targetX = 0;
            if (targetY < 0) targetY = 0;
            if (targetX >= MAP_WIDTH) targetX = MAP_WIDTH - 1;
            if (targetY >= MAP_HEIGHT) targetY = MAP_HEIGHT - 1;

            selected_target = NULL;
            for (int i = 0; i < 10; i++) {
                if (enemies[i].is_alive && enemies[i].x == targetX && enemies[i].y == targetY) {
                    selected_target = &enemies[i];
                    break;
                }
            }

            if (IsKeyPressed(KEY_Z)) {
                if (selected_target) {
                    if (UseSkill(active, selected_target, 0)) {
                        // 성공
                    } else {
                        AddLog("Skill failed!");
                    }
                } else {
                    AddLog("No target selected!");
                }
                current_state = STATE_MOVE;
            }
            if (IsKeyPressed(KEY_X)) {
                current_state = STATE_MOVE;
            }
        } else if (current_state == STATE_ENEMY_TURN) {
            ExecuteEnemyTurn(enemies, 10, &party, &fm.current_map);
            current_state = STATE_MOVE;
            AddLog("--- Your Turn Starts ---");
        }

        cam.x = active->base.x - (SCREEN_WIDTH / (2 * TILE_SIZE));
        cam.y = active->base.y - (GAME_HEIGHT / (2 * TILE_SIZE));
        if (cam.x < 0) cam.x = 0;
        if (cam.y < 0) cam.y = 0;
        if (cam.x > MAP_WIDTH - (SCREEN_WIDTH / TILE_SIZE)) cam.x = MAP_WIDTH - (SCREEN_WIDTH / TILE_SIZE);
        if (cam.y > MAP_HEIGHT - (GAME_HEIGHT / TILE_SIZE)) cam.y = MAP_HEIGHT - (GAME_HEIGHT / TILE_SIZE);

        UpdateFloatingTexts(delta);

        BeginDrawing();
        ClearBackground(BLACK);
        DrawMap(&fm.current_map, cam);
        for (int i = 0; i < party.count; i++) {
            DrawTile(party->members[i].base.x, party->members[i].base.y, party->members[i].base.symbol, cam);
        }
        for (int i = 0; i < 10; i++) {
            if (enemies[i].is_alive) {
                DrawTile(enemies[i].x, enemies[i].y, enemies[i].symbol, cam);
            }
        }
        if (current_state == STATE_TARGETING) {
            DrawTile(targetX, targetY, 'X', cam);
        }

        DrawFloatingTexts(cam);
        const char* stateStr = (current_state == STATE_MOVE) ? "MOVE" : (current_state == STATE_TARGETING ? "TARGETING" : "ENEMY TURN");
        DrawUI(active, &global_log, stateStr);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
