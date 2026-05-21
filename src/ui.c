#include "ui.h"
#include "raylib.h"
#include "game.h"
#include "map.h"
#include <stdio.h>
#include <stdarg.h>

GameLog global_log = {0};
FloatingText floating_texts[MAX_FLOATING_TEXTS] = {0};

void AddLog(const char* format, ...) {
    va_list args;
    va_start(args, format);
    char buffer[MAX_LOG_LENGTH];
    vsnprintf(buffer, MAX_LOG_LENGTH, format, args);
    va_end(args);

    // 오래된 메시지를 앞으로 당기고 (0번이 가장 오래된 메시지)
    for (int i = 0; i < MAX_LOG_MESSAGES - 1; i++) {
        strncpy(global_log.messages[i], global_log.messages[i + 1], MAX_LOG_LENGTH);
        global_log.messages[i][MAX_LOG_LENGTH - 1] = '\0';
    }
    // 가장 마지막 칸에 새 메시지를 삽입 (가장 최신 메시지)
    strncpy(global_log.messages[MAX_LOG_MESSAGES - 1], buffer, MAX_LOG_LENGTH);
    global_log.messages[MAX_LOG_MESSAGES - 1][MAX_LOG_LENGTH - 1] = '\0';
    global_log.count++;
}

void AddFloatingText(int x, int y, const char* text, Color color) {
    for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
        if (!floating_texts[i].active) {
            floating_texts[i].active = 1;
            floating_texts[i].x = (float)x;
            floating_texts[i].y = (float)y;
            floating_texts[i].text[0] = '\0';
            sprintf(floating_texts[i].text, "%s", text);
            floating_texts[i].color = color;
            floating_texts[i].life = 1.5f; // 1.5초 동안 표시
            return;
        }
    }
}

void UpdateFloatingTexts(float delta) {
    for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
        if (floating_texts[i].active) {
            floating_texts[i].y -= 30.0f * delta; // 서서히 위로 상승
            floating_texts[i].life -= delta;
            if (floating_texts[i].life <= 0) {
                floating_texts[i].active = 0;
            }
        }
    }
}

void DrawFloatingTexts(GameCamera cam) {
    for (int i = 0; i < MAX_FLOATING_TEXTS; i++) {
        if (floating_texts[i].active) {
            // 월드 좌표를 화면 좌표로 변환 (카메라 오프셋 적용)
            // 타일 크기 고려하여 텍스트 배치
            float screenX = (floating_texts[i].x - (float)cam.x) * TILE_SIZE;
            float screenY = (floating_texts[i].y - (float)cam.y) * TILE_SIZE;
            
            DrawText(floating_texts[i].text, (int)screenX, (int)screenY, 20, floating_texts[i].color);
        }
    }
}

void DrawUI(PlayerUnit *active, GameLog *log, const char* state_str) {
    DrawRectangle(0, GAME_HEIGHT, SCREEN_WIDTH, UI_HEIGHT, DARKGRAY);
    DrawRectangleLines(0, GAME_HEIGHT, SCREEN_WIDTH, UI_HEIGHT, WHITE);

    DrawRectangle(10, GAME_HEIGHT + 10, 300, UI_HEIGHT - 20, Fade(GRAY, 0.5f));
    char infoText[128];
    sprintf(infoText, "Unit: %d | HP: %d/%d | State: %s", 
            active->base.id, active->base.hp_cur, active->base.hp_max, state_str);
    DrawText(infoText, 20, GAME_HEIGHT + 20, 20, RAYWHITE);

    char guideText[128];
    sprintf(guideText, "Move: [Arrows] | Attack: [Z] | EndTurn: [Enter]");
    DrawText(guideText, 320, GAME_HEIGHT + 20, 18, LIGHTGRAY);

    int logX = SCREEN_WIDTH - 400;
    int logY = GAME_HEIGHT + 10;
    DrawRectangle(logX, logY, 390, UI_HEIGHT - 20, Fade(BLACK, 0.3f));
    
    for (int i = 0; i < MAX_LOG_MESSAGES; i++) {
        int y_pos = logY + 25 + (i * 22);
        if (global_log.messages[i][0] != '\0') {
            DrawText(global_log.messages[i], logX + 10, y_pos, 16, RAYWHITE);
        }
    }
}
