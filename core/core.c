#include "core.h"
#include <stdio.h>

GameState g_game_state = {0};
GameCamera g_game_camera = {0};

bool Core_Init(void) {
    g_game_state.window_width = 1280;
    g_game_state.window_height = 720;
    g_game_state.is_running = true;
    g_game_state.current_floor = 1;

    InitWindow(g_game_state.window_width, g_game_state.window_height, "Roguelike SRPG - Foundation");
    
    if (!IsWindowReady()) {
        fprintf(stderr, "[Core Error] Failed to initialize raylib window.\n");
        return false;
    }

    SetTargetFPS(60);
    
    g_game_state.screen_width = GetScreenWidth();
    g_game_state.screen_height = GetScreenHeight();

    printf("[Core] Window initialized successfully: %dx%d\n", g_game_state.screen_width, g_game_state.screen_height);
    return true;
}

void Core_Shutdown(void) {
    printf("[Core] Shutting down...\n");
    CloseWindow();
}

void Core_Update(void) {
    if (WindowShouldClose()) {
        g_game_state.is_running = false;
    }
    
    // Toggle Fullscreen (F11)
    if (IsKeyPressed(KEY_F11)) {
        if (IsWindowFullscreen()) {
            ToggleFullscreen();
        } else {
            ToggleFullscreen();
        }
    }
}

void Core_Draw(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    // Start 2D mode for world rendering
    BeginMode2D(g_game_camera.camera);
}

void Core_EndDraw(void) {
    EndMode2D();
    EndDrawing();
}

void Core_InitCamera(void) {
    g_game_camera.camera.offset = (Vector2){ g_game_state.screen_width / 2.0f, g_game_state.screen_height / 2.0f };
    g_game_camera.camera.target = (Vector2){ 0, 0 };
    g_game_camera.camera.rotation = 0.0f;
    g_game_camera.camera.zoom = 1.0f;
}

void Core_UpdateCamera(int32_t target_x, int32_t target_y) {
    g_game_camera.camera.target = (Vector2){ 
        target_x * TILE_SIZE + TILE_SIZE / 2.0f, 
        target_y * TILE_SIZE + TILE_SIZE / 2.0f 
    };
}
