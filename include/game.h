#ifndef GAME_H
#define GAME_H

#include "raylib.h"

// 화면 설정
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

// 타일 설정 (기본 20x20 픽셀)
#define TILE_SIZE 20

// 렌더링 헬퍼 함수
void DrawTile(int x, int y, char symbol);

#endif
