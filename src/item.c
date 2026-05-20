#include "item.h"
#include <string.h>

// 등급에 따른 기본 공격력/방어력 수치 (GDD 참조)
// 무기: 10/20/30/40/55
static float GetBaseAtk(ItemRarity rarity) {
    float atk[] = {10.0f, 20.0f, 30.0f, 40.0f, 55.0f};
    return atk[rarity];
}

Item CreateWeapon(WeaponType type, ItemRarity rarity, const char* name) {
    Item item;
    memset(&item, 0, sizeof(Item));
    item.category = ITEM_EQUIPMENT;
    item.rarity = rarity;
    strncpy(item.name, name, 63);
    
    item.equip_stats.is_weapon = true;
    item.equip_stats.w_type = type;
    item.equip_stats.atk_bonus = GetBaseAtk(rarity);
    
    // 무기별 사거리 설정
    if (type == WEAPON_SPEAR) item.equip_stats.range_bonus = 1;
    else if (type == WEAPON_CROSSBOW) item.equip_stats.range_bonus = 4;
    
    return item;
}

Item CreateArmor(ArmorType type, ItemRarity rarity, const char* name) {
    Item item;
    memset(&item, 0, sizeof(Item));
    item.category = ITEM_EQUIPMENT;
    item.rarity = rarity;
    strncpy(item.name, name, 63);
    
    item.equip_stats.is_armor = true;
    item.equip_stats.a_type = type;
    
    // GDD 명세 적용 (중갑은 공격력 감소 등)
    if (type == ARMOR_HEAVY) item.equip_stats.def_bonus = 20.0f * (rarity + 1);
    else if (type == ARMOR_LIGHT) item.equip_stats.def_bonus = 10.0f * (rarity + 1);
    else if (type == ARMOR_MAGIC) item.equip_stats.mres_bonus = 15.0f * (rarity + 1);
    
    return item;
}

Item CreateAccessory(ItemRarity rarity, const char* name) {
    Item item;
    memset(&item, 0, sizeof(Item));
    item.category = ITEM_EQUIPMENT;
    item.rarity = rarity;
    strncpy(item.name, name, 63);
    item.equip_stats.is_accessory = true;
    return item;
}
