/**
 * @file life.c
 * 
 * @author Garrett Wood
 * 
 * @brief object file for Game of Life
 * 
 * @date 2020-11-09
 * 
 * @todo init 2d array, read from file, read board, generate new generations(optimally if possible), change interpritation based on board type, read 105 files
 * 
 */
#include "life.h"

struct commands_t cmd_interpret(int argc, char* argv[], struct commands_t commands){
    int c;
    int x;
    int y;
    while((c = getopt(argc, argv, "w:h:e:r:g:b:s:f:o:H")) != -1){
        switch(c){
            case 'w':
                if(atoi(optarg) <= 0){
                    printf("invalid width\n");
                    exit(-1);
                }
                if(atoi(optarg) % 2 == 0 || atoi(optarg) == 1){
                    commands.w = atoi(optarg);
                }else{
                    commands.w = atoi(optarg) - 1;
                }
                break;
            case 'h':
                if(atoi(optarg) <= 0){
                    printf("invalid height\n");
                    exit(-1);
                }
                if(atoi(optarg) % 2 == 0 || atoi(optarg) == 1){
                    commands.h = atoi(optarg);
                }else{
                    commands.h = atoi(optarg) - 1;
                }
                break;
            case 'e':
                if(!strcmp(optarg, "hedge")){
                    commands.type = 1;
                }else if(!strcmp(optarg, "torus")){
                    commands.type = 2;
                }else if(!strcmp(optarg, "klein")){
                    commands.type = 3;
                }else{
                    printf("invalid board type\n");
                    exit(-1);
                }
                break;
            case 'r':
                if(atoi(optarg) < 0){
                    commands.r = 0;
                }if(atoi(optarg) > 255){
                    commands.r = 255;
                }else{
                    commands.r = atoi(optarg);
                }
                break;
            case 'g':
                if(atoi(optarg) < 0){
                    commands.g = 0;
                }if(atoi(optarg) > 255){
                    commands.g = 255;
                }else{
                    commands.g = atoi(optarg);
                }
                break;
            case 'b':
                if(atoi(optarg) < 0){
                    commands.b = 0;
                }if(atoi(optarg) > 255){
                    commands.b = 255;
                }else{
                    commands.b = atoi(optarg);
                }
                break;
            case 's':
                switch(atoi(optarg)){
                    case 2:
                    case 4:
                    case 8:
                    case 16:
                        commands.s = atoi(optarg);
                        break;
                    default:
                        commands.s = 2;
                        break;
                    
                }
                break;
            case 'f':
                strcpy(commands.file, optarg);
                break;
            case 'o':
                sscanf(optarg, "%d,%d", &x, &y);
                commands.coords[0] = x;
                commands.coords[1] = y;
                break;
            case 'H':
                print_help();
                break;
        }
    }
    return commands;

}

void print_help(){
    printf("Usage for this program is as follows:\n");
    printf("-w: width of window, int > 0\n");
    printf("-h height of window, int > 0\n");
    printf("-e type of edge to use, hedge, torus, klein. Do not capitalize\n");
    printf("-r the red color value, int between 0 and 255(inclusive)\n");
    printf("-g the green color value, int between 0 and 255(inlcusive)\n");
    printf("-b: the blue color value, int between 0 and 255(inclusive)\n");
    printf("-s: size of the sprite, int 2, 4, 8, 16. Defaults to 2\n");
    printf("-f: filename of the seed file, match case exactly\n");
    printf("-o: initial x,y coordinate of the seed file, x,y\n");
    printf("-H: prints this help statement\n");
    exit(0);

}

int **init_grid(int h, int w){
    int **grid = malloc(h * sizeof(int *));
    int i;
    int j;
    if(grid == NULL){
        printf("not enough memory 1\n");
        exit(-2);
    }
    for(i = 0; i < h; i++){
        *(grid + i) = malloc(w * sizeof(int));
        if(!*(grid + i)){
            for(j = 0; j < i; j++){
                free(*(grid + j));
            }
            free(grid);
            printf("not enough memory 2\n");
            exit(-2);
        }
    }
    for(i = 0; i < w; i++){
        for(j = 0; j < h; j++){
            *(*(grid + j) + i) = 1;
        }
    }
    return grid;
}

void free_grid(int** grid, int h){
    int i;
    for(i = 0; i < h; i++){
        free(grid[i]);
    }
    free(grid);
}