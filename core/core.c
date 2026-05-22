#include "core.h"
#include <stdio.h>

GameState g_game_state = {0};

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
    
    // Update actual screen size after window creation
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
}

void Core_Draw(void) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    // The actual drawing of game objects will be called from main.c 
    // using DrawTile or other UI/Map functions.
}

// This function is intended to be called after main drawing logic to end the frame
void Core_EndDraw(void) {
    EndDrawing();
}
