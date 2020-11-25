/**
 * @file life.c
 * 
 * @author Garrett Wood
 * 
 * @brief object file for Game of Life
 * 
 * @date 2020-11-09
 * 
 * @todo change interpritation based on board type, read 105 files
 * 
 */
#include "life.h"

/** fills the commands struct based on args given at run
 * 
 * @param argc the number of args given in the terminal
 * @param argv the command string
 * @param commands the struct that is populated
 * @return struct commands_t the populated struct
 * @remarks since im lazy and dont want to enter commands every time, all values are defaulted except filename at the start
 */
struct commands_t cmd_interpret(int argc, char* argv[], struct commands_t commands){
    int c;
    int x;
    int y;
    //inits commands in case of no input
    
    commands.w = 800;
    commands.h = 800;
    commands.type = 1;
    commands.r = 255;
    commands.g = 0;
    commands.b = 0;
    commands.s = 2;
    commands.coords[0] = 0;
    commands.coords[1] = 0;
    //loop for getting commands
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
                commands.coords[1] = x;
                commands.coords[0] = y;
                break;
            case 'H':
                print_help();
                break;
        }
    }
    return commands;

}

/** prints the use of the program/explains what arguments do
 */
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

/** inits the 2d array used for the grid
 * @param h the height of the grid
 * @param w the width of the grid
 * @return unsigned char** the generated 2d array
 */
unsigned char **init_grid(int w, int h){
    unsigned char **grid = malloc(h * sizeof(unsigned char *));
    int i;
    int j;
    if(grid == NULL){
        printf("not enough memory\n");
        exit(-2);
    }
    for(i = 0; i < h; i++){
        *(grid + i) = malloc(w * sizeof(unsigned char));
        if(!*(grid + i)){
            for(j = 0; j < i; j++){
                free(*(grid + j));
            }
            free(grid);
            printf("not enough memory\n");
            exit(-2);
        }
    }/*
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            grid[i][j] = 1;
        }
    }*/
    return grid;
}

/** frees the memory allocated for the grid
 * @param grid the grid to be freed
 * @param w the width of the grid
 */
void free_grid(unsigned char** grid, int h){
    int i;
    for(i = 0; i < h; i++){
        free(grid[i]);
    }
    free(grid);
}

/** reads 106 type files
 * @param fname the file name to be read
 * @param array the array that is populated with the file contents
 * @return int the number of coordinates
 */
int read_106(char fname[], int array[]){
    int i = 0;
    FILE *file = fopen(fname, "r");
    if(file != NULL){
        char line[50];
        while(fgets(line, sizeof(line), file) != NULL){
            if(line[0] == '#'){
            }else{
                sscanf(line, "%d %d", &array[i], &array[i + 1]);
                i += 2;
            }
        }
        
    }else{
        printf("File not found\n");
        exit(-3);
    }
    fclose(file);
    return i;
}

/** changes starting position of map based on user input and whether or not cells will be out of bounds
 * @param offset_x the user entered x offset
 * @param offset_y the user entered y offset
 * @param map the seed map
 * @param size number of coordinates in the seed map
 * @param w the width of the grid
 * @param h the height of the grid
 */
void get_offest(struct commands_t commands, int map[], int size, int w, int h){
    int i;
    //changes offset
    for(i = 0; i < size; i += 2){
        if(map[i] + commands.coords[0] < 0){
            commands.coords[0] -= map[i];
        }
        if(map[i] + commands.coords[0] >= h){ //checks if coordinate will go off the screen
            printf("Entered starting coordinates out of bounds!\n");
            exit(-4);
        }
        if(map[i + 1] + commands.coords[1] < 0){
            commands.coords[1] -= map[i + 1];
        }
        if(map[i + 1] + commands.coords[1] >= w){
            printf("Entered starting coordinates out of bounds!\n");
            exit(-4);
        }
    }//if board type is hedge, increases offset by 1 to account for the edge
    if(commands.type == 1){
        commands.coords[0]++;
        commands.coords[1]++;
    }
    printf("offset %d, %d\n", commands.coords[0], commands.coords[1]);
    //changes map seed values by offset
    for(i = 0; i < size; i += 2){
        map[i] += commands.coords[0];
        map[i + 1] += commands.coords[1];
        //printf("%d, %d\n", map[i], map[i + 1]);
    }

}

/** populates grid based on map
 * @param map the seed map
 * @param size the number of coordinates in the map
 * @param grid the grid that will be populated
 * @return unsigned char** the populated grid
 */
unsigned char **populate_grid(int map[], int size, unsigned char **grid){
    int i;
    for(i = 0; i < size; i += 2){
        grid[map[i]][map[i + 1]] = 1;
    }
    return grid;
}

/** pushes staged changes to the display grid
 * @param w width of grid
 * @param h height of grid
 * @param grid the displayed grid
 * @param temp the temp grid where changes are staged
 * @return unsigned char** the updated display grid
 */
unsigned char **update_grid(int w, int h, unsigned char **grid, unsigned char **temp){
    int i;
    int j;
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            grid[i][j] = temp[i][j];
        }
    }
    return grid;
}

