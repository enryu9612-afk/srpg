#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "battle/battle.h"
#include "entity/entity.h"

void test_physical_damage() {
    printf("Testing Physical Damage... ");
    assert(Battle_CalculatePhysicalDamage(20, 10) == 10);
    assert(Battle_CalculatePhysicalDamage(10, 20) == 0);
    printf("PASS\n");
}

void test_magic_damage() {
    printf("Testing Magic Damage... ");
    // 20 * (1 - 20/100) = 16
    assert(Battle_CalculateMagicDamage(20, 20) == 16);
    // 20 * (1 - 100/100) = 0
    assert(Battle_CalculateMagicDamage(20, 100) == 0);
    printf("PASS\n");
}

void test_hit_probability() {
    printf("Testing Hit Probability... ");
    // 100% hit
    assert(Battle_CheckHit(100, 0) == true);
    // 0% hit
    assert(Battle_CheckHit(0, 100) == false);
    printf("PASS\n");
}

void test_execute_attack() {
    printf("Testing Execute Attack... ");
    Operator op;
    Operator_Init(&op, 1, 0, 0);
    op.attack = 20;
    op.accuracy = 100; // Always hit

    Enemy en;
    Enemy_Init(&en, 101, 1, 0, 1); // Adjacent (range 1)
    en.defense = 10;
    en.evasion = 0;
    en.hp = 100;

    int32_t damage = Battle_ExecuteAttack(&op, (Entity*)&en);
    assert(damage == 10);
    assert(en.hp == 90);
    printf("PASS\n");
}

int main() {
    printf("--- Starting Battle Logic Headless Tests ---\n");
    test_physical_damage();
    test_magic_damage();
    test_hit_probability();
    test_execute_attack();
    printf("--- All Tests Passed Successfully! ---\n");
    return 0;
}
