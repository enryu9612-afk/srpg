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
 
---
 
## 4. Debug/Testing API
- `Map_PrintToConsole(Map* map)`: Prints a text representation of the map to stdout.
- `Map_FindSafeSpawnPos(Map* map, int32_t px, int32_t py, int32_t* out_x, int32_t* out_y)`:
    - Searches 8 directions around (px, py).
    - Returns `true` and sets `out_x/out_y` if a walkable tile is found.
    - Used for spawning enemies safely without overlapping with walls or the player.
