#include "core/core.h"
#include "ui/ui.h"
#include "map/map.h"
#include "entity/entity.h"
#include "battle/battle.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
 
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
    if (!Map_Generate(game_map, (uint32_t)time(NULL))) {
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
                
                // Safe Enemy Spawn: Place enemy in a different room from the player
                int32_t ex, ey;
                if (Map_FindRoomSpawnPos(game_map, x, y, &ex, &ey)) {
                    enemy.base.x = ex;
                    enemy.base.y = ey;
                } else {
                    // Fallback: if different room spawn fails, use the safe nearby pos
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
 
    // Initial camera update
    Core_UpdateCamera(player.base.x, player.base.y);
 
    // 4. Main Game Loop
    while (g_game_state.is_running) {
        // --- Update ---
        Core_Update();
 
        // Combat Trigger Check
        if (enemy.hp > 0) {
            // Distance-based combat activation (Manhattan distance <= 5)
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
            // Enemy died -> exit combat mode
            if (g_battle_state.is_combat_active) {
                g_battle_state.is_combat_active = false;
                UI_AddLog("Enemy defeated! Exploration mode restored.");
            }
        }
 
        // Player Input & Movement
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
                        // Collision check with enemy
                        if (next_x == enemy.base.x && next_y == enemy.base.y) {
                            UI_AddLog("Blocked by an enemy!");
                        } else {
                            player.base.x = next_x;
                            player.base.y = next_y;
                            Core_UpdateCamera(player.base.x, player.base.y);
                            UI_AddLog("Moving...");
                            
                            // Only end turn if in COMBAT mode. In EXPLORATION mode, move freely.
                            if (g_battle_state.is_combat_active) {
                                Battle_NextTurn();
                            }
                        }
                    } else {
                        UI_AddLog("Blocked by a wall!");
                    }
                    
                    // After player action, update status effects for the player
                    if (g_battle_state.is_combat_active) {
                        Battle_UpdateStatusEffects(&player.base);
                    }
                }
            }
 
            // Attack Input (SPACE)
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
 
        // Enemy Turn Logic
        if (g_battle_state.current_turn != BATTLE_TURN_PLAYER && enemy.hp > 0) {
            static float enemy_timer = 0;
            enemy_timer += GetFrameTime();
            
            if (enemy_timer >= 0.1f) { 
                UI_AddLog("Enemy is thinking...");
                Battle_UpdateEnemyAI(&enemy, &player, game_map);
                
                // After AI action, update status effects for the enemy
                Battle_UpdateStatusEffects(&enemy.base);
                
                Battle_NextTurn();
                enemy_timer = 0;
            }
        }
                
                Battle_NextTurn();
                enemy_timer = 0;
            }
        }
 
        // --- Draw ---
        Core_Draw();
        BeginMode2D(g_game_camera.camera);
 
        // Render Map
        for (int32_t y = 0; y < game_map->height; y++) {
            for (int32_t x = 0; x < game_map->width; x++) {
                int32_t dx = x - player.base.x;
                int32_t dy = y - player.base.y;
                int32_t distSq = dx*dx + dy*dy;
                
                Color color;
                const char* symbol = " ";
                int wallFontSize = 12; // Bold and substantial
                int floorFontSize = 8;    // Subtle and clean
                int offset_x = (TILE_SIZE - 8) / 2; 
                int offset_y = (TILE_SIZE - 12) / 2;

                if (game_map->tiles[y * game_map->width + x] == TILE_WALL) {
                    bool top = (y > 0 && game_map->tiles[(y-1) * game_map->width + x] == TILE_WALL);
                    bool bot = (y < game_map->height-1 && game_map->tiles[(y+1) * game_map->width + x] == TILE_WALL);
                    bool lft = (x > 0 && game_map->tiles[y * game_map->width + (x-1)] == TILE_WALL);
                    bool rgt = (x < game_map->width-1 && game_map->tiles[y * game_map->width + (x+1)] == TILE_WALL);

                    if (top && bot && lft && rgt) symbol = "#";
                    else if (top && bot) symbol = "|";
                    else if (lft && rgt) symbol = "-";
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
                    else { symbol = "."; }
                    
                    int f_offset_x = (TILE_SIZE - 6) / 2;
                    int f_offset_y = (TILE_SIZE - 8) / 2;
                    DrawText(symbol, x * TILE_SIZE + f_offset_x, y * TILE_SIZE + f_offset_y, floorFontSize, color);
                }
            }
        }
 
        // Render Enemy
        if (enemy.hp > 0) {
            DrawText("E", enemy.base.x * TILE_SIZE + (TILE_SIZE-10)/2, enemy.base.y * TILE_SIZE + (TILE_SIZE-10)/2, TILE_SIZE - 6, RED);
        }
 
        // Render Player
        DrawText("@", player.base.x * TILE_SIZE + (TILE_SIZE-10)/2, player.base.y * TILE_SIZE + (TILE_SIZE-10)/2, TILE_SIZE - 6, BLUE);

 
        // Render UI (Screen-space) - Must be drawn last to be on top
        EndMode2D();
        UI_DrawLogPanel();
 
        Core_EndDraw();
    }
 
    // 5. Shutdown
    Map_Destroy(game_map);
    Core_Shutdown();
    printf("[Main] Game exited cleanly.\n");
 
    return 0;
}
