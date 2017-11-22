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

void initArray(Array *a, int array_size) {
  a->array = (unsigned char *)malloc(array_size * sizeof(unsigned char));
}

void initStruct(Nodes *n, int size) {
    n = (Nodes *)malloc(size * sizeof(Nodes));
}

int total_nodes = 1;
int addMemory(Nodes **n, int currentSize, int numNewElems, int isNode)
{
    const int totalSize = currentSize + numNewElems;
    Nodes *temp = (Nodes*)realloc(*n, (totalSize * sizeof(Nodes)));

    if (temp == NULL) {
        printf("Cannot allocate more memory.\n");
        return 0;
    } else {
        *n = temp;
    }

    if(isNode == 0){
      total_nodes = totalSize;
    }

    return totalSize;
}

int getNodeSize(){
  return total_nodes;
}

int path_size = 0;
int getPathSize(size){
  if(size != -1){
    path_size = size;
  }
  return path_size;
}

void freeArray(Array *a) {
  free(a->array);
}

void freeStruct(Nodes *n, Path *p){
  free(n);
  free(p);
}