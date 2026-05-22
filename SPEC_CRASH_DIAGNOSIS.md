# SPEC_CRASH_DIAGNOSIS.md
Goal: Identify the exact line of the crash.
Tasks:
- Add printf logs with fflush(stdout) to main.c and init functions.
- Log points: InitWindow, InitFloorManager, InitParty, memset, loop entry.
- NO code fixes allowed, only diagnostics.

