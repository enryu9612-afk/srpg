#include "entity.h"
#include <stdio.h>
#include <math.h>

void InitEntity(Entity *e, int id, EntityType type, int x, int y, char symbol, int level) {
    e->id = id;
    e->type = type;
    e->x = x;
    e->y = y;
    e->symbol = symbol;
    e->level = level;
    
    // 기초값 설정 (GDD 기반으로 추후 레벨별로 세분화 예정)
    e->atk = 10.0f * powf(1.2f, level - 1);
    e->hp_max = 100 * (int)powf(1.2f, level - 1);
    e->hp_cur = e->hp_max;
    e->def = 5.0f;
    e->magic_res = 5.0f;
    e->acc = 80.0f;
    e->eva = 10.0f;
    e->is_alive = 1;
}

const char* StatToRoman(int level) {
    switch (level) {
        case 1: return "I";
        case 2: return "II";
        case 3: return "III";
        case 4: return "IV";
        case 5: return "V";
        default: return "0";
    }
}

// GDD 명세: 눈/귀 (10%, 25%, 45%, 70%, 100%)
float GetEyeBonus(int level) {
    float bonuses[] = {0.0f, 0.10f, 0.25f, 0.45f, 0.70f, 1.00f};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0.0f;
}

float GetEarBonus(int level) {
    float bonuses[] = {0.0f, 0.10f, 0.25f, 0.45f, 0.70f, 1.00f};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0.0f;
}

// GDD 명세: 손/심장 (5%, 10%, 20%, 35%, 50%)
float GetHandBonus(int level) {
    float bonuses[] = {0.0f, 0.05f, 0.10f, 0.20f, 0.35f, 0.50f};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0.0f;
}

float GetHeartBonus(int level) {
    float bonuses[] = {0.0f, 0.05f, 0.10f, 0.20f, 0.35f, 0.50f};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0.0f;
}

// GDD 명세: 혀 (소모 SP -1, -2, -3, -4, -5)
int GetTongueBonus(int level) {
    int bonuses[] = {0, -1, -2, -3, -4, -5};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0;
}
