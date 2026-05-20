#ifndef ENTITY_H
#define ENTITY_H

#include "map.h"

typedef enum {
    ENTITY_PLAYER,
    ENTITY_MONSTER
} EntityType;

// 모든 엔티티가 공통으로 가지는 스탯
typedef struct {
    int id;
    EntityType type;
    int x, y;
    char symbol;

    int level;
    float atk;
    float eva;
    float acc;
    int hp_max;
    int hp_cur;
    float def;
    float magic_res;

    int is_alive;
} Entity;

// 오퍼레이터(플레이어 유닛) 전용 추가 스탯
typedef struct {
    Entity base;
    int eye;    // 눈: 명중률
    int ear;    // 귀: 회피율
    int tongue; // 혀: SP 소모 감소
    int hand;   // 손: 크리티컬, 장비 효과
    int heart;  // 심장: 공격력, 체력
} Operator;

void InitEntity(Entity *e, int id, EntityType type, int x, int y, char symbol, int level);
const char* StatToRoman(int level);

// GDD 기반 스탯 보너스 헬퍼 함수
float GetEyeBonus(int level);    // 명중률 보너스
float GetEarBonus(int level);    // 회피율 보너스
float GetHandBonus(int level);   // 크리/장비 보너스
float GetHeartBonus(int level);  // 공격/체력 보너스
int GetTongueBonus(int level);   // SP 소모 감소

#endif
