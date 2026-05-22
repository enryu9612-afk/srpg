@echo off
echo [Building SRPG - Phase 2 Updated...]

set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -I.
set LDFLAGS=-L"C:/raylib/lib" -lraylib -lgdi32 -lwinmm

echo Compiling battle...
%CC% %CFLAGS% -c battle/battle.c -o battle/battle.o
echo Compiling core...
%CC% %CFLAGS% -c core/core.c -o core/core.o
echo Compiling ui...
%CC% %CFLAGS% -c ui/ui.c    -o ui/ui.o
echo Compiling map...
%CC% %CFLAGS% -c map/map.c    -o map/map.o
echo Compiling entity...
%CC% %CFLAGS% -c entity/entity.c -o entity/entity.o
echo Compiling main...
%CC% %CFLAGS% -c main.c     -o main.o

echo Linking...
%CC% battle/battle.o core/core.o ui/ui.o map/map.o entity/entity.o main.o -o srpg.exe %LDFLAGS%

if %errorlevel% equ 0 (
    echo [Build Successful!]
    srpg.exe
) else (
    echo [Build Failed!]
    pause
)
