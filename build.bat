@echo off
echo [Building SRPG - Phase 1...]

set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -Icore -Iui -I"C:/raylib/include"
set LDFLAGS=-L"C:/raylib/lib" -lraylib -lgdi32 -lwinmm

%CC% %CFLAGS% -c core/core.c -o core/core.o
%CC% %CFLAGS% -c ui/ui.c    -o ui/ui.o
%CC% %CFLAGS% -c main.c     -o main.o

%CC% core/core.o ui/ui.o main.o -o srpg.exe %LDFLAGS%

if %errorlevel% equ 0 (
    echo [Build Successful!]
    srpg.exe
) else (
    echo [Build Failed!]
    pause
)
