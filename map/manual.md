# Map Domain Manual

## 1. Domain Responsibility
The `map` domain handles the structure and generation of the game world. It defines what the world is made of and how it is created.

### Included
- Map tile definitions (`TileType`).
- Random map generation using the BSP (Binary Space Partitioning) algorithm.
- Wall collision detection (`Map_IsWalkable`).
- Floor management and transition logic.

### Excluded
- Entity placement and spawning (handled by `entity` or a separate spawn system).
- Rendering of the map (handled by `core` and `ui`).
- Combat logic within the map (handled by `battle`).

---

## 2. Coding Conventions

### Naming
- **Public Functions**: All public API functions must start with the `Map_` prefix.
  - Example: `Map_Generate()`, `Map_IsWalkable()`.
- **Structures**: Use PascalCase for structure names.
  - Example: `Map`, `Rect`.

### Data Structures
- Use `uint8_t` for tile arrays to minimize memory footprint.
- Map data must be decoupled from rendering logic to allow for headless console testing.

---

## 3. Interface Principles
- **Pure Logic**: Map generation functions should take parameters (width, height, seed) and return a map structure without calling any GUI functions.
- **Collision API**: Provide a simple boolean function `Map_IsWalkable(x, y)` that other domains can use for movement validation.
- **Dependency Flow**: `main.c` $\rightarrow$ `map` $\rightarrow$ `core`.
