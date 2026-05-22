#ifndef UI_UI_H
#define UI_UI_H

#include <stdbool.h>
#include <stdint.h>
#include "core/core.h"

#define MAX_LOG_MESSAGES 10
#define LOG_PANEL_HEIGHT 250

typedef struct {
    char messages[MAX_LOG_MESSAGES][256];
    int32_t count;
    int32_t head;
} LogPanel;

extern LogPanel g_log_panel;

void UI_Init(void);
void UI_AddLog(const char* text);
void UI_DrawLogPanel(void);

#endif // UI_UI_H
