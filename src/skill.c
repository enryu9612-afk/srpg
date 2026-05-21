#include "skill.h"
#include "combat.h"
#include "ui.h"
#include <stdio.h>
#include <string.h>

int UseSkill(Operator *op, Entity *target, int skill_idx) {
    if (skill_idx < 0 || skill_idx >= op->skill_count) {
        AddLog("No skill equipped at this slot!");
        return 0;
    }

    Skill *skill = &op->skills[skill_idx];
    
    // 1. SP 비용 계산 (혀 스탯 반영)
    int final_sp_cost = skill->sp_cost + GetTongueBonus(op->tongue);
    if (final_sp_cost < 0) final_sp_cost = 0;

    if (op->base.sp_cur < final_sp_cost) {
        char spMsg[128];
        sprintf(spMsg, "Not enough SP to use %s!", skill->name);
        AddLog(spMsg);
        return 0;
    }

    // 2. 스킬 효과 적용
    // Smite의 경우: 공격력 (100 + n*10)%로 공격
    float multiplier = skill->multiplier;
    
    // 실제 데미지 계산을 위해 ResolveCombat의 로직을 활용하거나, 
    // CalculateDamage를 호출하여 결과에 multiplier를 곱함
    CombatResult res = CalculateDamage(&op->base, target, NULL, ATTACK_PHYSICAL, op);
    
    if (res.is_hit) {
        int skill_damage = (int)(res.damage * multiplier);
        target->hp_cur -= skill_damage;
        if (target->hp_cur < 0) target->hp_cur = 0;
        if (target->hp_cur == 0) target->is_alive = 0;

        char logMsg[128];
        sprintf(logMsg, "[SKILL] %s! Dealt %d damage to Enemy.", skill->name, skill_damage);
        AddLog(logMsg);

        // 플로팅 텍스트 추가
        char dmgText[32];
        sprintf(dmgText, "%d", skill_damage);
        AddFloatingText(target->x, target->y, dmgText, YELLOW);
    } else {
        char missMsg[128];
        sprintf(missMsg, "Skill %s MISSED!", skill->name);
        AddLog(missMsg);
        AddFloatingText(target->x, target->y, "MISS", GRAY);
    }

    // 3. SP 소모
    op->base.sp_cur -= final_sp_cost;
    
    return 1;
}
