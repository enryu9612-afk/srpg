#ifndef ENTITY_H
#define ENTITY_H

#include "map.h"

typedef enum {
    ENTITY_PLAYER,
    ENTITY_MONSTER
} EntityType;

typedef struct {
    EntityType type;
    int x, y;
    char symbol;
    
    // 스탯 (내부적으로 1~5 단계로 관리)
    int level;
    int hp_max;
    int hp_cur;
    int atk;      // 물리 공격력
    int def;      // 물리 방어력
    int matk;     // 마법 공격력
    int mdef;     // 마법 저항력
    int hit;      // 명중률
    int eva;      // 회피율
    
    int is_alive;
} Entity;

// 기초 함수
void InitEntity(Entity *e, EntityType type, int x, int y, char symbol);
const char* StatToRoman(int level); // 1~5 -> I~V 변환

#endif
