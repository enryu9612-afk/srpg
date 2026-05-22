#ifndef ENTITY_ENTITY_H
#define ENTITY_ENTITY_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ENTITY_TYPE_PLAYER,
    ENTITY_TYPE_ENEMY,
    ENTITY_TYPE_NPC
} EntityType;

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
    int32_t hp;
    int32_t max_hp;
    int32_t attack;
    int32_t defense;
    int32_t accuracy;
    int32_t evasion;
    int32_t magic_res;
    
    OperatorStats special_stats;
} Operator;

// Entity API
void Entity_Init(Entity* e, uint32_t id, int32_t x, int32_t y, EntityType type);

// Operator API
void Operator_Init(Operator* op, uint32_t id, int32_t x, int32_t y);
void Operator_SetLevel(Operator* op, int32_t level);

#endif // ENTITY_ENTITY_H
