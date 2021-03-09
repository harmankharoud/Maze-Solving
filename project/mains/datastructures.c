//the nodes from the image where possible to move up/below/left/right
typedef struct Nodes{
  unsigned int position[2];
  unsigned int neighbours[4][2];
} Nodes;

typedef struct {
  unsigned char *array;
} Array;

typedef struct Path{
  unsigned int position[2];
} Path;

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
} ImageDimensions;

void initArray(Array *a, int array_size) {
  a->array = (unsigned char *)malloc(array_size * sizeof(unsigned char));
}

void initStruct(Nodes *n, int size) {
    n = (Nodes *)malloc(size * sizeof(Nodes));
}

int totalNodes = 1;
int addMemory(Nodes **n, int currentSize, int numNewElems, int isNode)
{
    const int totalSize = currentSize + numNewElems;
    Nodes *temp = (Nodes*)realloc(*n, (totalSize * sizeof(Nodes)));

    if (temp == 0) {
        printf("Cannot allocate more memory.\n");
        return 0;
    } else {
        *n = temp;
    }

    if(isNode == 0){
      totalNodes = totalSize;
    }

    return totalSize;
}

int getNodeSize(){
  return totalNodes;
}

int pathSize = 0;
int getPathSize(size){
  if(size != -1){
    pathSize = size;
  }
  return pathSize;
}

void freeArray(Array *a) {
  free(a->array);
}

void freeStruct(Nodes *n, Path *p){
  free(n);
  free(p);
}
