#include "core/core.h"
#include "ui/ui.h"
#include "map/map.h"
#include "entity/entity.h"
#include "battle/battle.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    printf("[Main] Starting Roguelike SRPG - Phase 2 Integration...\n");

    // 1. Infrastructure Initialization
    if (!Core_Init()) {
        fprintf(stderr, "[Main Error] Core initialization failed. Exiting.\n");
        return 1;
    }
    Core_InitCamera();
    UI_Init();

    // 2. World Generation
    Map* game_map = Map_Create(60, 30);
    if (!Map_Generate(game_map, 12345)) {
        fprintf(stderr, "[Main Error] Map generation failed. Exiting.\n");
        return 1;
    }
    UI_AddLog("World generated successfully.");

    // 3. Player Initialization
    Operator player;
    Operator_Init(&player, 1, 0, 0);
    
    // 3-1. Enemy Initialization (Test)
    Enemy enemy;
    Enemy_Init(&enemy, 101, 0, 0, 1); // Level 1 Enemy

    // Set player starting position to the first available floor tile
    bool found_start = false;
    for (int32_t y = 0; y < game_map->height && !found_start; y++) {
        for (int32_t x = 0; x < game_map->width && !found_start; x++) {
            if (Map_IsWalkable(game_map, x, y)) {
                player.base.x = x;
                player.base.y = y;
                
                // Safe Enemy Spawn: Use verified domain function
                int32_t ex, ey;
                if (Map_FindSafeSpawnPos(game_map, x, y, &ex, &ey)) {
                    enemy.base.x = ex;
                    enemy.base.y = ey;
                } else {
                    // Fallback: just place it (should rarely happen with BSP)
                    enemy.base.x = x + 1;
                    enemy.base.y = y;
                }
                
                found_start = true;
            }
        }
    }
    UI_AddLog("Operator deployed to the sector.");
    UI_AddLog("A hostile enemy has appeared nearby!");

    // Initial camera update
    Core_UpdateCamera(player.base.x, player.base.y);

    // 4. Main Game Loop
    while (g_game_state.is_running) {
        // --- Update ---
        Core_Update();

        // Player Turn Logic
        if (g_battle_state.current_turn == BATTLE_TURN_PLAYER) {
            int32_t dx = 0, dy = 0;
            if (IsKeyPressed(KEY_UP))    dy = -1;
            else if (IsKeyPressed(KEY_DOWN))  dy = 1;
            else if (IsKeyPressed(KEY_LEFT))  dx = -1;
            else if (IsKeyPressed(KEY_RIGHT)) dx = 1;

            if (dx != 0 || dy != 0) {
                int32_t next_x = player.base.x + dx;
                int32_t next_y = player.base.y + dy;

                if (Map_IsWalkable(game_map, next_x, next_y)) {
                    player.base.x = next_x;
                    player.base.y = next_y;
                    Core_UpdateCamera(player.base.x, player.base.y);
                    UI_AddLog("Moving...");
                    Battle_NextTurn(); // End turn after moving
                } else {
                    UI_AddLog("Blocked by a wall!");
                }
            }

            // Attack Input (SPACE)
            if (IsKeyPressed(KEY_SPACE)) {
                int32_t result = Battle_ExecuteAttack(&player, (Entity*)&enemy);
                if (result > 0) {
                    char buf[64];
                    sprintf(buf, "Attack Hit! Dealt %d damage. Enemy HP: %d", result, enemy.hp);
                    UI_AddLog(buf);
                    Battle_NextTurn(); // End turn after attacking
                } else if (result == 0) {
                    UI_AddLog("Attack Missed!");
                    Battle_NextTurn(); // Miss still consumes a turn
                } else if (result == -1) {
                    UI_AddLog("Target is too far away!");
                } else if (result == -2) {
                    UI_AddLog("Invalid attack target!");
                }
            }
        } // <--- Fixed: Closing Player Turn Logic block
        
        // Enemy Turn Logic (Simple AI Placeholder)
        if (g_battle_state.current_turn != BATTLE_TURN_PLAYER && enemy.hp > 0) {
            static float enemy_timer = 0;
            enemy_timer += GetFrameTime();
            if (enemy_timer >= 1.0f) { // Act every 1 second
                UI_AddLog("Enemy is thinking...");
                
                int32_t next_x = enemy.base.x;
                int32_t next_y = enemy.base.y;
                
                // Basic AI: move towards player
                if (enemy.base.x < player.base.x) next_x++;
                else if (enemy.base.x > player.base.x) next_x--;
                else if (enemy.base.y < player.base.y) next_y++;
                else if (enemy.base.y > player.base.y) next_y--;
                
                // Wall Collision Check
                if (Map_IsWalkable(game_map, next_x, next_y)) {
                    enemy.base.x = next_x;
                    enemy.base.y = next_y;
                    UI_AddLog("Enemy moved.");
                } else {
                    UI_AddLog("Enemy is blocked by a wall.");
                }
                
                Battle_NextTurn();
                enemy_timer = 0;
            }
        }

        // --- Draw ---
        Core_Draw();

        // Render Map
        for (int32_t y = 0; y < game_map->height; y++) {
            for (int32_t x = 0; x < game_map->width; x++) {
                Color tile_color = (game_map->tiles[y * game_map->width + x] == TILE_WALL) ? DARKGRAY : GRAY;
                DrawRectangle(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE - 1, TILE_SIZE - 1, tile_color);
            }
        }

        // Render Enemy
        if (enemy.hp > 0) {
            DrawRectangle(enemy.base.x * TILE_SIZE, enemy.base.y * TILE_SIZE, TILE_SIZE - 1, TILE_SIZE - 1, RED);
            DrawText("E", enemy.base.x * TILE_SIZE + 8, enemy.base.y * TILE_SIZE + 4, 20, WHITE);
        }

        // Render Player
        DrawRectangle(player.base.x * TILE_SIZE, player.base.y * TILE_SIZE, TILE_SIZE - 1, TILE_SIZE - 1, BLUE);
        DrawText("@", player.base.x * TILE_SIZE + 8, player.base.y * TILE_SIZE + 4, 20, WHITE);

        // Render UI (Screen-space) - Must be drawn last to be on top
        UI_DrawLogPanel();

        Core_EndDraw();
    }

    // 5. Shutdown
    Map_Destroy(game_map);
    Core_Shutdown();
    printf("[Main] Game exited cleanly.\n");

    return 0;
}
