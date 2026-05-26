#include "growth.h"
#include <stdio.h>
#include <math.h>
#include <stdbool.h>

// Base stats for Level 1 Operator
static const int32_t BASE_HP = 100;
static const int32_t BASE_ATTACK = 10;
static const int32_t BASE_DEFENSE = 5;
static const int32_t BASE_ACCURACY = 80;
static const int32_t BASE_EVASION = 10;
static const int32_t BASE_MAGIC_RES = 10;
static const int32_t BASE_CRIT_CHANCE = 5;

// Organ bonus tables (Level I to V)
static const float ORGAN_BONUS_PERCENT[] = {
    0.00f, // Index 0 not used
    0.10f, // Level I
    0.25f, // Level II
    0.45f, // Level III
    0.70f, // Level IV
    1.00f  // Level V
};

static const int32_t TONGUE_SP_REDUCTION[] = {
    0, // Index 0
    -1, // Level I
    -2, // Level II
    -3, // Level III
    -4, // Level IV
    -5  // Level V
};

bool Growth_AddExp(Operator* op, int32_t amount) {
    if (!op) return false;

    op->exp += amount;
    bool leveled_up = false;

    // Level up check: Next Level EXP = Current Level * 10
    while (op->level < 25 && op->exp >= (op->level * 10)) {
        op->exp -= (op->level * 10);
        op->level++;
        op->stat_points++;
        leveled_up = true;
        printf("[Growth] Level Up! Current Level: %d, Stat Points: %d\n", op->level, op->stat_points);
    }

    if (leveled_up) {
        Growth_ApplyStatBonuses(op);
    }

    return leveled_up;
}

bool Growth_InvestStat(Operator* op, StatType type, int32_t points) {
    if (!op) return false;

    // Current organ level
    int32_t current_level = 0;
    switch (type) {
        case STAT_EYE:    current_level = op->special_stats.eye; break;
        case STAT_EAR:    current_level = op->special_stats.ear; break;
        case STAT_TONGUE: current_level = op->special_stats.tongue; break;
        case STAT_HAND:   current_level = op->special_stats.hand; break;
        case STAT_HEART:  current_level = op->special_stats.heart; break;
        default: return false;
    }

    int32_t next_level = current_level + 1;
    if (next_level > 5) return false; // Max level is V

    // Investment cost: To upgrade to level n, costs n points
    int32_t cost = next_level;
    if (op->stat_points < cost) return false;

    op->stat_points -= cost;
    
    // Update organ level
    switch (type) {
        case STAT_EYE:    op->special_stats.eye = next_level; break;
        case STAT_EAR:    op->special_stats.ear = next_level; break;
        case STAT_TONGUE: op->special_stats.tongue = next_level; break;
        case STAT_HAND:   op->special_stats.hand = next_level; break;
        case STAT_HEART:  op->special_stats.heart = next_level; break;
        default: return false;
    }

    printf("[Growth] Stat invested in %d. New Level: %d, Remaining Points: %d\n", type, next_level, op->stat_points);
    Growth_ApplyStatBonuses(op);
    return true;
}

void Growth_ApplyStatBonuses(Operator* op) {
    if (!op) return;

    // 1. Base stat growth (1.2x per level)
    double multiplier = pow(1.2, (double)(op->level - 1));
    
    double current_hp = BASE_HP * multiplier;
    double current_atk = BASE_ATTACK * multiplier;
    double current_def = BASE_DEFENSE * multiplier;
    double current_acc = BASE_ACCURACY * multiplier;
    double current_eva = BASE_EVASION * multiplier;
    double current_mres = BASE_MAGIC_RES * multiplier;
    double current_crit = BASE_CRIT_CHANCE * multiplier;

    // 2. Organ Bonuses
    // Heart: Attack & HP (+5% to +100%)
    float heart_bonus = ORGAN_BONUS_PERCENT[op->special_stats.heart];
    current_hp *= (1.0f + heart_bonus);
    current_atk *= (1.0f + heart_bonus);

    // Eye: Accuracy (+5% to +100%)
    float eye_bonus = ORGAN_BONUS_PERCENT[op->special_stats.eye];
    current_acc *= (1.0f + eye_bonus);

    // Ear: Evasion (+5% to +100%)
    float ear_bonus = ORGAN_BONUS_PERCENT[op->special_stats.ear];
    current_eva *= (1.0f + ear_bonus);

    // Hand: Crit & Equip Effect (+5% to +100%)
    float hand_bonus = ORGAN_BONUS_PERCENT[op->special_stats.hand];
    current_crit *= (1.0f + hand_bonus);

    // Final assignment (Cast to int32_t)
    op->max_hp = (int32_t)current_hp;
    op->hp = op->max_hp; // For simplicity, refill on update
    op->attack = (int32_t)current_atk;
    op->defense = (int32_t)current_def;
    op->accuracy = (int32_t)current_acc;
    op->evasion = (int32_t)current_eva;
    op->magic_res = (int32_t)current_mres;
    op->crit_chance = (int32_t)current_crit;
}
