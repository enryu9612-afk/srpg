#include "core/core.h"
#include "ui/ui.h"
#include "map/map.h"
#include "entity/entity.h"
#include "battle/battle.h"
#include "growth/growth.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void) {
    printf("[Main] Starting Roguelike SRPG - Phase 5 Integration...\\n");

    if (!Core_Init()) {
        fprintf(stderr, "[Main Error] Core initialization failed. Exiting.\\n");
        return 1;
    }
    Core_InitCamera();
    UI_Init();

    Map* game_map = Map_Create(60, 30);
    if (!Map_Generate(game_map, (uint32_t)time(NULL))) {
        fprintf(stderr, "[Main Error] Map generation failed. Exiting.\\n");
        return 1;
    }
    UI_AddLog("World generated successfully.");

    Operator player;
    Operator_Init(&player, 1, 0, 0);
    
    Enemy enemy;
    Enemy_Init(&enemy, 101, 0, 0, 1);

    bool found_start = false;
    for (int32_t y = 0; y < game_map->height && !found_start; y++) {
        for (int32_t x = 0; x < game_map->width && !found_start; x++) {
            if (Map_IsWalkable(game_map, x, y)) {
                player.base.x = x;
                player.base.y = y;
                int32_t ex, ey;
                if (Map_FindRoomSpawnPos(game_map, x, y, &ex, &ey)) {
                    enemy.base.x = ex;
                    enemy.base.y = ey;
                } else {
                    if (!Map_FindSafeSpawnPos(game_map, x, y, &ex, &ey)) {
                        enemy.base.x = x + 1;
                        enemy.base.y = y;
                    } else {
                        enemy.base.x = ex;
                        enemy.base.y = ey;
                    }
                }
                found_start = true;
            }
        }
    }
    UI_AddLog("Operator deployed to the sector.");
    UI_AddLog("A hostile enemy has appeared nearby!");

    Core_UpdateCamera(player.base.x, player.base.y);

    while (g_game_state.is_running) {
        Core_Update();

        if (enemy.hp > 0) {
            if (Battle_CheckRange(&player.base, &enemy.base, 5)) {
                if (!g_battle_state.is_combat_active) {
                    g_battle_state.is_combat_active = true;
                    UI_AddLog("!!! COMBAT STARTED !!!");
                }
            } else {
                if (g_battle_state.is_combat_active) {
                    g_battle_state.is_combat_active = false;
                    UI_AddLog("Combat ended. Returning to exploration mode.");
                }
            }
        } else {
            if (g_battle_state.is_combat_active) {
                g_battle_state.is_combat_active = false;
                UI_AddLog("Enemy defeated! Exploration mode restored.");
            }
        }

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
                    if (next_x == enemy.base.x && next_y == enemy.base.y) {
                        UI_AddLog("Blocked by an enemy!");
                    } else {
                        player.base.x = next_x;
                        player.base.y = next_y;
                        Core_UpdateCamera(player.base.x, player.base.y);
                        UI_AddLog("Moving...");
                        if (g_battle_state.is_combat_active) {
                            Battle_NextTurn();
                        }
                    }
                } else {
                    UI_AddLog("Blocked by a wall!");
                }
                
                if (g_battle_state.is_combat_active) {
                    Battle_UpdateStatusEffects(&player.base);
                }
            }

            if (IsKeyPressed(KEY_SPACE)) {
                int32_t result = Battle_ExecuteAttack(&player, (Entity*)&enemy);
                if (result > 0) {
                    char buf[64];
                    sprintf(buf, "Attack Hit! Dealt %d damage. Enemy HP: %d", result, enemy.hp);
                    UI_AddLog(buf);
                    Battle_NextTurn(); 
                } else if (result == 0) {
                    UI_AddLog("Attack Missed!");
                    Battle_NextTurn(); 
                } else if (result == -1) {
                    UI_AddLog("Target is too far away!");
                } else if (result == -2) {
                    UI_AddLog("Invalid attack target!");
                }
            }
        }

        if (g_battle_state.current_turn != BATTLE_TURN_PLAYER && enemy.hp > 0) {
            static float enemy_timer = 0;
            enemy_timer += GetFrameTime();
            if (enemy_timer >= 0.1f) { 
                UI_AddLog("Enemy is thinking...");
                Battle_UpdateEnemyAI(&enemy, &player, game_map);
                Battle_UpdateStatusEffects(&enemy.base);
                Battle_NextTurn();
                enemy_timer = 0;
            }
        }

        // --- Character Menu Logic ---
        if (IsKeyPressed(KEY_C)) {
            g_ui_context.is_open = !g_ui_context.is_open;
            UI_AddLog(g_ui_context.is_open ? "Menu Opened" : "Menu Closed");
        }

        Core_Draw();
        BeginMode2D(g_game_camera.camera);

        for (int32_t y = 0; y < game_map->height; y++) {
            for (int32_t x = 0; x < game_map->width; x++) {
                int32_t dx = x - player.base.x;
                int32_t dy = y - player.base.y;
                int32_t distSq = dx*dx + dy*dy;
                Color color;
                const char* symbol = " ";
                int wallFontSize = 12;
                int floorFontSize = 8;
                int offset_x = (TILE_SIZE - 8) / 2; 
                int offset_y = (TILE_SIZE - 12) / 2;

                if (game_map->tiles[y * game_map->width + x] == TILE_WALL) {
                    bool top = (y > 0 && game_map->tiles[(y-1) * game_map->width + x] == TILE_WALL);
                    bool bot = (y < game_map->height-1 && game_map->tiles[(y+1) * game_map->width + x] == TILE_WALL);
                    bool lft = (x > 0 && game_map->tiles[y * game_map->width + (x-1)] == TILE_WALL);
                    bool rgt = (x < game_map->width-1 && game_map->tiles[y * game_map->width + (x+1)] == TILE_WALL);

                    if (top && bot && lft && rgt) symbol = "#";
                    else if (top && bot) symbol = "|";
                    else if (lft && rgt) symbol = "#";
                    else if (top || bot || lft || rgt) symbol = "+";
                    else symbol = "#";

                    if (distSq < 100) color = LIGHTGRAY;
                    else if (distSq < 400) color = DARKGRAY;
                    else color = (Color){40, 40, 40, 255};
                    
                    DrawText(symbol, x * TILE_SIZE + offset_x, y * TILE_SIZE + offset_y, wallFontSize, color);
                } else {
                    color = (distSq < 100) ? GRAY : (distSq < 400) ? (Color){70, 70, 70, 255} : (Color){30, 30, 30, 255};
                    uint32_t detail_seed = (x * 12345) ^ (y * 67890);
                    if (detail_seed % 100 < 2) { symbol = "*"; color = GOLD; } 
                    else if (detail_seed % 100 < 4) { symbol = "o"; color = LIME; } 
                    else if (detail_seed % 100 < 6) { symbol = "~"; color = SKYBLUE; } 
                    else { symbol = ".";}
                    
                    DrawText(symbol, x * TILE_SIZE + offset_x, y * TILE_SIZE + offset_y, floorFontSize, color);
                }
            }
        }

        if (enemy.hp > 0) {
            DrawText("E", enemy.base.x * TILE_SIZE + (TILE_SIZE-10)/2, enemy.base.y * TILE_SIZE + (TILE_SIZE-10)/2, TILE_SIZE - 6, RED);
        }

        if (player.base.x >= 0 && player.base.y >= 0) {
            DrawText("@", player.base.x * TILE_SIZE + (TILE_SIZE-10)/2, player.base.y * TILE_SIZE + (TILE_SIZE-10)/2, TILE_SIZE - 6, BLUE);
        }

        EndMode2D();
        UI_DrawLogPanel();
        
        if (g_ui_context.is_open) {
            UI_DrawCharacterMenu();
        }
        Core_EndDraw();
    }

    Map_Destroy(game_map);
    Core_Shutdown();
    printf("[Main] Game exited cleanly.\\n");

    return 0;
}
