#include "../include/macros.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>



void update_canvas(int** current_canvas, int** next_canvas, int h, int w) {
    for (int i=0; i < h; i++) {
        for (int j=0; j<w; j++ ) {

            int count = 0;

            if(current_canvas[i][(j +1) %w]) count++;
            if(current_canvas[i][j-1 < 0 ? w-1 : j-1]) count++;
            if(current_canvas[(i+1) % h][j-1 < 0 ? w-1 : j-1]) count++;
            if(current_canvas[(i+1) % h][j]) count++;
            if(current_canvas[(i+1) % h][(j +1) %w]) count++;
            if(current_canvas[i-1 < 0 ? h-1 : i-1][j-1 < 0 ? w-1 : j-1]) count++;
            if(current_canvas[i-1 < 0 ? h-1 : i-1][j]) count++;
            if(current_canvas[i-1 < 0 ? h-1 : i-1][(j +1) %w]) count++;

            if(count == 3) next_canvas[i][j] = 1;
            else if (current_canvas[i][j] && count>=2 && count <= 3) next_canvas[i][j] = 1;
            else next_canvas[i][j] = 0;
        }
    }
}

void print_canvas(int** canvas, int h, int w, int generation) {

    printf("\n\n------------------ generation: %d -------------------\n", generation);

   
    for (int i=0; i < h; i++) {
         printf("|");
        for (int j=0; j<w; j++ ) {
            printf("%s", canvas[i][j] == 0 ? " " : "*");
        }

    printf("|\n");
    }

    printf("---------------- ende generation: %d -----------------\n", generation);




} 


int main(int argc, char const *argv[]) {

    if (argc < 5) {
        printf("USAGE: gol <height> <width> <generations> <sleeptime>");
        exit(1);
    }


    int h = atoi(argv[1]);
    int w = atoi(argv[2]);
    int generations = atoi(argv[3]);
    int sleepms = atoi(argv[4]);


    if(h < 3 || w < 3) {
        printf("size must be at least 3x3");
        exit(1);
    }


    int **current_canvas, **next_canvas;

    ALLOC_ARRAY2D(current_canvas,int,h,w,0);

    current_canvas[0][1] = 1;
    current_canvas[1][2] = 1;
    current_canvas[2][0] = 1;
    current_canvas[2][1] = 1;
    current_canvas[2][2] = 1;



    print_canvas(current_canvas, h, w, 0);

    for (int g = 0; g < generations || generations == -1; g++) {
        usleep(sleepms * 1000);
        ALLOC_ARRAY2D(next_canvas,int,h,w,0);

        update_canvas(current_canvas, next_canvas, h, w);
        print_canvas(next_canvas, h, w, g+1);

        //swap canvases
        DEL_BLOCK2D(current_canvas);
        current_canvas = next_canvas;
    }

    DEL_BLOCK2D(current_canvas);


}