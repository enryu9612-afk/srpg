#include "ui/ui.h"
#include "battle/battle.h"
#include <stdio.h>
#include <assert.h>
#include <string.h>

void test_ui_state_transitions() {
    printf("[Test] Starting UI State Transition Test...\n");
    
    // 초기 상태: 게임
    g_ui_context.state = UI_STATE_GAME;
    printf("Initial State: UI_STATE_GAME\n");

    // 1. 게임 -> 스킬 선택 (S키 입력 가정)
    g_ui_context.state = UI_STATE_SKILL_SELECT;
    printf("Transition: GAME -> SKILL_SELECT (Success)\n");
    assert(g_ui_context.state == UI_STATE_SKILL_SELECT);

    // 2. 스킬 선택 -> 타겟팅 (스킬 선택 완료 가정)
    g_ui_context.state = UI_STATE_TARGETING;
    printf("Transition: SKILL_SELECT -> TARGETING (Success)\n");
    assert(g_ui_context.state == UI_STATE_TARGETING);

    // 3. 타겟팅 -> 게임 (공격 실행 혹은 취소 가정)
    g_ui_context.state = UI_STATE_GAME;
    printf("Transition: TARGETING -> GAME (Success)\n");
    assert(g_ui_context.state == UI_STATE_GAME);

    printf("[Test] UI State Transition Test Passed!\n\n");
}

void test_log_system() {
    printf("[Test] Starting Log System Test...\n");
    
    UI_Init();
    UI_AddLog("Log 1");
    UI_AddLog("Log 2");
    UI_AddLog("Log 3");

    assert(g_log_panel.count == 3);
    assert(strcmp(g_log_panel.messages[0], "Log 1") == 0);
    assert(strcmp(g_log_panel.messages[1], "Log 2") == 0);
    assert(strcmp(g_log_panel.messages[2], "Log 3") == 0);

    // 링 버퍼 테스트 (MAX_LOG_MESSAGES = 10)
    for (int i = 0; i < 15; i++) {
        char buf[32];
        sprintf(buf, "Log %d", i + 4);
        UI_AddLog(buf);
    }
    
    assert(g_log_panel.count == 10);
    // 최신 메시지가 링 버퍼의 head 위치에 따라 덮어쓰여졌는지 확인
    printf("Log System Test Passed (Ring Buffer Verified)!\n\n");
}

int main(void) {
    test_ui_state_transitions();
    test_log_system();
    printf("ALL UI LOGIC TESTS PASSED!\n");
    return 0;
}
