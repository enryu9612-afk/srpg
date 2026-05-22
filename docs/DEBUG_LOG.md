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

### [Phase 2] Emergency Fixes - Build/Rendering/Data Structure
- **Date**: 2026-05-23
- **Symptoms**:
    1. **BUG-01**: Build error due to potential missing files or incorrect include paths.
    2. **BUG-02**: UI rendering failure due to output order.
    3. **BUG-03**: Operator struct missing SP and isAlive fields.
    4. **BUG-04**: Non-standard naming conventions.
- **Action**: Implementing systematic fixes across core/entity/ui domains.
- **Status**: In Progress.


---
