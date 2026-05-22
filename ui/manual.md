# UI Domain Manual

## 1. Domain Responsibility
The `ui` domain is responsible for the visual representation of game information. It translates game state into text-based visual elements.

### Included
- Text-based log panels (Combat logs, system messages).
- Floating text (Damage numbers, status change notifications).
- Stat panels and character information displays.
- Font management and color palettes.

### Excluded
- Window and screen management (handled by `core`).
- Game logic and state transitions (handled by `battle`, `map`, etc.).
- World object rendering (handled by `core/DrawTile` or `map`).

---

## 2. Coding Conventions

### Naming
- **Public Functions**: All public API functions must start with the `UI_` prefix.
  - Example: `UI_Init()`, `UI_AddLog()`, `UI_DrawLogPanel()`.
- **Structures**: Use PascalCase for structure names.
  - Example: `LogPanel`, `FloatingText`.
- **Variables/Internal Functions**: Use snake_case.

### Rendering Principles
- **Coordinate System**: UI elements use screen-space coordinates (absolute) unless they are specifically anchored to world objects.
- **Consistency**: All UI text should use a consistent font and color scheme defined within this domain.

---

## 3. Interface Principles
- **Read-Only State**: The `ui` domain should primarily be a consumer of game state. It should not modify `GameState` or other domain data.
- **Log Management**: The log system must implement a fixed-size buffer (circular buffer or queue) to prevent memory overflow and maintain only the most recent messages.
- **Dependency Flow**: `main.c` $\rightarrow$ `ui` $\rightarrow$ `core`.
