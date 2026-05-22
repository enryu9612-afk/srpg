#include "core/core.h"
#include "ui/ui.h"
#include "map/map.h"
#include "entity/entity.h"
#include <stdio.h>

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

    // Set player starting position to the first available floor tile
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

    // Initial camera update
    Core_UpdateCamera(player.base.x, player.base.y);

    // 4. Main Game Loop
    while (g_game_state.is_running) {
        // --- Update ---
        Core_Update();

        // Player Input & Movement
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
            } else {
                UI_AddLog("Blocked by a wall!");
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

        // Render Player
        DrawRectangle(player.base.x * TILE_SIZE, player.base.y * TILE_SIZE, TILE_SIZE - 1, TILE_SIZE - 1, BLUE);
        DrawText("@", player.base.x * TILE_SIZE + 8, player.base.y * TILE_SIZE + 4, 20, WHITE);

        Core_EndDraw();

        // Render UI (Screen-space)
        UI_DrawLogPanel();
    }

    // 5. Shutdown
    Map_Destroy(game_map);
    Core_Shutdown();
    printf("[Main] Game exited cleanly.\n");

    return 0;
}
