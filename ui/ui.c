#include "ui.h"
#include <string.h>
#include <stdio.h>
#include "raylib.h"

LogPanel g_log_panel = {0};
UIContext g_ui_context = {UI_STATE_GAME, false, 0};
int32_t g_inventory_selected_slot = 0;

void UI_Init(void) {
    g_log_panel.count = 0;
    g_log_panel.head = 0;
    g_ui_context.state = UI_STATE_GAME;
    g_ui_context.is_open = false;
    printf("[UI] Log system initialized.\n");
}

void UI_AddLog(const char* text) {
    if (text == NULL) return;
    strncpy(g_log_panel.messages[g_log_panel.head], text, 255);
    g_log_panel.messages[g_log_panel.head][255] = '\0';
    g_log_panel.head = (g_log_panel.head + 1) % MAX_LOG_MESSAGES;
    if (g_log_panel.count < MAX_LOG_MESSAGES) {
        g_log_panel.count++;
    }
}

void UI_DrawLogPanel(void) {
    int32_t panel_x = GetScreenWidth() - UI_PANEL_WIDTH;
    DrawRectangle(panel_x, 0, UI_PANEL_WIDTH, GetScreenHeight(), Fade(BLACK, 0.8f));
    DrawRectangleLines(panel_x, 0, UI_PANEL_WIDTH, GetScreenHeight(), DARKGRAY);
    DrawText("SYSTEM LOG", panel_x + 10, 10, 20, GOLD);
    int32_t start_index = (g_log_panel.count < MAX_LOG_MESSAGES) ? 0 : g_log_panel.head;
    int32_t text_y_offset = 40;
    for (int32_t i = 0; i < g_log_panel.count; i++) {
        int32_t msg_index = (start_index + i) % MAX_LOG_MESSAGES;
        DrawText(g_log_panel.messages[msg_index], panel_x + 10, text_y_offset, 18, LIGHTGRAY);
        text_y_offset += 25;
    }
}

const char* UI_GetRomanNumeral(int32_t level) {
    if (level < 1) return "0";
    if (level == 1) return "I";
    if (level == 2) return "II";
    if (level == 3) return "III";
    if (level == 4) return "IV";
    if (level == 5) return "V";
    return "MAX";
}

void UI_DrawCyberWindow(Rectangle rect, Color color, const char* title) {
    // 1. Draw the main window body
    DrawRectangleRec(rect, Fade(color, 0.4f));
    DrawRectangleLinesEx(rect, 2, color);

    // 2. Draw the "Industrial Warning Stripe" at the top
    int32_t stripe_h = 8;
    for (int32_t x = (int32_t)rect.x; x < (int32_t)rect.x + (int32_t)rect.width; x += 16) {
        DrawRectangle(x, (int32_t)rect.y, 8, stripe_h, ORANGE);
        DrawRectangle(x + 8, (int32_t)rect.y, 8, stripe_h, color);
    }

    // 3. Draw Title
    DrawText(title, (int32_t)rect.x + 10, (int32_t)rect.y + 12, 18, WHITE);
    // Shadow for title
    DrawText(title, (int32_t)rect.x + 11, (int32_t)rect.y + 13, 18, BLACK);
}

void UI_DrawStatRow(Rectangle rect, const char* label, int32_t level, int32_t points, bool* clicked) {
    // Row Background
    DrawRectangleRec(rect, Fade(GRAY, 0.2f));
    
    // Label and Level
    DrawText(label, (int32_t)rect.x + 10, (int32_t)rect.y + 5, 20, LIGHTGRAY);
    
    const char* roman = UI_GetRomanNumeral(level);
    DrawText(roman, (int32_t)rect.x + 80, (int32_t)rect.y + 5, 20, WHITE);

    // Arrow Button [ + ]
    Rectangle btn = { rect.x + rect.width - 40, rect.y + 5, 30, 20 };
    Color btnColor = (points > 0) ? GOLD : DARKGRAY;
    DrawRectangleRec(btn, btnColor);
    DrawText("+", (int32_t)btn.x + 10, (int32_t)btn.y + 2, 18, BLACK);

    if (CheckCollisionPointRec(GetMousePosition(), btn) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        *clicked = true;
    } else {
        *clicked = false;
    }
}

