# Windows Makefile for SRPG
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude -I"C:/raylib/include"
LDFLAGS = -L"C:/raylib/lib" -lraylib -lgdi32 -lwinmm

# Objects
OBJS = src/main.o src/game.o src/map.o src/entity.o src/player.o

# Target
TARGET = srpg.exe

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	del /q src\*.o $(TARGET)
