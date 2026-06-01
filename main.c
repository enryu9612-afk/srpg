#include "core/core.h"
#include "ui/ui.h"
#include "map/map.h"
#include "entity/entity.h"
#include "battle/battle.h"
#include "growth/growth.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_ENEMIES 10
#define PLAYER_ATTACK_RANGE 1

int main(void) {
    DEBUG_PRINT("[Main] Starting Roguelike SRPG - Phase 5 Integration...\n");

    if (!Core_Init()) {
        fprintf(stderr, "[Main Error] Core initialization failed. Exiting.\n");
        return 1;
    }
    SetExitKey(KEY_NULL); 
    Core_InitCamera();
    UI_Init();

    Map* game_map = Map_Create(60, 30);
    if (!Map_Generate(game_map, (uint32_t)time(NULL))) {
        fprintf(stderr, "[Main Error] Map generation failed. Exiting.\n");
        return 1;
    }
    UI_AddLog("World generated successfully.");

    Operator player;
    Operator_Init(&player, 1, 0, 0);
    
    Enemy enemies[MAX_ENEMIES];
    bool enemy_active[MAX_ENEMIES] = {false};
    
    int32_t enemies_spawned = 0;
    int32_t spawn_attempts = 0;
    while (enemies_spawned < 5 && spawn_attempts < 100) {
        spawn_attempts++;
        int32_t ex, ey;
        if (Map_FindRoomSpawnPos(game_map, 0, 0, &ex, &ey)) {
            bool occupied = false;
            for (int i = 0; i < MAX_ENEMIES; i++) {
                if (enemy_active[i] && enemies[i].base.x == ex && enemies[i].base.y == ey) {
                    occupied = true;
                    break;
                }
            }
            if (!occupied) {
                int32_t idx = -1;
                for(int i=0; i<MAX_ENEMIES; i++) if(!enemy_active[i]) { idx = i; break; }
                if (idx != -1) {
                    Enemy_Init(&enemies[idx], 100 + idx, ex, ey, 1);
                    enemy_active[idx] = true;
                    enemies_spawned++;
                }
            }
        }
    }

    bool found_start = false;
    for (int32_t y = 0; y < game_map->height && !found_start; y++) {
        for (int32_t x = 0; x < game_map->width && !found_start; x++) {
            if (Map_IsWalkable(game_map, x, y)) {
                player.base.x = x;
                player.base.y = y;
                found_start = true;
            }
        }
    }
    UI_AddLog("Operator deployed to the sector.");
    UI_AddLog("Hostile enemies detected in the sector!");

    Core_UpdateCamera(player.base.x, player.base.y);

    Vector2 target_cursor = {0, 0};
    int32_t selected_enemy_index = -1;
    bool any_enemy_alive = false;

    while (g_game_state.is_running) {
        Core_Update();

        any_enemy_alive = false;
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemy_active[i] && enemies[i].hp > 0) {
                any_enemy_alive = true;
                break;
            }
        }

        if (g_battle_state.current_turn == BATTLE_TURN_PLAYER) {
            if (g_ui_context.state == UI_STATE_GAME) {
                int32_t dx = 0, dy = 0;
                if (IsKeyPressed(KEY_UP))    dy = -1;
                else if (IsKeyPressed(KEY_DOWN))  dy = 1;
                else if (IsKeyPressed(KEY_LEFT))  dx = -1;
                else if (IsKeyPressed(KEY_RIGHT)) dx = 1;

                if (dx != 0 || dy != 0) {
                    int32_t next_x = player.base.x + dx;
                    int32_t next_y = player.base.y + dy;

                    bool blocked = false;
                    if (!Map_IsWalkable(game_map, next_x, next_y)) {
                        blocked = true;
                        UI_AddLog("Blocked by a wall!");
                    } else {
                        for (int i = 0; i < MAX_ENEMIES; i++) {
                            if (enemy_active[i] && Entity_CheckCollisionAt(next_x, next_y, &enemies[i].base)) {
                                blocked = true;
                                UI_AddLog("Blocked by an enemy!");
                                break;
                            }
                        }
                    }

                    if (blocked) {
                        // Blocked
                    } else {
                        player.base.x = next_x;
                        player.base.y = next_y;
                        Core_UpdateCamera(player.base.x, player.base.y);
                        UI_AddLog("Moving...");
                        Battle_NextTurn(); 
                    }
                }

                if (IsKeyPressed(KEY_S)) {
                    g_ui_context.state = UI_STATE_TARGETING;
                    target_cursor.x = player.base.x;
                    target_cursor.y = player.base.y;
                    selected_enemy_index = -1;
                    UI_AddLog("Targeting Mode. Move cursor to select enemy.");
                }
                if (IsKeyPressed(KEY_I)) {
                    g_ui_context.state = UI_STATE_INVENTORY;
                    UI_AddLog("Inventory Opened.");
                }
            } else if (g_ui_context.state == UI_STATE_TARGETING) {
                int32_t dx = 0, dy = 0;
                if (IsKeyPressed(KEY_UP))    dy = -1;
                else if (IsKeyPressed(KEY_DOWN))  dy = 1;
                else if (IsKeyPressed(KEY_LEFT))  dx = -1;
                else if (IsKeyPressed(KEY_RIGHT)) dx = 1;

                if (dx != 0 || dy != 0) {
                    int32_t next_tx = (int32_t)target_cursor.x + dx;
                    int32_t next_ty = (int32_t)target_cursor.y + dy;
                    int32_t dist = (abs(next_tx - player.base.x) > abs(next_ty - player.base.y)) 
                                   ? abs(next_tx - player.base.x) : abs(next_ty - player.base.y);
                    
                    if (dist <= PLAYER_ATTACK_RANGE && Map_IsWalkable(game_map, next_tx, next_ty)) {
                        target_cursor.x = next_tx;
                        target_cursor.y = next_ty;
                    }
                }

                selected_enemy_index = -1;
                for (int i = 0; i < MAX_ENEMIES; i++) {
                    if (enemy_active[i] && enemies[i].base.x == (int32_t)target_cursor.x && enemies[i].base.y == (int32_t)target_cursor.y) {
                        selected_enemy_index = i;
                        break;
                    }
                }

                if (IsKeyPressed(KEY_ENTER)) {
                    if (selected_enemy_index != -1) {
                        int32_t result = Battle_ExecuteAttack(&player, (Entity*)&enemies[selected_enemy_index]);
                        if (result > 0) {
                            char buf[64];
                            sprintf(buf, "Attack Hit! Dealt %d damage. Enemy HP: %d", result, enemies[selected_enemy_index].hp);
                            UI_AddLog(buf);
                        } else if (result == 0) {
                            UI_AddLog("Attack Missed!");
                        }
                        if (enemies[selected_enemy_index].hp <= 0) {
                            enemy_active[selected_enemy_index] = false;
                            UI_AddLog("Enemy eliminated!");
                        }
                        Battle_NextTurn();
                        g_ui_context.state = UI_STATE_GAME;
                    } else {
                        UI_AddLog("No target selected!");
                    }
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    g_ui_context.state = UI_STATE_GAME;
                    UI_AddLog("Targeting cancelled.");
                }
            } else if (g_ui_context.state == UI_STATE_INVENTORY) {
                if (IsKeyPressed(KEY_LEFT)) UI_MoveInventorySlot(&g_inventory_selected_slot, 0);
                if (IsKeyPressed(KEY_RIGHT)) UI_MoveInventorySlot(&g_inventory_selected_slot, 1);
                if (IsKeyPressed(KEY_UP)) UI_MoveInventorySlot(&g_inventory_selected_slot, 2);
                if (IsKeyPressed(KEY_DOWN)) UI_MoveInventorySlot(&g_inventory_selected_slot, 3);
                if (IsKeyPressed(KEY_E)) {
                    Operator_EquipItem(&player, g_inventory_selected_slot);
                    UI_AddLog("Attempting to equip/use item...");
                }
                if (IsKeyPressed(KEY_ESCAPE)) {
                    g_ui_context.state = UI_STATE_GAME;
                    UI_AddLog("Inventory closed.");
                }
            }
        }

        if (g_battle_state.current_turn != BATTLE_TURN_PLAYER && any_enemy_alive) {
            static float enemy_timer = 0;
            enemy_timer += GetFrameTime();
            if (enemy_timer >= 0.1f) { 
                for (int i = 0; i < MAX_ENEMIES; i++) {
                    if (enemy_active[i]) {
                        Battle_UpdateEnemyAI(&enemies[i], &player, game_map);
                        Battle_UpdateStatusEffects(&enemies[i].base);
                        break; 
                    }
                }
                Battle_NextTurn();
                enemy_timer = 0;
            }
        }

        if (IsKeyPressed(KEY_C)) {
            g_ui_context.is_open = !g_ui_context.is_open;
            UI_AddLog(g_ui_context.is_open ? "Menu Opened" : "Menu Closed");
        }

        Core_Draw();
        BeginMode2D(g_game_camera.camera);
        if (g_ui_context.state == UI_STATE_TARGETING) {
            for (int32_t dy = -PLAYER_ATTACK_RANGE; dy <= PLAYER_ATTACK_RANGE; dy++) {
                for (int32_t dx = -PLAYER_ATTACK_RANGE; dx <= PLAYER_ATTACK_RANGE; dx++) {
                    if (dx == 0 && dy == 0) { continue; }
                    DrawRectangle((player.base.x + dx) * TILE_SIZE, (player.base.y + dy) * TILE_SIZE, TILE_SIZE, TILE_SIZE, Fade(ORANGE, 0.3f));
                }
            }
            DrawRectangleLinesEx((Rectangle){target_cursor.x * TILE_SIZE, target_cursor.y * TILE_SIZE, TILE_SIZE, TILE_SIZE}, 3, RED);
        }
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
        for (int i = 0; i < MAX_ENEMIES; i++) {
            if (enemy_active[i]) {
                Color eColor = (i == selected_enemy_index) ? YELLOW : RED;
                DrawText("E", enemies[i].base.x * TILE_SIZE + (TILE_SIZE-10)/2, enemies[i].base.y * TILE_SIZE + (TILE_SIZE-10)/2, TILE_SIZE - 6, eColor);
            }
        }
        if (player.base.x >= 0 && player.base.y >= 0) {
            DrawText("@", player.base.x * TILE_SIZE + (TILE_SIZE-10)/2, player.base.y * TILE_SIZE + (TILE_SIZE-10)/2, TILE_SIZE - 6, BLUE);
        }
        EndMode2D();
        UI_DrawLogPanel();
        if (g_ui_context.is_open) {
            UI_DrawCharacterMenu();
        }
        if (g_ui_context.state == UI_STATE_INVENTORY) {
            UI_DrawInventory(&player.inventory, &g_inventory_selected_slot);
        }
        Core_EndDraw();
    }

    Map_Destroy(game_map);
    Core_Shutdown();
    DEBUG_PRINT("[Main] Game exited cleanly.\n");
    return 0;
}
