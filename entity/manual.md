# Entity Domain Manual

## 1. Domain Responsibility
The `entity` domain defines the "who" of the game. It handles the pure data structures and initialization of all living beings in the world.

### Included
- Base `Entity` structure (id, position, type).
- `Operator` structure (player-specific stats, growth, classes).
- `Party` structure (management of multiple operators).
- Pure stat bonus functions (e.g., `GetEyeBonus`).

### Excluded
- Combat logic and damage calculation (handled by `battle`).
- Status effect processing (handled by `battle`).
- AI behavior (handled by `battle`).
- Rendering of entities (handled by `core` and `ui`).

---

## 2. Coding Conventions

### Naming
- **Public Functions**: Use `Entity_` or `Operator_` prefixes.
  - Example: `Entity_Init()`, `Operator_CalculateStat()`.
- **Structures**: Use PascalCase for structure names.
  - Example: `Entity`, `Operator`, `Party`.

### Design Philosophy
- **Data-Oriented**: Structures should be primarily Plain Old Data (POD). Avoid putting complex behavior/logic inside these structures.
- **Unique Identification**: Every entity must have a unique `uint32_t` ID for tracking and targeting.

---

## 3. Interface Principles
- **Stat Calculation**: This domain provides the "base" stats. Final combat values (after equipment/buffs) are calculated in the `battle` domain.
- **Dependency Flow**: `main.c` $\rightarrow$ `entity` $\rightarrow$ `core`.
