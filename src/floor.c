#include "floor.h"
#include "map.h"
#include "entity.h"
#include "player.h"
#include <stdlib.h>
#include <stdio.h>

void InitFloorManager(FloorManager *fm) {
    fm->current_floor_idx = 0;
    fm->current_sector = 0;
    fm->current_type = FLOOR_REGULAR;
    GenerateFloor(fm);
}

void GenerateFloor(FloorManager *fm) {
    // GDD 명세: 일반 4개 -> 보스 1개 -> 휴식 1개 반복 (총 4구역)
    int floor_in_sector = fm->current_floor_idx % 6;

    if (floor_in_sector < 4) {
        fm->current_type = FLOOR_REGULAR;
        CreateRegularMap(&fm->current_map);
    } else if (floor_in_sector == 4) {
        fm->current_type = FLOOR_BOSS;
        CreateBossMap(&fm->current_map);
    } else {
        fm->current_type = FLOOR_REST;
        CreateRestMap(&fm->current_map);
    }
    
    fm->current_sector = fm->current_floor_idx / 6;
}

void TransitionToNextFloor(FloorManager *fm, Party *party) {
    fm->current_floor_idx++;
    GenerateFloor(fm);
    
    // 플레이어 위치를 새 층의 시작점으로 재배치 (예: 1,1)
    for(int i = 0; i < party->count; i++) {
        party->members[i].base.x = 1;
        party->members[i].base.y = 1;
    }
    printf("Moving to Floor %d...\n", fm->current_floor_idx + 1);
}

void CreateRegularMap(Map *map) {
    // 초기화: 전체 벽으로 채우기
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            map->tiles[y][x] = TILE_WALL;
        }
    }

    // 5명의 워커가 서로 다른 지점에서 시작하여 맵을 뚫음
    int walkerCount = 5;
    int tiles_per_walker = 600; 

    for (int w = 0; w < walkerCount; w++) {
        int curX = 1 + (rand() % (MAP_WIDTH - 2));
        int curY = 1 + (rand() % (MAP_HEIGHT - 2));
        int dug = 0;

        while(dug < tiles_per_walker) {
            map->tiles[curY][curX] = TILE_FLOOR;
            dug++;

            int dir = rand() % 4;
            if(dir == 0 && curY > 1) curY--;
            else if(dir == 1 && curY < MAP_HEIGHT - 2) curY++;
            else if(dir == 2 && curX > 1) curX--;
            else if(dir == 3 && curX < MAP_WIDTH - 2) curX++;
        }
    }
    
    // 플레이어 시작점 및 주변 확보
    for(int y = 0; y < 3; y++) {
        for(int x = 0; x < 3; x++) {
            map->tiles[1+y][1+x] = TILE_FLOOR;
        }
    }
}

void CreateBossMap(Map *map) {
    // 보스층은 좀 더 넓은 빈 공간을 가짐 (GDD: 5*5 사이즈 빈 공간 등)
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            map->tiles[y][x] = TILE_WALL;
        }
    }

    // 중앙에 큰 보스 챔버 생성
    int centerX = MAP_WIDTH / 2;
    int centerY = MAP_HEIGHT / 2;
    for(int y = centerY - 2; y <= centerY + 2; y++) {
        for(int x = centerX - 2; x <= centerX + 2; x++) {
            map->tiles[y][x] = TILE_FLOOR;
        }
    }
    
    // 시작점에서 보스 챔버까지 연결하는 단순한 복도
    for(int x = 1; x <= centerX; x++) map->tiles[1][x] = TILE_FLOOR;
    for(int y = 1; y <= centerY; y++) map->tiles[y][centerX] = TILE_FLOOR;
    
    map->tiles[1][1] = TILE_FLOOR;
}

void CreateRestMap(Map *map) {
    // 휴식층은 사실상 텍스트 UI로 처리되지만, 
    // 렌더링 오류 방지를 위해 최소한의 맵 구조를 유지
    for(int y = 0; y < MAP_HEIGHT; y++) {
        for(int x = 0; x < MAP_WIDTH; x++) {
            map->tiles[y][x] = TILE_WALL;
        }
    }
    map->tiles[1][1] = TILE_FLOOR;
}
