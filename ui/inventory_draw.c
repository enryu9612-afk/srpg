#include <stdint.h>
#include <stddef.h>
#include "raylib.h"
#include "ui/ui.h"
#include "item/item.h"

void UI_DrawInventory(Inventory* inv, int32_t* selected_slot) {
    int32_t screenW = GetScreenWidth();
    int32_t screenH = GetScreenHeight();
    
    // 배경 딤 처리 (이미 CharacterMenu와 비슷하게 처리됨)
    DrawRectangle(0, 0, screenW, screenH, Fade(BLACK, 0.9f));
    
    Rectangle invRect = { screenW * 0.2f, screenH * 0.2f, 600, 400 };
    UI_DrawCyberWindow(invRect, DARKGRAY, "S-OPERATOR INVENTORY");
    
    int32_t slot_w = 60;
    int32_t slot_h = 60;
    int32_t padding = 15;
    int32_t cols = 6;
    int32_t rows = 4;
    
    for (int32_t i = 0; i < MAX_INVENTORY_SLOTS; i++) {
        int32_t r = i / cols;
        int32_t c = i % cols;
        
        Rectangle slotRect = { 
            invRect.x + padding + (c * (slot_w + padding)), 
            invRect.y + 40 + (r * (slot_h + padding)), 
            (float)slot_w, (float)slot_h 
        };
        
        // 선택된 슬롯 강조
        Color slotColor = (*selected_slot == i) ? GOLD : Fade(GRAY, 0.3f);
        DrawRectangleRec(slotRect, slotColor);
        DrawRectangleLinesEx(slotRect, 1, GRAY);
        
        // 아이템 렌더링
        if (inv->slots[i] != NULL) {
            Item* item = inv->slots[i];
            
            // 등급별 색상 지정
            Color itemColor = WHITE;
            switch(item->grade) {
                case ITEM_GRADE_COMMON: itemColor = LIGHTGRAY; break;
                case ITEM_GRADE_UNCOMMON: itemColor = LIME; break;
                case ITEM_GRADE_RARE: itemColor = BLUE; break;
                case ITEM_GRADE_EPIC: itemColor = PURPLE; break;
                case ITEM_GRADE_LEGENDARY: itemColor = GOLD; break;
            }
            
            // 아이템 이름 요약 표시 (중앙 정렬)
            int fontSize = 12;
            int textW = MeasureText(item->name, fontSize);
            DrawText(item->name, slotRect.x + (slot_w - textW)/2, slotRect.y + (slot_h - fontSize)/2, fontSize, itemColor);
        }
    }
    
    // 하단 안내 메시지
    DrawText("Use ARROWS to navigate | E to Equip/Use | ESC to Close", 
             invRect.x + 20, invRect.y + invRect.height - 30, 15, LIGHTGRAY);
}
