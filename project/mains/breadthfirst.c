
void addToQueue(Nodes to[], Nodes from, int size){
  for(int i = 0; i < size; i++){
    to[i] = from;
  }
}

void assignNode(Nodes **current, Nodes from){
  *current = &from;
}


void unshiftQueue(Nodes *node, int size, Nodes *newNode){
  for (int i = size; i > 0; i--) {      
    Nodes *tempUnshift = (Nodes *) malloc(sizeof(Nodes));    
    memcpy(tempUnshift, &node[i - 1], sizeof(Nodes));    
    node[i] = tempUnshift[0];      
  }

  memcpy(&node[0], newNode, sizeof(Nodes));
}

void removeNode(Nodes *node, int index, int size){
  int i;
  for(i = index; i < size - 1; i++) node[i] = node[i + 1];
}

void getFromNodes(Nodes **storeTo, Nodes *routeNodes, unsigned int compare[2]){
  int nodesSize = getNodeSize(); 
  for (int i = 0; i < nodesSize; i++){
    if(routeNodes[i].position[0] == compare[0] && routeNodes[i].position[1] == compare[1] ){
      *storeTo = &routeNodes[i];            
    }
  }
}

int (*solver(Image_Dimentions im, Nodes routeNodes[], Nodes startNode, Nodes endNode))[2]{
  int height = im.height;
  int width = im.width;

  int imageSize = height * width;
  Nodes *queue = (Nodes *) malloc(sizeof(Nodes));

  int (*prev)[2] = malloc(imageSize * sizeof(int[1][2]));

  addToQueue(queue, startNode, 1);

  int *visited = (int *)malloc(imageSize *sizeof(int));

  for(int i = 0; i < imageSize; i++){
      visited[i] = 0;
  }

  visited[startNode.position[0] * width + startNode.position[1]] = 1;

  int size = 1;
    
  while(queue){
    //get the current node as the last element from queue/pop queue
    Nodes *current = (Nodes *) malloc(sizeof(Nodes));

    assignNode(&current, queue[size - 1]);
    removeNode(queue, size - 1, size); 

    size = size - 1;

    Nodes *tmp = realloc(queue, (size) * sizeof(Nodes));
    if (tmp == NULL && size > 1) {
      /* No memory available */
      printf("No memory available");
    }
    queue = tmp;
    
    if(current->position[0] == endNode.position[0] && current->position[1] == endNode.position[1]){
      break;
    }

    for(int i = 0; i < 4; i++){
      if((current->neighbours[i][0] != 0) && (current->neighbours[i][1] != 0)){
        int nPos = current->neighbours[i][0] * width + current->neighbours[i][1];
        if(visited[nPos] == 0) {
          prev[nPos][0] = current->position[0];
          prev[nPos][1] = current->position[1];

          visited[nPos] = 1;
          
          size++;

          Nodes *temp = (Nodes *) malloc(sizeof(Nodes));

          getFromNodes(&temp, routeNodes, current->neighbours[i]);
          addMemory(&queue, size, 1, -1);
          unshiftQueue(queue, size, temp);
        }
      }
    }
  }

  int (*path)[2] = malloc(sizeof(int[1][2]));
  int (*nCurrent)[2] = malloc(sizeof(int[1][2]));
  nCurrent[0][0] = endNode.position[0];
  nCurrent[0][1] = endNode.position[1];
  int pathSize = 0;
  
  while (nCurrent[0][1] && nCurrent[0][0]) {
    pathSize++;
    
    for (int i = pathSize; i > 0; i--) {
      int (*tempUnshift)[2] = malloc(sizeof(int[1][2]));          
      memcpy(&tempUnshift[0], &path[i - 1], sizeof(int[1][2]));          
      path[i][0] = tempUnshift[0][0];
      path[i][1] = tempUnshift[0][1];

      free(tempUnshift);
    }

    int (*tempCurrent)[2] = malloc(sizeof(int[1][2]));
    tempCurrent[0][0] = nCurrent[0][0];
    tempCurrent[0][1] = nCurrent[0][1];    
    memcpy(&path[0], &tempCurrent[0], sizeof(int[1][2])); 
    int position = nCurrent[0][0] * width + nCurrent[0][1];
    nCurrent[0][0] = prev[position][0];
    nCurrent[0][1] = prev[position][1]; 

    free(tempCurrent); 
  }

  getPathSize(pathSize);
  free(nCurrent);
  free(queue);
  free(routeNodes);
  free(prev);

  return path;

}