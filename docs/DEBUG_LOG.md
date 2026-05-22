# Debug & Error Log

This document records the technical challenges, bugs, and solutions encountered during the development of the Roguelike SRPG.

## Log Entries

### [Phase 1] Header Inclusion Path Error
- **Date**: 2026-05-22
- **Symptom**: `fatal error: core/core.h: No such file or directory` during `ui/ui.c` compilation.
- **Cause**: The `build.bat` uses the `-Icore` flag, which adds the `core` directory to the include search path. Including `"core/core.h"` caused the compiler to look for `core/core/core.h`.
- **Resolution**: Changed `#include "core/core.h"` to `#include "core.h"`.
- **Lesson**: When using `-I<dir>` in GCC, the files inside `<dir>` should be included relative to that directory, not from the project root.

---

### [Phase 2] BSP Map Boundary Overflow
- **Date**: 2026-05-22
- **Symptom**: The outer walls of the generated map were occasionally breached, creating holes in the map boundaries.
- **Cause**: Lack of strict boundary checks in `carve_room` and `carve_corridor` functions, allowing floor tiles to be placed on the map edges.
- **Resolution**:
    1. Added explicit boundary checks (`x > 0 && x < map->width - 1`) before modifying any tile.
    2. Redefined the BSP root rectangle to start at `(1, 1)` with dimensions `(width-2, height-2)`, effectively creating a 1-tile thick "Safe Zone" of walls around the map.
- **Lesson**: In procedural generation, always implement a "Safe Zone" or strict boundary constraints to prevent array index overflow and ensure visual integrity of map borders.

---