void UI_DrawCharacterMenu(void) {
    int32_t screenW = GetScreenWidth();
    int32_t screenH = GetScreenHeight();
    
    // Background dim
    DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.95f));
    
    // 1. Left: Inventory / Quick Slots Area
    Rectangle invRect = { screenW * 0.1f, screenH * 0.2f, 300, screenH * 0.6f };
    UI_DrawCyberWindow(invRect, DARKBLUE, "INVENTORY / QUICK-SLOTS");
    
    // Quick Slots (5x1 Grid)
    for (int32_t i = 0; i < 5; i++) {
        Rectangle slot = { invRect.x + 10 + (i * 55), invRect.y + 50, 50, 50 };
        DrawRectangleRec(slot, Fade(LIGHTGRAY, 0.3f));
        DrawRectangleLinesEx(slot, 1, GRAY);
    }
    DrawText("CLICK TO EXPAND", invRect.x + 50, invRect.y + 110, 15, DARKGRAY);
    
    // 2. Right: Info Windows
    Rectangle redRect = { screenW * 0.45f, screenH * 0.2f, 400, 150 };
    UI_DrawCyberWindow(redRect, MAROON, "WEAPON & OFFENSE");
    DrawText("Current: [Iron Sword]", redRect.x + 20, redRect.y + 40, 20, WHITE);
    DrawText("ATK: 15 | Type: Physical", redRect.x + 20, redRect.y + 70, 18, LIGHTGRAY);
    
    Rectangle blueRect = { screenW * 0.45f, screenH * 0.37f, 400, 150 };
    UI_DrawCyberWindow(blueRect, DARKBLUE, "ARMOR & DEFENSE");
    DrawText("Current: [Cloth Tunic]", blueRect.x + 20, blueRect.y + 40, 20, WHITE);
    DrawText("DEF: 5 | MR: 10", blueRect.x + 20, blueRect.y + 70, 18, LIGHTGRAY);
    
    // 3. Bottom: Stat List (Growth)
    Rectangle statPanel = { screenW * 0.45f, screenH * 0.55f, 400, 220 };
    UI_DrawCyberWindow(statPanel, DARKGRAY, "ORGAN GROWTH");
    
    int32_t row_h = 35;
    int32_t start_y = (int32_t)statPanel.y + 40;
    
    const char* labels[] = { "Eye", "Ear", "Tongue", "Hand", "Heart" };
    int32_t levels[] = { 1, 2, 1, 3, 1 };
    int32_t points = 5;
    bool clicks[5] = {false};
    
    for (int32_t i = 0; i < 5; i++) {
        Rectangle row = { statPanel.x + 20, start_y + (i * row_h), statPanel.width - 40, 30 };
        UI_DrawStatRow(row, labels[i], levels[i], points, &clicks[i]);
    }
}

void UI_DrawSkillMenu(int32_t* selected_index) {
    int32_t screenW = GetScreenWidth();
    int32_t screenH = GetScreenHeight();
    Rectangle menuRect = { screenW * 0.35f, screenH * 0.3f, 300, 200 };
    UI_DrawCyberWindow(menuRect, DARKGRAY, "SKILL SELECTION");
    
    const char* skills[] = { "Basic Attack", "Slam (Strong)", "Analyze", "Heal" };
    int32_t skill_count = 4;
    int32_t row_h = 30;
    
    for (int32_t i = 0; i < skill_count; i++) {
        Rectangle row = { menuRect.x + 20, menuRect.y + 40 + (i * row_h), menuRect.width - 40, 25 };
        Color rowColor = (*selected_index == i) ? GOLD : Fade(GRAY, 0.3f);
        DrawRectangleRec(row, rowColor);
        DrawText(skills[i], row.x + 10, row.y + 5, 18, BLACK);
    }
}

void UI_DrawTargetingOverlay(Entity* target) {
    if (target == NULL) return;
    
    int32_t tx = target->x * TILE_SIZE;
    int32_t ty = target->y * TILE_SIZE;
    
    DrawRectangleLinesEx((Rectangle){tx, ty, TILE_SIZE, TILE_SIZE}, 3, RED);
    DrawText("TARGET LOCKED", tx, ty - 20, 15, RED);
}

