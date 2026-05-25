#ifndef CORE_CORE_H
#define CORE_CORE_H

#include <stdbool.h>
#include <stdint.h>
#include "raylib.h"

#define TILE_SIZE 32

typedef struct {

    int32_t window_width;
    int32_t window_height;
    int32_t screen_width;
    int32_t screen_height;
    bool is_running;
    int32_t current_floor;
} GameState;

typedef struct {
    Camera2D camera;
} GameCamera;

extern GameState g_game_state;
extern GameCamera g_game_camera;

bool Core_Init(void);
void Core_Shutdown(void);
void Core_Update(void);
void Core_Draw(void);
void Core_EndDraw(void);

// Camera API
void Core_InitCamera(void);
void Core_UpdateCamera(int32_t target_x, int32_t target_y);

#endif // CORE_CORE_H
