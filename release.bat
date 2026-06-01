@echo off
echo [Building SRPG - STANDALONE RELEASE MODE]

set CC=gcc
:: Added -O2 for optimization and -DNDEBUG to disable debug logs
set CFLAGS=-O2 -Wall -Wextra -std=c11 -I. -DNDEBUG
:: Added -static to embed all libraries (including CRT) into the EXE
set LDFLAGS=-L"C:/raylib/lib" -lraylib -lgdi32 -lwinmm -static

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

echo Linking Standalone Binary...
%CC% core/core.o map/map.o entity/entity.o growth/growth.o battle/battle.o item/item.o item/db/item_database.o entity/equipment_system.o ui/ui.o ui/ui_logic.o ui/inventory_draw.o main.o -o srpg_release.exe %LDFLAGS%

if %errorlevel% equ 0 (
    echo [Standalone Build Successful!]
    echo This EXE can now run on PCs without raylib installed.
    echo Binary created: srpg_release.exe
) else (
    echo [Standalone Build Failed!]
    pause
)
