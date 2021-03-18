#define solve_using

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include "../libs/stb_image.h"

#define STBIW_ASSERT(x)

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../libs/stb_image_write.h"

#include "datastructures.c"
#include "generatepath.c"
#include "factory.c"

void solve(char *inputFile, char *outputFile, char *traversalAlgorithm){
  // Load Image 
  printf("Loading Image \n");
  
  int width,height,n;
  unsigned char *image = stbi_load(inputFile, &width, &height, &n, 1);  

  // rgb is now three bytes per pixel, width*height size. Or NULL if load failed.    
  if (image == 0) {
    printf("No image found or proccessed :( \n");
    return;
  }
  printf("Creating Maze \n");
  //get an instance of get time function
  char* t0 = loadTime();

  printf("Maze started creating Nodes at %s: \n", t0);

  clock_t startTime, endTime;

  startTime = clock();

  ImageDimensions im;
  im.height = width;
  im.width = height;
  unsigned int imageSize = (width*height);

  INIT_NODE(startNode);
  INIT_NODE(endNode);

  //send generatepath class all the info to read nodes and image size
  //getting a pointer back to the new address assigned to the graphNodes
  //its either same pointer or realloc changed the address in memory
  //either way it will return the proper pointer to address
  Nodes *graphNodes = generatepath(im, image, &startNode, &endNode);

  //return graphNodes;

  if(graphNodes != NULL){
    //get the solved path as a pointer to the localally created array in the file
    unsigned int (*solvedPath)[2] = factory(traversalAlgorithm, im, graphNodes, startNode, endNode);

    char* tMazeSolved = loadTime();
    printf("Image Solved at: \n");
    printf("%s", tMazeSolved);

    //get the size of path
    unsigned int pathSize = getPathSize();
           
    for(unsigned int i = 0; i < pathSize; i++){
      int min;
      int max;
      if(solvedPath[i][0] == solvedPath[i + 1][0]){
        if(solvedPath[i][1] < solvedPath[i + 1][1]){
          min = solvedPath[i][1];
          max = solvedPath[i + 1][1];
        } else{
          min = solvedPath[i + 1][1];
          max = solvedPath[i][1];
        }

        for(int j = min; j < max; j++){
          int k = j + (solvedPath[i][0] * height);
          image[k] = 100;
        }
      } else if(solvedPath[i][1] == solvedPath[i + 1][1]){
        if(solvedPath[i][0] < solvedPath[i + 1][0]){
          min = solvedPath[i][0];
          max = solvedPath[i + 1][0] + 1;
        } else{
          min = solvedPath[i + 1][0];
          max = solvedPath[i][0] + 1;
        }

        for(int j = min; j < max; j++){
          int k = (j * height) + solvedPath[i][1];
          image[k] = 100;
        }
      }
    }

    STBIW_ASSERT(resultImage);
    int imageSolved = stbi_write_png(outputFile, width, height, 1, image, width);
    if (imageSolved == 1){
      //free the memory after image succesfully created.
      stbi_image_free(image);

      //create an instance of get time function
      char* t1 = loadTime();
      printf("Maze solved at: \n");
      printf("%s", t1);
      endTime = clock();
      float diff = ((float)(endTime - startTime) / 1000000.0F ) * 1000;
      printf ("Time elapsed: %f \n", diff); 
    } else{
      printf("Failed to write file.");
    }
  }
}


int main(int argc, char *argv[]) 
{

  //create a struct to get back startNode, endNode, graphNodes, imageSize
  //return all the values from solve function.
  //include selected algoritm from cmd line here inside this function.
  //including inside main funtion is bad but temp solution for now.

  char *inputFile = argv[1]; // input image to be solve
  char *outputFile = argv[2]; // output solved image.
  char *traversalAlgorithm = argv[3]; // traversal algorithm used to solve
  solve(inputFile, outputFile, traversalAlgorithm); 
  return 0;
}
 
