#ifndef UI_UI_H
#define UI_UI_H

#include <stdbool.h>
#include <stdint.h>
#include "entity/entity.h"
#include "core/core.h"
#include "raylib.h"

#define MAX_LOG_MESSAGES 10
#define LOG_PANEL_HEIGHT 250
#define UI_PANEL_WIDTH 300

typedef struct {
    char messages[MAX_LOG_MESSAGES][256];
    int32_t count;
    int32_t head;
} LogPanel;

typedef enum {
    UI_STATE_GAME,
    UI_STATE_CHARACTER_MENU,
    UI_STATE_SKILL_SELECT,
    UI_STATE_TARGETING
} UIState;

typedef struct {
    UIState state;
    bool is_open;
} UIContext;

extern LogPanel g_log_panel;
extern UIContext g_ui_context;

void UI_Init(void);
void UI_AddLog(const char* text);
void UI_DrawLogPanel(void);

// Cybernetic UI Elements
void UI_DrawCyberWindow(Rectangle rect, Color color, const char* title);
void UI_DrawStatRow(Rectangle rect, const char* label, int32_t level, int32_t points, bool* clicked);
const char* UI_GetRomanNumeral(int32_t level);

void UI_DrawCharacterMenu(void);
void UI_DrawSkillMenu(int32_t* selected_index);
void UI_DrawTargetingOverlay(Entity* target);

#endif // UI_UI_H
