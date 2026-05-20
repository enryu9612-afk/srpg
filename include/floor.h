#ifndef FLOOR_H
#define FLOOR_H

#include "map.h"
#include "entity.h"
#include "player.h"

typedef enum {
    FLOOR_REGULAR,
    FLOOR_BOSS,
    FLOOR_REST
} FloorType;

typedef struct {
    int current_floor_idx; // 0부터 시작
    int current_sector;    // 0~3 (총 4구역)
    FloorType current_type;
    Map current_map;
} FloorManager;

// 층 관리자 초기화 및 첫 층 생성
void InitFloorManager(FloorManager *fm);

// 현재 층의 타입 결정 및 맵 생성
void GenerateFloor(FloorManager *fm);

// 다음 층으로 이동
void TransitionToNextFloor(FloorManager *fm, Party *party);

// 특정 타입의 맵 생성 함수들
void CreateRegularMap(Map *map);
void CreateBossMap(Map *map);
void CreateRestMap(Map *map); // 휴식층은 사실상 텍스트 UI지만 맵 구조는 유지

#endif
