#ifndef ENTITY_H
#define ENTITY_H

#include "map.h"
#include "item.h" // Item 타입 사용을 위해 추가

#define MAX_STATUS_EFFECTS 5
#define MAX_SKILLS 3

typedef enum {
    ENTITY_PLAYER,
    ENTITY_MONSTER
} EntityType;

typedef enum {
    STATUS_NONE,
    STATUS_RAGE,
    STATUS_HUNGER,
    STATUS_SICKLY,
    STATUS_BURN
} StatusType;

typedef struct {
    StatusType type;
    int duration;
} StatusEffect;

typedef struct {
    char name[32];
    int sp_cost;
    float multiplier;
    int level;
} Skill;

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
    int sp_max;
    int sp_cur;
    float def;
    float magic_res;

    StatusEffect statuses[MAX_STATUS_EFFECTS];
    int is_alive;
} Entity;

// 오퍼레이터(플레이어 유닛) 전용 추가 스탯 및 장비
typedef struct {
    Entity base;
    int eye;
    int ear;
    int tongue;
    int hand;
    int heart;
    Skill skills[MAX_SKILLS];
    int skill_count;

    // 장비 슬롯 추가
    Item *weapon;
    Item *armor;
    Item *accessory;
} Operator;

void InitEntity(Entity *e, int id, EntityType type, int x, int y, char symbol, int level);
const char* StatToRoman(int level);

void AddStatusEffect(Entity *e, StatusType type, int duration);
void UpdateStatusEffects(Entity *e);
const char* GetStatusName(StatusType type);

float GetEyeBonus(int level);
float GetEarBonus(int level);
float GetHandBonus(int level);
float GetHeartBonus(int level);
int GetTongueBonus(int level);

#endif
