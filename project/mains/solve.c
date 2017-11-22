#define solve_using

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STBIW_ASSERT(x)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "structure_arrays.c"

//need bool for if/else statements
typedef enum {
    false, true
} bool;

// get the time to print how long it took to sove the maze
char* loadTime(){

    time_t current_time;
    char* c_time_string;

    /* Obtain current time. */
    current_time = time(NULL);

    /* Convert to local time format. */
    c_time_string = ctime(&current_time);

    return  c_time_string;
}

typedef struct Image
{
    int height, width;
} Image_Dimentions;

#include "maze.c"
#include "breadthfirst.c"

unsigned char *Maze(Image_Dimentions im, unsigned char image_pixels[], Nodes route_nodes[], Nodes start_node[], Nodes end_node[]);

void solve(char *arguments[]){

    clock_t c_i1, c_i2;

    Image_Dimentions im;
    // Load Image 
    printf("Loading Image \n");
    char *input_file = arguments[1]; // input image to be solve
    char *output_file = arguments[2]; // output solved image.
    // solve_using = argument[0];
    
    int x,y,n;
    unsigned char* image = stbi_load(input_file, &x, &y, &n, 1);
    // rgb is now three bytes per pixel, width*height size. Or NULL if load failed.

    printf("Creating Maze \n");
        //get an instance of get time function
    char* t0 = loadTime();
    

    printf("Maze started creating Nodes at: \n");
    printf(t0);
    c_i1 = clock();

    im.height = x;
    im.width = y;
    int image_size = (x*y);

    //this is to dynamically intiliaze array so that array size/memory can be passed from image size
    Array image_pixels;
    initArray(&image_pixels, image_size);

    // this need to be intilized here so we can realloc more memory
    Nodes *route_nodes = (Nodes *) malloc(sizeof(Nodes));

    Nodes start_node;
    initStruct(&start_node, 1);

    Nodes end_node;
    initStruct(&end_node, 1);

    for(int i = 0; i < image_size; i++){
        image_pixels.array[i] = image[i];
    }

    //send maze class all the info to read nodes and image size
    //getting a pointer back to the new address assigned to the path_node
    //its either same pointer or realloc changed the address in memory
    //either way it will return the proper pointer to address
    unsigned char *path_node = Maze(im, image_pixels.array, route_nodes, &start_node, &end_node);

    //return path_node;

    if(path_node != NULL){
        //get the solved path as a pointer to the localally created array in the file
       int (*solved_path)[2] = solver(im, path_node, start_node, end_node);

       char* t_mazeSolved = loadTime();
       printf("Image Solved at: \n");
       printf(t_mazeSolved);

       //get the size of path
       int path_size = getPathSize(-1);

       int write_image_size = 3*image_size;
       
       unsigned char *result_image = malloc(write_image_size * sizeof(unsigned char));

       int track = 0;
       int isFirst = 0;
       for(int i = 0; i < image_size; i++){
            if(image_pixels.array[i] == 255){
                //add to three more bytes, its a png
                if(isFirst == 0){
                    isFirst = 1;
                    int i_value = 0;
                    while(i_value < 3){
                        if(i_value == 0){
                            result_image[i_value + track] = 255;
                        } else{
                            result_image[i_value + track] = 0;
                        }
                        i_value++;
                        if(i_value == 3){
                            track += 3;
                        }
                    }
                } else{
                    int i_value = 0;
                    while(i_value < 3){
                        result_image[i_value + track] = 255;
                        i_value++;
                        if(i_value == 3){
                            track += 3;
                        }
                    }
                }                
            }

            if(image_pixels.array[i] == 0){
                int i_value = 0;
                while(i_value < 3){
                    result_image[i_value + track] = 0;
                    i_value++;
                    if(i_value == 3){
                        track += 3;
                    } 
                }
            }

       }

    //free the mem reserved for path
        
        for(int i = 0; i < path_size; i++){
            int min;
            int max;

            if(solved_path[i][0] == solved_path[i + 1][0]){
                if(solved_path[i][1] < solved_path[i + 1][1]){
                    min = solved_path[i][1];
                    max = solved_path[i + 1][1];
                } else{
                    min = solved_path[i + 1][1];
                    max = solved_path[i][1];
                }

                for(int j = min; j < max; j++){
                    int k = j + (solved_path[i][0] * y);
                    int i_value = k * 3;
                    int j_value = 0;
                    while(j_value < 3){
                        if(j_value == 0){
                            result_image[k * 3] = 255;
                        } else{
                            result_image[i_value] = 0;
                        }
                        i_value++;
                        j_value++;
                    }
                }
            } else if(solved_path[i][1] == solved_path[i + 1][1]){
                if(solved_path[i][0] < solved_path[i + 1][0]){
                    min = solved_path[i][0];
                    max = solved_path[i + 1][0] + 1;
                } else{
                    min = solved_path[i + 1][0];
                    max = solved_path[i][0] + 1;
                }

                for(int j = min; j < max; j++){
                    int k = (j * y) + solved_path[i][1];
                    int i_value = k * 3;
                    int j_value = 0;
                    while(j_value < 3){
                        if(j_value == 0){
                            result_image[i_value] = 255;
                        } else{
                            result_image[i_value] = 0;
                        }
                        i_value++;
                        j_value++;
                    }
                }
            }
        } 

        STBIW_ASSERT(result_image);
        int image_solve = stbi_write_png(output_file, x, y, 3, result_image, x*3);
        if (image_solve == 1){
            //free the memory after image succesfully created.
            freeArray(&image_pixels);
            stbi_image_free(image); 
            free(result_image);

                //create an instance of get time function
            char* t1 = loadTime();
            printf("Maze solved at: \n");
            printf(t1);
            c_i2 = clock();
            //int total = t1 -t0;
            float diff = ((float)(c_i2 - c_i1) / 1000000.0F ) * 1000;
            printf ("Time elapsed: %f", diff, "\n"); 

            
            //free(&solved_path);
        } else{
            printf("Failed to write file.");
        }
 

    }
}


int main(int argc, char *argv[]) 
{

    //create a struct to get back start_node, end_node, path_node, image_size
    //return all the values from solve function.
    //include selected algoritm from cmd line here inside this function.
    //including inside main funtion is bad but temp solution for now.

   //Nodes *path_node = solve(argv);
    solve(argv);   

    return 0;
}
 