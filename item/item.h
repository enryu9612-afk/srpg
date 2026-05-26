#ifndef ITEM_ITEM_H
#define ITEM_ITEM_H

#include <stdint.h>
#include <stdbool.h>

typedef enum {
    ITEM_TYPE_CONSUMABLE,
    ITEM_TYPE_EQUIPMENT
} ItemType;

typedef enum {
    EQUIP_SLOT_NONE,
    EQUIP_SLOT_WEAPON,
    EQUIP_SLOT_ARMOR,
    EQUIP_SLOT_ACCESSORY
} EquipSlot;

typedef enum {
    ITEM_GRADE_COMMON,
    ITEM_GRADE_UNCOMMON,
    ITEM_GRADE_RARE,
    ITEM_GRADE_EPIC,
    ITEM_GRADE_LEGENDARY
} ItemGrade;

typedef struct {
    int32_t atk_bonus;
    int32_t def_bonus;
    int32_t mr_bonus;
    int32_t acc_bonus;
    int32_t eva_bonus;
} StatBonus;

typedef struct {
    uint32_t id;
    char name[64];
    ItemType type;
    ItemGrade grade;
    
    union {
        struct {
            int32_t heal_amount;
            int32_t sp_restore;
        } consumable;
        
        struct {
            EquipSlot slot;
            StatBonus bonus;
            int32_t weight;
        } equipment;
    } data;
} Item;

// 아이템 템플릿 (데이터베이스용 정적 데이터)
typedef struct {
    uint32_t id;
    char name[64];
    ItemType type;
    ItemGrade grade;
    union {
        struct {
            int32_t heal_amount;
            int32_t sp_restore;
        } consumable;
        struct {
            EquipSlot slot;
            StatBonus bonus;
            int32_t weight;
        } equipment;
    } data;
} ItemTemplate;

// 인벤토리 구조체
#define MAX_INVENTORY_SLOTS 24
typedef struct {
    Item* slots[MAX_INVENTORY_SLOTS];
    int32_t item_count;
} Inventory;

// 장비 장착 상태
typedef struct {
    Item* weapon;
    Item* armor;
    Item* accessory;
} EquipmentSet;

// DB API
Item* Item_CreateFromTemplate(uint32_t id);
extern const ItemTemplate g_item_database[];
extern const int32_t g_item_database_count;

// Inventory API
bool Inventory_Add(Inventory* inv, Item* item);
void Inventory_Remove(Inventory* inv, int32_t slot_index);

// Helper API (restored for backward compatibility in tests/build)
Item* Item_Create(uint32_t id, const char* name, ItemType type, ItemGrade grade);
void Item_SetEquipment(Item* item, EquipSlot slot, StatBonus bonus);

#endif // ITEM_ITEM_H
