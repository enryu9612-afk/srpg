#include "core/core.h"
#include "ui/ui.h"
#include <stdio.h>

int main(void) {
    printf("[Main] Starting Roguelike SRPG...\n");

    // 1. Initialization
    if (!Core_Init()) {
        fprintf(stderr, "[Main Error] Core initialization failed. Exiting.\n");
        return 1;
    }

    UI_Init();

    // Initial welcome logs
    UI_AddLog("Welcome to the Roguelike SRPG!");
    UI_AddLog("Foundation Phase: Core and UI systems are online.");
    UI_AddLog("Current Floor: 1 - Famine Layer");

    // 2. Main Game Loop
    while (g_game_state.is_running) {
        // Update
        Core_Update();

        // Draw
        Core_Draw();
        
        // UI rendering (on top of everything)
        UI_DrawLogPanel();
        
        Core_EndDraw();
    }

    // 3. Shutdown
    Core_Shutdown();
    printf("[Main] Game exited cleanly.\n");

    return 0;
}
