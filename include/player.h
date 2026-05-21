#ifndef PLAYER_H
#define PLAYER_H

#include "entity.h"
#include "item.h"

#define MAX_INVENTORY 10

// GDD의 '오퍼레이터' 정의: 기본 엔티티 스탯 + 5대 기관 스탯 + 스킬
typedef Operator PlayerUnit; 

void InitPlayer(PlayerUnit *player);
void MovePlayer(PlayerUnit *player, int dx, int dy, const Map *map);
bool EquipItem(PlayerUnit *player, Item *item);

#endif
