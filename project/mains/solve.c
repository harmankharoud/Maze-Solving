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

#include "maze.c"
#include "factory.c"

void solve(char *inputFile, char *outputFile, char *traversalAlgorithm){
  // Load Image 
  printf("Loading Image \n");
  
  int width,height,n;
  unsigned char* image = stbi_load(inputFile, &width, &height, &n, 1);

  // rgb is now three bytes per pixel, width*height size. Or NULL if load failed.    
  if (image == 0) {
    printf("No image found or proccessed :( \n");
    return;
  }
  

  printf("Creating Maze \n");
  //get an instance of get time function
  char* t0 = loadTime();
  

  printf("Maze started creating Nodes at %s:", t0);

  clock_t startTime, endTime;

  startTime = clock();

  Image_Dimentions im;
  im.height = width;
  im.width = height;
  int imageSize = (width*height);

  //This is to dynamically intiliaze array so that array size/memory can be passed from image size
  Array imagePixels;
  initArray(&imagePixels, imageSize);

  //This need to be intilized here so we can realloc more memory
  Nodes *routeNodes = (Nodes *) malloc(sizeof(Nodes));

  Nodes startNode;
  initStruct(&startNode, 1);

  Nodes endNode;
  initStruct(&endNode, 1);

  for(int i = 0; i < imageSize; i++){
    imagePixels.array[i] = image[i];
  }

  //send maze class all the info to read nodes and image size
  //getting a pointer back to the new address assigned to the pathNode
  //its either same pointer or realloc changed the address in memory
  //either way it will return the proper pointer to address
  Nodes *pathNode = Maze(im, imagePixels.array, routeNodes, &startNode, &endNode);

  //return pathNode;

  if(pathNode != NULL){
    //get the solved path as a pointer to the localally created array in the file
    int (*solvedPath)[2] = factory(traversalAlgorithm, im, pathNode, startNode, endNode);

    char* tMazeSolved = loadTime();
    printf("Image Solved at: \n");
    printf("%s", tMazeSolved);

    //get the size of path
    int pathSize = getPathSize(-1);

    int resultImageSize = 3*imageSize;
    
    unsigned char *resultImage = malloc(resultImageSize * sizeof(unsigned char));

    int track = 0;
    int isFirst = 0;
    for(int i = 0; i < imageSize; i++){
      if(imagePixels.array[i] == 255){
          //add to three more bytes, its a png
        if(isFirst == 0){
          isFirst = 1;
          int rgbValue = 0;
          while(rgbValue < 3){
            if(rgbValue == 0){
              resultImage[rgbValue + track] = 255;
            } else{
              resultImage[rgbValue + track] = 0;
            }
            rgbValue++;
            if(rgbValue == 3){
              track += 3;
            }
          }
        } else{
          int rgbValue = 0;
          while(rgbValue < 3){
            resultImage[rgbValue + track] = 255;
            rgbValue++;
            if(rgbValue == 3){
              track += 3;
            }
          }
        }                
      }

      if(imagePixels.array[i] == 0){
        int rgbValue = 0;
        while(rgbValue < 3){
          resultImage[rgbValue + track] = 0;
          rgbValue++;
          if(rgbValue == 3){
            track += 3;
          } 
        }
      }
    }

    //free the mem reserved for path        
    for(int i = 0; i < pathSize; i++){
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
          int offsetValue = k * 3;
          int rgbValue = 0;
          while(rgbValue < 3){
            if(rgbValue == 0){
              resultImage[k * 3] = 255;
            } else{
              resultImage[offsetValue] = 0;
            }
            offsetValue++;
            rgbValue++;
          }
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
          int offsetValue = k * 3;
          int rgbValue = 0;
          while(rgbValue < 3){
            if(rgbValue == 0){
              resultImage[offsetValue] = 255;
            } else{
              resultImage[offsetValue] = 0;
            }
            offsetValue++;
            rgbValue++;
          }
        }
      }
    } 

    STBIW_ASSERT(resultImage);
    int imageSolved = stbi_write_png(outputFile, width, height, 3, resultImage, width*3);
    if (imageSolved == 1){
      //free the memory after image succesfully created.
      freeArray(&imagePixels);
      stbi_image_free(image); 
      free(resultImage);

      //create an instance of get time function
      char* t1 = loadTime();
      printf("Maze solved at: \n");
      printf("%s", t1);
      endTime = clock();
      float diff = ((float)(endTime - startTime) / 1000000.0F ) * 1000;
      printf ("Time elapsed: %f \n", diff); 

      
      //free(&solvedPath);
    } else{
      printf("Failed to write file.");
    }
  }
}


int main(int argc, char *argv[]) 
{

  //create a struct to get back startNode, endNode, pathNode, imageSize
  //return all the values from solve function.
  //include selected algoritm from cmd line here inside this function.
  //including inside main funtion is bad but temp solution for now.

  char *inputFile = argv[1]; // input image to be solve
  char *outputFile = argv[2]; // output solved image.
  char *traversalAlgorithm = argv[3]; // traversal algorithm used to solve
  solve(inputFile, outputFile, traversalAlgorithm); 
  return 0;
}
 
