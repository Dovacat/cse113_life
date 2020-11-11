/**
 * @file life.h
 * 
 * @author Garrett Wood
 * 
 * @brief header file for Game of Life
 * 
 * @date 2020-11-09
 */
#ifndef life_h
#define life_h
#include <stdio.h>
#include <stdlib.h>
#include "SDL2/SDL.h" 
#include "sdl.h"
#include <unistd.h>
#include <string.h>

struct commands_t{
    int w;
    int h;
    int type;
    int r;
    int g;
    int b;
    int s;
    char file[30];
    int coords[2];
};
struct commands_t cmd_interpret(int argc, char* argv[], struct commands_t commands);

void print_help();

unsigned char **init_grid(int w, int h);

void free_grid(unsigned char** grid, int h);

int read_106(char file[], int array[]);

void get_offest(int offset_x, int offset_y, int map[], int size, int w, int h);

unsigned char **populate_grid(int map[], int size, unsigned char **grid);
#endif
