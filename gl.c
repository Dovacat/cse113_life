/**
 * @file gl.c
 * 
 * @author Garrett Wood
 * 
 * @brief Game of Life main code
 * 
 * @date 2020-11-09
 * 
 * @todo init 2d array, read from file, read board, generate new generations(optimally if possible), change interpritation based on board type, read 105 files
 * 
 */
#include <stdlib.h>
#include "SDL2/SDL.h" 
#include "sdl.h"
#include "life.h"

int main(int argc, char *argv[])
{
	struct commands_t commands;
	commands = cmd_interpret(argc, argv, commands);
	int w = commands.w / commands.s;
	int h = commands.h / commands.s;
	int **grid = init_grid(h, w);
	printf("test\n");
	int **temp = init_grid(h, w);
	//allocate arrays
	/*
	int width = 800;
	int height = 600;
	int sprite_size = 4; //either 2, 4, 8, or 16
	int m = -66;
	int n = -10;
        //colors are RGB model valid values [0, 255]
	unsigned char red = 140;
	unsigned char green = 145;
	unsigned char blue = 250;
    struct sdl_info_t sdl_info; //this is needed to graphically display the game
        
        //set up SDL -- works with SDL2
	init_sdl_info(&sdl_info, width, height, sprite_size, red, green, blue);

	//your life initialization code here
	
        //Main loop: loop forever.
	while (1)
	{
		//your game of life code goes here		

		//change the  modulus value to slow the rendering
		 //if (SDL_GetTicks() % 1 == 0)
			sdl_test(&sdl_info, m, n);

                 //Poll for events, and handle the ones we care about. 
                 //You can click the X button to close the window
                  
		SDL_Event event;
		while (SDL_PollEvent(&event)) 
		{
			switch (event.type) 
			{
			case SDL_KEYDOWN:
				break;
			case SDL_KEYUP:
                        //If escape is pressed, return (and thus, quit)
				if (event.key.keysym.sym == SDLK_ESCAPE)
					return 0;
				break;
			case SDL_QUIT:
				return(0);
			}
		}
	}*/
	free_grid(grid, h);
	free_grid(temp, h);
	return 0;
}
