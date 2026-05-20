#include "raylib.h"
#include "game.h"
#include "map.h"
#include "player.h"
#include "entity.h"
#include "combat.h"
#include "ai.h"
#include <stdio.h>
#include <stdlib.h>

typedef enum {
    STATE_MOVE,
    STATE_TARGETING,
    STATE_ENEMY_TURN
} ControlState;

int main(void) {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Roguelike SRPG - Strategic Combat");
    SetTargetFPS(60);

    Map gameMap;
    InitMap(&gameMap);

    Party party;
    InitParty(&party);

    Entity enemies[5];
    for(int i = 0; i < 5; i++) {
        InitEntity(&enemies[i], 100 + i, ENTITY_MONSTER, 15 + (i * 2), 12, 'M', 1);
    }

    GameCamera cam = {0, 0};
    ControlState current_state = STATE_MOVE;
    
    int targetX = 0, targetY = 0;
    Entity *selected_target = NULL;
    Equipment default_sword = {WEAPON_SWORD, 1, 10.0f, 0.0f, 0.0f};

    while (!WindowShouldClose()) {
        PlayerUnit *active = GetActiveUnit(&party);

        // 1. 입력 처리
        if (IsKeyPressed(KEY_TAB)) {
            party.current_unit_idx = (party.current_unit_idx + 1) % party.count;
            current_state = STATE_MOVE;
        }

        if (current_state == STATE_MOVE) {
            if (IsKeyPressed(KEY_UP))    MoveActiveUnit(&party, 0, -1, &gameMap);
            if (IsKeyPressed(KEY_DOWN))  MoveActiveUnit(&party, 0, 1, &gameMap);
            if (IsKeyPressed(KEY_LEFT))  MoveActiveUnit(&party, -1, 0, &gameMap);
            if (IsKeyPressed(KEY_RIGHT)) MoveActiveUnit(&party, 1, 0, &gameMap);

            if (IsKeyPressed(KEY_Z)) {
                current_state = STATE_TARGETING;
                targetX = active->base.x;
                targetY = active->base.y;
            }
            if (IsKeyPressed(KEY_ENTER)) {
                current_state = STATE_ENEMY_TURN;
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
            for (int i = 0; i < 5; i++) {
                if (enemies[i].is_alive && enemies[i].x == targetX && enemies[i].y == targetY) {
                    selected_target = &enemies[i];
                    break;
                }
            }

            if (IsKeyPressed(KEY_Z)) {
                if (selected_target && CanAttack(&active->base, selected_target, &default_sword)) {
                    ResolveCombat(&active->base, selected_target, &default_sword, ATTACK_PHYSICAL, active);
                    printf("Attacked Enemy! Damage dealt.\n");
                }
                current_state = STATE_MOVE;
            }
            if (IsKeyPressed(KEY_X)) {
                current_state = STATE_MOVE;
            }
        } else if (current_state == STATE_ENEMY_TURN) {
            // 적군 턴 실행 (Map 정보 전달)
            ExecuteEnemyTurn(enemies, 5, &party, &gameMap);
            current_state = STATE_MOVE;
        }

        // 2. 카메라 업데이트
        cam.x = active->base.x - (SCREEN_WIDTH / (2 * TILE_SIZE));
        cam.y = active->base.y - (GAME_HEIGHT / (2 * TILE_SIZE));
        if (cam.x < 0) cam.x = 0;
        if (cam.y < 0) cam.y = 0;
        if (cam.x > MAP_WIDTH - (SCREEN_WIDTH / TILE_SIZE)) cam.x = MAP_WIDTH - (SCREEN_WIDTH / TILE_SIZE);
        if (cam.y > MAP_HEIGHT - (GAME_HEIGHT / TILE_SIZE)) cam.y = MAP_HEIGHT - (GAME_HEIGHT / TILE_SIZE);

        // 3. 렌더링
        BeginDrawing();
        ClearBackground(BLACK);
        DrawMap(&gameMap, cam);
        for (int i = 0; i < party.count; i++) {
            DrawTile(party.members[i].base.x, party.members[i].base.y, party.members[i].base.symbol, cam);
        }
        for (int i = 0; i < 5; i++) {
            if (enemies[i].is_alive) {
                DrawTile(enemies[i].x, enemies[i].y, enemies[i].symbol, cam);
            }
        }
        if (current_state == STATE_TARGETING) {
            DrawTile(targetX, targetY, 'X', cam);
        }

        // 하단 UI
        DrawRectangle(0, GAME_HEIGHT, SCREEN_WIDTH, UI_HEIGHT, DARKGRAY);
        DrawRectangleLines(0, GAME_HEIGHT, SCREEN_WIDTH, UI_HEIGHT, WHITE);
        
        char statusText[128];
        const char* stateStr = (current_state == STATE_MOVE) ? "MOVE" : (current_state == STATE_TARGETING ? "TARGETING" : "ENEMY TURN");
        sprintf(statusText, "Unit: %d | HP: %d/%d | State: %s", 
                party.current_unit_idx + 1, active->base.hp_cur, active->base.hp_max, stateStr);
        DrawText(statusText, 20, GAME_HEIGHT + 20, 20, RAYWHITE);

        if (current_state == STATE_MOVE) {
            DrawText("Cmd: [Arrows] Move | [TAB] Switch | [Z] Attack | [Enter] End Turn", 20, GAME_HEIGHT + 50, 20, LIGHTGRAY);
        } else if (current_state == STATE_TARGETING) {
            DrawText("Cmd: [Arrows] Target | [Z] Confirm | [X] Cancel", 20, GAME_HEIGHT + 50, 20, YELLOW);
        } else {
            DrawText("Enemies are moving...", 20, GAME_HEIGHT + 50, 20, RED);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
