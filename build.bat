@echo off
echo [Building SRPG - Phase 5 Integrated]

set CC=gcc
set CFLAGS=-Wall -Wextra -std=c11 -I.
set LDFLAGS=-L"C:/raylib/lib" -lraylib -lgdi32 -lwinmm

echo Compiling core...
%CC% %CFLAGS% -c core/core.c -o core/core.o
echo Compiling map...
%CC% %CFLAGS% -c map/map.c -o map/map.o
echo Compiling entity...
%CC% %CFLAGS% -c entity/entity.c -o entity/entity.o
echo Compiling growth...
%CC% %CFLAGS% -c growth/growth.c -o growth/growth.o
echo Compiling battle...
%CC% %CFLAGS% -c battle/battle.c -o battle/battle.o
echo Compiling item...
%CC% %CFLAGS% -c item/item.c -o item/item.o
echo Compiling item db...
%CC% %CFLAGS% -c item/db/item_database.c -o item/db/item_database.o
echo Compiling equipment...
%CC% %CFLAGS% -c entity/equipment_system.c -o entity/equipment_system.o
echo Compiling ui...
%CC% %CFLAGS% -c ui/ui.c -o ui/ui.o
%CC% %CFLAGS% -c ui/ui_logic.c -o ui/ui_logic.o
%CC% %CFLAGS% -c ui/inventory_draw.c -o ui/inventory_draw.o
echo Compiling main...
%CC% %CFLAGS% -c main.c -o main.o

echo Linking...
%CC% core/core.o map/map.o entity/entity.o growth/growth.o battle/battle.o item/item.o item/db/item_database.o entity/equipment_system.o ui/ui.o ui/ui_logic.o ui/inventory_draw.o main.o -o srpg.exe %LDFLAGS%

if %errorlevel% equ 0 (
    echo [Build Successful!]
    srpg.exe
) else (
    echo [Build Failed!]
    pause
)
