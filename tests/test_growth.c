#include <stdio.h>
#include <assert.h>
#include <stdbool.h>
#include "entity/entity.h"
#include "growth/growth.h"

void test_level_up() {
    printf("Testing Level Up...\n");
    Operator op;
    Operator_Init(&op, 1, 0, 0);
    
    // Lv 1 -> 2: Req 10 EXP
    bool leveled = Growth_AddExp(&op, 10);
    assert(leveled == true);
    assert(op.level == 2);
    assert(op.stat_points == 1);
    printf("  - Single level up passed.\n");

    // Lv 2 -> 3: Req 20 EXP
    leveled = Growth_AddExp(&op, 20);
    assert(leveled == true);
    assert(op.level == 3);
    assert(op.stat_points == 2);
    printf("  - Sequential level up passed.\n");
}

void test_multi_level_up() {
    printf("Testing Multi Level Up...\n");
    Operator op;
    Operator_Init(&op, 1, 0, 0);
    
    // Lv 1(10) + Lv 2(20) + Lv 3(30) = 60 EXP total to reach Lv 4
    Growth_AddExp(&op, 60);
    assert(op.level == 4);
    assert(op.stat_points == 3);
    printf("  - Multi level up passed.\n");
}

void test_stat_investment() {
    printf("Testing Stat Investment...\n");
    Operator op;
    Operator_Init(&op, 1, 0, 0);
    op.stat_points = 10;

    // Invest in Eye: Lv 1 -> 2 (Cost 2)
    bool success = Growth_InvestStat(&op, STAT_EYE, 1);
    assert(success == true);
    assert(op.special_stats.eye == 2);
    assert(op.stat_points == 8); // 10 - 2
    printf("  - Valid investment passed.\n");

    // Invest in Eye: Lv 2 -> 3 (Cost 3)
    success = Growth_InvestStat(&op, STAT_EYE, 1);
    assert(success == true);
    assert(op.special_stats.eye == 3);
    assert(op.stat_points == 5); // 8 - 3
    printf("  - Sequential investment passed.\n");

    // Invest in Eye: Lv 3 -> 4 (Cost 4)
    success = Growth_InvestStat(&op, STAT_EYE, 1);
    assert(success == true);
    assert(op.special_stats.eye == 4);
    assert(op.stat_points == 1); // 5 - 4
    printf("  - Investment near zero points passed.\n");

    // Invest in Eye: Lv 4 -> 5 (Cost 5) - Should fail (points=1)
    success = Growth_InvestStat(&op, STAT_EYE, 1);
    assert(success == false);
    assert(op.special_stats.eye == 4);
    printf("  - Insufficient points investment failed as expected.\n");
}

void test_stat_bonuses() {
    printf("Testing Stat Bonuses...\n");
    Operator op;
    Operator_Init(&op, 1, 0, 0);
    
    int32_t initial_atk = op.attack;
    
    // Level up to 2
    Growth_AddExp(&op, 10); 
    int32_t lv2_atk = op.attack;
    assert(lv2_atk > initial_atk);
    printf("  - Level based attack increase passed.\n");

    // Invest in Heart (Lv 1 -> 2: Cost 2)
    op.stat_points = 10;
    Growth_InvestStat(&op, STAT_HEART, 1);
    int32_t heart_lv2_atk = op.attack;
    assert(heart_lv2_atk > lv2_atk);
    printf("  - Heart investment attack increase passed.\n");
}

int main() {
    printf("=== Growth System Headless Tests ===\n");
    test_level_up();
    test_multi_level_up();
    test_stat_investment();
    test_stat_bonuses();
    printf("=== All Tests Passed Successfully ===\n");
    return 0;
}
