#include "entity.h"
#include <stdio.h>
#include <math.h>

void Entity_Init(Entity* e, uint32_t id, int32_t x, int32_t y, EntityType type) {
    if (!e) return;
    e->id = id;
    e->x = x;
    e->y = y;
    e->type = type;
}

void Operator_Init(Operator* op, uint32_t id, int32_t x, int32_t y) {
    if (!op) return;

    // Initialize base entity
    Entity_Init(&op->base, id, x, y, ENTITY_TYPE_PLAYER);

    // Initial Stats (Level 1)
    op->level = 1;
    op->exp = 0;
    op->stat_points = 0; // Explicitly initialize stat points
    op->hp = 100;
    op->max_hp = 100;
    op->sp = 20;
    op->max_sp = 20;
    op->is_alive = true;
    op->attack = 10;
    op->defense = 5;
    op->accuracy = 80;
    op->evasion = 10;
    op->magic_res = 10;
    op->crit_chance = 5; // Initialize crit chance

    // Equipment & Inventory 초기화 (반드시 있어야 함 — 없으면 SEGFAULT 발생)
    for (int32_t i = 0; i < MAX_INVENTORY_SLOTS; i++) {
        op->inventory.slots[i] = NULL;
    }
    op->inventory.item_count = 0;
    op->equipment.weapon    = NULL;
    op->equipment.armor     = NULL;
    op->equipment.accessory = NULL;

    // Status Effects 초기화
    for (int32_t i = 0; i < 5; i++) {
        op->statuses[i].type     = STATUS_NONE;
        op->statuses[i].duration = 0;
    }
    
    // Special Stats (Initial Level I)
    op->special_stats.eye = 1;
    op->special_stats.ear = 1;
    op->special_stats.tongue = 1;
    op->special_stats.hand = 1;
    op->special_stats.heart = 1;

    printf("[Entity] Operator initialized: ID=%u, Pos=(%d, %d)\n", id, x, y);
}

void Operator_SetLevel(Operator* op, int32_t level) {
    if (!op) return;
 
    // Base stats increase by 1.2x per level
    float multiplier = powf(1.2f, (float)(level - 1));
 
    op->level = level;
    op->hp = (int32_t)(100 * multiplier);
    op->max_hp = op->hp;
    op->attack = (int32_t)(10 * multiplier);
    op->defense = (int32_t)(5 * multiplier);
    op->accuracy = (int32_t)(80 * multiplier);
    op->evasion = (int32_t)(10 * multiplier);
    op->magic_res = (int32_t)(10 * multiplier);
    
    printf("[Entity] Operator level updated to %d. Multiplier: %.2f\n", level, multiplier);
}

void Enemy_Init(Enemy* en, uint32_t id, int32_t x, int32_t y, int32_t level) {
    if (!en) return;
    Entity_Init(&en->base, id, x, y, ENTITY_TYPE_ENEMY);
    en->level = level;
    en->max_hp = 50 + (level * 10);
    en->hp = en->max_hp;
    en->attack = 5 + (level * 2);
    en->defense = 2 + level;
    en->accuracy = 70 + (level * 2);
    en->evasion = 5 + level;
    en->magic_res = 5 + level;
    en->exp_reward = level * 2;
    printf("[Entity] Enemy initialized: ID=%u, Level=%d, Pos=(%d, %d)\n", id, level, x, y);
}
