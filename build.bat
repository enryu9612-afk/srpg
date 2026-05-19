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

:: Link (combat.o 추가)
%CC% src/main.o src/game.o src/map.o src/entity.o src/player.o src/combat.o -o srpg.exe %LDFLAGS%

if %errorlevel% equ 0 (
    echo [Build Successful!]
    echo Running srpg.exe...
    srpg.exe
) else (
    echo [Build Failed!]
    pause
)
