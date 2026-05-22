#include "ui.h"
#include <string.h>
#include <stdio.h>
#include "raylib.h"

LogPanel g_log_panel = {0};

void UI_Init(void) {
    g_log_panel.count = 0;
    g_log_panel.head = 0;
    printf("[UI] Log system initialized.\n");
}

void UI_AddLog(const char* text) {
    if (text == NULL) return;

    // Copy text to circular buffer
    strncpy(g_log_panel.messages[g_log_panel.head], text, 255);
    g_log_panel.messages[g_log_panel.head][255] = '\0'; // Ensure null termination

    // Advance head and update count
    g_log_panel.head = (g_log_panel.head + 1) % MAX_LOG_MESSAGES;
    if (g_log_panel.count < MAX_LOG_MESSAGES) {
        g_log_panel.count++;
    }
}

void UI_DrawLogPanel(void) {
    int32_t panel_y = g_game_state.screen_height - LOG_PANEL_HEIGHT;
    
    // Draw background
    DrawRectangle(0, panel_y, g_game_state.screen_width, LOG_PANEL_HEIGHT, DARKGRAY);
    DrawRectangleLines(0, panel_y, g_game_state.screen_width, LOG_PANEL_HEIGHT, WHITE);

    // Draw messages
    int32_t start_index = (g_log_panel.count < MAX_LOG_MESSAGES) ? 0 : g_log_panel.head;
    int32_t text_y_offset = 10;

    for (int32_t i = 0; i < g_log_panel.count; i++) {
        int32_t msg_index = (start_index + i) % MAX_LOG_MESSAGES;
        DrawText(g_log_panel.messages[msg_index], 20, panel_y + text_y_offset, 20, LIGHTGRAY);
        text_y_offset += 25;
    }
}
