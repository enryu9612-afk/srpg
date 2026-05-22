#ifndef CORE_CORE_H
#define CORE_CORE_H

#include <stdbool.h>
#include <stdint.h>
#include "raylib.h"

typedef struct {
    int32_t window_width;
    int32_t window_height;
    int32_t screen_width;
    int32_t screen_height;
    bool is_running;
    int32_t current_floor;
} GameState;

extern GameState g_game_state;

bool Core_Init(void);
void Core_Shutdown(void);
void Core_Update(void);
void Core_Draw(void);
void Core_EndDraw(void);

#endif // CORE_CORE_H
