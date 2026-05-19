#include "entity.h"
#include <stdio.h>

void InitEntity(Entity *e, EntityType type, int x, int y, char symbol) {
    e->type = type;
    e->x = x;
    e->y = y;
    e->symbol = symbol;
    e->level = 1;
    e->hp_max = 100;
    e->hp_cur = 100;
    e->atk = 10;
    e->def = 5;
    e->matk = 5;
    e->mdef = 5;
    e->hit = 80;
    e->eva = 10;
    e->is_alive = 1;
}

const char* StatToRoman(int level) {
    switch (level) {
        case 1: return "I";
        case 2: return "II";
        case 3: return "III";
        case 4: return "IV";
        case 5: return "V";
        default: return "?";
    }
}
