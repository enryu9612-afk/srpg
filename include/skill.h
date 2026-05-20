#ifndef SKILL_H
#define SKILL_H

#include "entity.h"
#include "player.h"

typedef enum {
    SKILL_BASIC_ATTACK,
    SKILL_SMITE
} SkillType;

// 스킬 실행 함수
int UseSkill(Operator *op, Entity *target, int skill_idx);

#endif
