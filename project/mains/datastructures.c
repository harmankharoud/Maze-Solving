
// intilize node to have all zeros so that nothing is out of the blue garbage memory.
#define INIT_NODE(X) Nodes X = {.position = {0, 0}, .neighbours = {{0, 0}, {0, 0}, {0, 0}, {0, 0}}}

//@TODO should rename this to NODE since its just one node
typedef struct Nodes {
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
  unsigned int height, width;
} ImageDimensions;

void allocateMoreMem(Nodes **graphNodes, unsigned int size)
{
  Nodes *temp = (Nodes*)realloc(*graphNodes, (size * sizeof(Nodes)));

  if (temp == 0) {
    printf("Cannot allocate more memory.\n");
  } else {
    *graphNodes = temp;
  }
}

unsigned int graphNodeSize = 0;
unsigned int getGraphNodesSize(){
  return graphNodeSize;
}
void setGraphNodesSize(unsigned int size) {
  graphNodeSize = size;
}

unsigned int pathSize = 0;
void setPathSize(size){  
  pathSize = size;
}

unsigned int getPathSize() {
  return pathSize;
}
