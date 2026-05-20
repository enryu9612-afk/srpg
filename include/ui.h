#ifndef UI_H
#define UI_H

#include "player.h"
#include "entity.h"
#include "raylib.h"

#define MAX_LOG_MESSAGES 5
#define MAX_LOG_LENGTH 128

// 플로팅 텍스트 구조체
typedef struct {
    float x, y;
    char text[32];
    Color color;
    float life; // 남은 수명 (초 단위)
    int active;
} FloatingText;

#define MAX_FLOATING_TEXTS 20

typedef struct {
    char messages[MAX_LOG_MESSAGES][MAX_LOG_LENGTH];
    int count;
} GameLog;

extern GameLog global_log;
extern FloatingText floating_texts[MAX_FLOATING_TEXTS];

void AddLog(const char* format, ...);
void DrawUI(PlayerUnit *active, GameLog *log, const char* state_str);

// 플로팅 텍스트 관련 함수
void AddFloatingText(int x, int y, const char* text, Color color);
void UpdateFloatingTexts(float delta);
void DrawFloatingTexts(GameCamera cam);

#endif
