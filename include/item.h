#ifndef ITEM_H
#define ITEM_H

#include <stdbool.h>

typedef enum {
    ITEM_EQUIPMENT,
    ITEM_RELIC,
    ITEM_CONSUMABLE,
    ITEM_TICKET
} ItemCategory;

typedef enum {
    RARITY_CRUDE,    // 조잡
    RARITY_COMMON,   // 평범
    RARITY_ADVANCED, // 고급
    RARITY_RARE,     // 희귀
    RARITY_UNIQUE    // 유일
} ItemRarity;

typedef enum {
    WEAPON_SWORD,
    WEAPON_SPEAR,
    WEAPON_CROSSBOW,
    WEAPON_STAFF,
    WEAPON_MACE
} WeaponType;

typedef enum {
    ARMOR_LIGHT,
    ARMOR_HEAVY,
    ARMOR_MAGIC
} ArmorType;

typedef struct {
    int id;
    char name[64];
    ItemCategory category;
    ItemRarity rarity;
    
    // 장비일 경우의 속성
    struct {
        WeaponType w_type;
        ArmorType a_type;
        float atk_bonus;
        float def_bonus;
        float mres_bonus;
        int range_bonus;
        bool is_weapon;
        bool is_armor;
        bool is_accessory;
    } equip_stats;
} Item;

// 아이템 생성 헬퍼
Item CreateWeapon(WeaponType type, ItemRarity rarity, const char* name);
Item CreateArmor(ArmorType type, ItemRarity rarity, const char* name);
Item CreateAccessory(ItemRarity rarity, const char* name);

#endif
