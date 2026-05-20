@echo off
echo [Building SRPG for Windows...]

set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -Iinclude -I"C:/raylib/include"
set LDFLAGS=-L"C:/raylib/lib" -lraylib -lgdi32 -lwinmm

:: Create objects
%CC% %CFLAGS% -c src/main.c -o src/main.o
%CC% %CFLAGS% -c src/game.c -o src/game.o
%CC% %CFLAGS% -c src/map.c -o src/map.o
%CC% %CFLAGS% -c src/entity.c -o src/entity.o
%CC% %CFLAGS% -c src/player.c -o src/player.o
%CC% %CFLAGS% -c src/combat.c -o src/combat.o
%CC% %CFLAGS% -c src/ui.c -o src/ui.o
%CC% %CFLAGS% -c src/skill.c -o src/skill.o
%CC% %CFLAGS% -c src/floor.c -o src/floor.o
%CC% %CFLAGS% -c src/world_item.c -o src/world_item.o
%CC% %CFLAGS% -c src/shop.c -o src/shop.o

:: Link
%CC% src/main.o src/game.o src/map.o src/entity.o src/player.o src/combat.o src/ui.o src/skill.o src/floor.o src/world_item.o src/shop.o -o srpg.exe %LDFLAGS%

if %errorlevel% equ 0 (
    echo [Build Successful!]
    echo Running srpg.exe...
    srpg.exe
) else (
    echo [Build Failed!]
    pause
)
