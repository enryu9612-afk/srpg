#include "entity.h"
#include "ui.h"
#include <stdio.h>
#include <math.h>
#include <string.h>

void InitEntity(Entity *e, int id, EntityType type, int x, int y, char symbol, int level) {
    e->id = id;
    e->type = type;
    e->x = x;
    e->y = y;
    e->symbol = symbol;
    e->level = level;
    
    e->atk = 10.0f * powf(1.2f, level - 1);
    e->hp_max = 100 * (int)powf(1.2f, level - 1);
    e->hp_cur = e->hp_max;
    e->sp_max = 50;
    e->sp_cur = e->sp_max;
    e->def = 5.0f;
    e->magic_res = 5.0f;
    e->acc = 80.0f;
    e->eva = 10.0f;
    e->is_alive = 1;

    for(int i = 0; i < MAX_STATUS_EFFECTS; i++) {
        e->statuses[i].type = STATUS_NONE;
        e->statuses[i].duration = 0;
    }
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

void AddStatusEffect(Entity *e, StatusType type, int duration) {
    for(int i = 0; i < MAX_STATUS_EFFECTS; i++) {
        if(e->statuses[i].type == type) {
            e->statuses[i].duration = duration;
            return;
        }
    }
    for(int i = 0; i < MAX_STATUS_EFFECTS; i++) {
        if(e->statuses[i].type == STATUS_NONE) {
            e->statuses[i].type = type;
            e->statuses[i].duration = duration;
            return;
        }
    }
}

void UpdateStatusEffects(Entity *e) {
    for(int i = 0; i < MAX_STATUS_EFFECTS; i++) {
        if(e->statuses[i].type == STATUS_NONE) continue;

        if(e->statuses[i].type == STATUS_BURN) {
            int damage = (int)(e->hp_max * 0.05f);
            if(damage < 1) damage = 1;
            e->hp_cur -= damage;
            
            char logMsg[64];
            sprintf(logMsg, "Unit %d took %d Burn damage!", e->id, damage);
            AddLog(logMsg);
        }

        e->statuses[i].duration--;
        if(e->statuses[i].duration <= 0) {
            e->statuses[i].type = STATUS_NONE;
        }
    }
    if(e->hp_cur <= 0) {
        e->hp_cur = 0;
        e->is_alive = 0;
    }
}

const char* GetStatusName(StatusType type) {
    switch (type) {
        case STATUS_RAGE: return "Rage";
        case STATUS_HUNGER: return "Hunger";
        case STATUS_SICKLY: return "Sickly";
        case STATUS_BURN: return "Burn";
        default: return "None";
    }
}

float GetEyeBonus(int level) {
    float bonuses[] = {0.0f, 0.10f, 0.25f, 0.45f, 0.70f, 1.00f};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0.0f;
}

float GetEarBonus(int level) {
    float bonuses[] = {0.0f, 0.10f, 0.25f, 0.45f, 0.70f, 1.00f};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0.0f;
}

float GetHandBonus(int level) {
    float bonuses[] = {0.0f, 0.05f, 0.10f, 0.20f, 0.35f, 0.50f};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0.0f;
}

float GetHeartBonus(int level) {
    float bonuses[] = {0.0f, 0.05f, 0.10f, 0.20f, 0.35f, 0.50f};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0.0f;
}

int GetTongueBonus(int level) {
    int bonuses[] = {0, -1, -2, -3, -4, -5};
    return (level >= 0 && level <= 5) ? bonuses[level] : 0;
}