/** determines what a cell should do(live/die)
 * @param sum the sum of the neighbors
 * @param state the current state of the cell
 * @return int the updated state of the cell
 * @remarks since all alive cells have a value of 1, I can just add up all the neighbors to determine what a cell should do
 */
int behavior(int sum, int state){
    if(state){
        if(sum < 2){
            return 0;
        }else if(sum > 3){
            return 0;
        }
        return 1;
    }else{
        if(sum == 3){
            return 1;
        }
    }
    return state;
}

/** updates hedge type board
 * @param grid the grid that is displayed
 * @param temp the temp grid where updates are staged
 * @param w the width of the grid
 * @param h the height of the grid
 * @return unsigned char** the updated grid
 * @remarks I wish this was a better implementation, Ill change it if it becomes problematic or I think of a better solution
 */
unsigned char **update_hedge(unsigned char **grid, unsigned char **temp, int w, int h){
    int sum;
    int i;
    int j;
    /*//prints array for debugging purposes
    for(i = 0; i < h; i++){
        for(j = 0; j < w; j++){
            printf("%d", temp[i][j]);
        }
        printf("\n");
    }*/
    //inits edges to 0
    for(i = 0; i < h; i++){
        temp[0][i] = 0;
        temp[h - 1][i] = 0;
    }
    for(i = 0; i < w; i++){
        temp[i][0] = 0;
        temp[i][w - 1] = 0;
    }
    for(j = 1; j < w - 1; j++){
        for(i = 1; i < h - 1; i++){
            //I hate this line but it will be here unless I think of something else
            sum = grid[i - 1][j - 1] + grid[i - 1][j] + grid[i - 1][j + 1] + grid[i][j - 1] + grid[i][j + 1] + grid[i + 1][j - 1] + grid[i + 1][j] + grid[i + 1][j + 1];
            if(sum == 3 || grid[i][j]){
                temp[i][j] = behavior(sum, grid[i][j]);
            }
        }
    }
    return temp;
}

/** updates torus type board
 * @param grid the grid that is diplayed
 * @param temp the temp grid where updates are staged
 * @param w the width of the grid
 * @param h the height of the grid
 * @return unsigned char** the updated grid
 * @remarks Oh lord I hate this thing but its the only way I can come up with atm, might try a diff solution later
 * @remarks actually I found a cool ish solution
 */
unsigned char **update_torus(unsigned char **grid, unsigned char **temp, int w, int h){
    int i; // Row index
    int j; // Column
    int sum = 0;
    for(j = 0; j < w; j++){
        for(i = 0; i < h; i++){
            sum = grid[(i + h - 1) % h][(j + w - 1) % w] + grid[(i + h - 1) % h][j] + grid[(i + h - 1) % h][(j + 1) % w] + grid[i][(j + w - 1) % w] + grid[i][(j + 1) % w] + grid[(i + 1) % h][(j + w - 1) % w] + grid[(i + 1) % h][j] + grid[(i + 1) % h][(j + 1) % w];
            if(sum == 3 || grid[i][j]){
                temp[i][j] = behavior(sum, grid[i][j]);
            }
        }
    }
    return temp;
}
/** updates klein bottle board type
 * @param grid the grid that is diplayed
 * @param temp the temp grid where updates are staged
 * @param w the width of the grid
 * @param h the height of the grid
 * @return unsigned char** the updated grid
 */
unsigned char **update_klein(unsigned char **grid, unsigned char **temp, int w, int h){
    int i;
    int j;
    int sum;
    for(j = 0; j < w; j++){
        for(i = 0; i < h; i++){
            if(i == 0){
                sum = grid[(i + h - 1) % h][(2 * w - (j + w - 1)) % w] + grid[(i + h - 1) % h][w - j] + grid[(i + h - 1) % h][(2 * w - (j + 1)) % w] + grid[i][(j + w - 1) % w] + grid[i][(j + 1) % w] + grid[(i + 1) % h][(j + w - 1) % w] + grid[(i + 1) % h][j] + grid[(i + 1) % h][(j + 1) % w];
            }else if(i == h - 1){
                sum = grid[(i + h - 1) % h][(j + w - 1) % w] + grid[(i + h - 1) % h][j] + grid[(i + h - 1) % h][(j + 1) % w] + grid[i][(j + w - 1) % w] + grid[i][(j + 1) % w] + grid[(i + 1) % h][(2 * w - (j + w - 1)) % w] + grid[(i + 1) % h][w - j] + grid[(i + 1) % h][(2 * w - (j + 1)) % w];
            }else{
                sum = grid[(i + h - 1) % h][(j + w - 1) % w] + grid[(i + h - 1) % h][j] + grid[(i + h - 1) % h][(j + 1) % w] + grid[i][(j + w - 1) % w] + grid[i][(j + 1) % w] + grid[(i + 1) % h][(j + w - 1) % w] + grid[(i + 1) % h][j] + grid[(i + 1) % h][(j + 1) % w]
            }
            if(sum == 3 || grid[i][j]){
                temp[i][j] = behavior(sum, grid[i][j]);
            }
        }
    }

    return temp;
}