#ifndef ENTITY_ENTITY_H
#define ENTITY_ENTITY_H

#include <stdint.h>
#include <stdbool.h>
#include "item/item.h"

typedef enum {
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_ENEMY,
    ENTITY_TYPE_NPC
} EntityType;

typedef enum {
    STATUS_NONE = 0,
    STATUS_BURN,
    STATUS_RAGE,
    STATUS_SICKLY,
    STATUS_HUNGER
} StatusType;

typedef struct {
    StatusType type;
    int32_t duration; // Turns remaining
} StatusEffect;

typedef struct {
    uint32_t id;
    int32_t x;
    int32_t y;
    EntityType type;
} Entity;

typedef struct {
    // 5 Great Organs (Stats I to V)
    int32_t eye;     // Accuracy
    int32_t ear;     // Evasion
    int32_t tongue;  // SP Cost Reduction
    int32_t hand;    // Crit & Equip effect
    int32_t heart;   // Attack & HP
} OperatorStats;

typedef struct {
    Entity base;
    
    // Common Stats
    int32_t level;
    int32_t exp;
    int32_t stat_points; // Points available for spending on organs
    int32_t hp;
    int32_t max_hp;
    int32_t sp;
    int32_t max_sp;
    bool is_alive;
    int32_t attack;
    int32_t defense;
    int32_t accuracy;
    int32_t evasion;
    int32_t magic_res;
    int32_t crit_chance; // Added for Hand stat
    
    OperatorStats special_stats;
    StatusEffect statuses[5]; // Max 5 active effects

    // Equipment & Inventory
    EquipmentSet equipment;
    Inventory inventory;
} Operator;

typedef struct {
    Entity base;
    
    // Enemy Stats
    int32_t level;
    int32_t hp;
    int32_t max_hp;
    int32_t attack;
    int32_t defense;
    int32_t accuracy;
    int32_t evasion;
    int32_t magic_res;
    int32_t exp_reward;
    StatusEffect statuses[5]; // Max 5 active effects
} Enemy;

// Entity API
void Entity_Init(Entity* e, uint32_t id, int32_t x, int32_t y, EntityType type);

// Operator API
void Operator_Init(Operator* op, uint32_t id, int32_t x, int32_t y);
void Operator_SetLevel(Operator* op, int32_t level);
void Operator_UpdateFinalStats(Operator* op);
void Operator_EquipItem(Operator* op, int32_t inventory_slot);

// Enemy API
void Enemy_Init(Enemy* en, uint32_t id, int32_t x, int32_t y, int32_t level);

#endif // ENTITY_ENTITY_H
