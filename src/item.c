#include "item.h"
#include <string.h>

// 등급별 공격력/방어력 수치 (GDD 참조: 10/20/30/40/55)
// 조잡: 10, 평범: 20, 고급: 30, 희귀: 40, 유일: 55
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
    
    // rarity 0(조잡)부터 4(유일)까지 단계별 보너스 적용
    float modifier = (rarity + 1) * 10.0f; 
    
    if (type == ARMOR_HEAVY) item.equip_stats.def_bonus = 20.0f + modifier;
    else if (type == ARMOR_LIGHT) item.equip_stats.def_bonus = 10.0f + modifier;
    else if (type == ARMOR_MAGIC) item.equip_stats.mres_bonus = 15.0f + modifier;
    
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
