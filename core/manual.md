# Core Domain Manual
 
## 1. Domain Responsibility
The `core` domain is the foundation of the game engine. It handles the infrastructure required to run the game but remains agnostic of the actual game rules.
 
### Included
- Window initialization and management (raylib).
- The main game loop (Update/Draw cycle).
- `GameCamera` management (scaling, offsetting).
- Basic primitive rendering (e.g., `DrawTile`).
- Global game state management (`GameState`).
 
### Excluded
- Map generation algorithms (handled by `map`).
- Combat calculations and AI (handled by `battle`).
- Entity data and stats (handled by `entity`).
- High-level UI panels (handled by `ui`).
 
---
 
## 2. Coding Conventions
 
### Naming
- **Public Functions**: All public API functions must start with the `Core_` prefix.
  - Example: `Core_InitWindow()`, `Core_GetGameState()`.
- **Structures**: Use PascalCase for structure names.
  - Example: `GameState`, `GameCamera`.
- **Variables/Internal Functions**: Use snake_case.
 
### Types
- Use `stdbool.h` for boolean values.
- Use `stdint.h` (e.g., `int32_t`, `uint32_t`) for fixed-width integers to ensure cross-platform consistency.
 
### Error Handling
- Initialization and critical functions must return a `bool` (true for success, false for failure).
- Errors should be logged via a consistent logging mechanism before returning false.
 
---
 
## 3. Interface Principles
- **Dependency Flow**: `main.c` $\rightarrow$ `core` $\rightarrow$ (Other Domains).
- `core` should not depend on the internal details of other domains. It provides the "stage" where other domains' logic is executed and rendered.
- All resource allocation in `core` must have a corresponding deallocation function (e.g., `Core_Init` $\rightarrow$ `Core_Shutdown`).
 
---
 
## 4. Main Loop & Turn Structure
- **Loop Cycle**: The main loop follows a strict `Core_Update()` $\rightarrow$ `Logic Processing` $\rightarrow$ `Core_Draw()` $\rightarrow$ `UI Rendering` $\rightarrow$ `Core_EndDraw()` sequence.
- **Turn Management**: The game operates on a turn-based system. Input is only processed when `g_battle_state.current_turn == BATTLE_TURN_PLAYER`.
- **Turn Transition**: A turn ends immediately after a successful action (Move, Attack, etc.), triggering `Battle_NextTurn()`.
