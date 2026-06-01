# Roguelike SRPG

## 🎮 조작법 (Controls)

### 🏃 기본 이동 및 시스템
- **방향키 ($\uparrow \downarrow \leftarrow \rightarrow$)**: 캐릭터 이동
- **`C`**: 캐릭터 정보 및 시스템 메뉴 토글
- **`F11`**: 전체 화면 모드 전환

### 🎯 전투 및 타겟팅
- **`S`**: 타겟팅 모드 진입 (Targeting Mode)
- **타겟팅 중 방향키**: 공격 대상 커서 이동
- **`ENTER`**: 선택한 대상에게 공격 실행
- **`ESC`**: 타겟팅 취소 및 기본 모드 복귀

### 🎒 인벤토리 및 장비
- **`I`**: 인벤토리 창 열기/닫기
- **`ESC`**: 인벤토리 닫기
- **`E`**: 선택한 아이템 장착 또는 사용

## 🛠 빌드 및 실행 (For Developers)

### 윈도우 빌드 (Standalone)
`release.bat`를 실행하면 `raylib`가 설치되지 않은 환경에서도 실행 가능한 `srpg_release.exe`가 생성됩니다.

```cmd
release.bat
```

### 프로젝트 구조
- `core/`: 게임 엔진 및 윈도우 관리 - 김서진
- `map/`: BSP 알고리즘 기반 맵 생성 및 관리 - 김정근
- `entity/`: 캐릭터, 적, 장비 시스템 - 서동현
- `battle/`: 전투 공식 및 턴 관리 - 김서진
- `ui/`: 사이버네틱 UI 렌더링 및 로직 - 문홍준
- `growth/`: 성장 시스템 및 스탯 계산 - 김서진
- `item/`: 아이템 DB 및 인벤토리 관리 - 이동혁
- `main.c`:  main함수 - 김서진

## 📜 라이선스
MIT License


조작법이랑 설치법 ai한테 써달라했습니다.
