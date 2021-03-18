
void getNodeFromGraphNodes(Nodes **storeTo, Nodes *graphNodes, unsigned int compare[2], unsigned int graphNodesSize){
  for (int i = 0; i < graphNodesSize; i++){
    if(graphNodes[i].position[0] == compare[0] && graphNodes[i].position[1] == compare[1] ){
      *storeTo = &graphNodes[i];
      break;          
    }
  }
}

unsigned int (*bfs(ImageDimensions im, Nodes graphNodes[], Nodes startNode, Nodes endNode))[2]{
  unsigned int height = im.height;
  unsigned int width = im.width;
  unsigned int imageSize = height * width;

  // Intialize queue with 64% of the graphNodes memory allocation.
  unsigned int graphNodesSize = getGraphNodesSize();
  // unsigned int queueSize = (unsigned int)graphNodeSize * 0.64;
  Nodes *queue = (Nodes *) malloc(graphNodeSize * sizeof(Nodes));

  // Manually allocating memory so we can deallocate once done with traversal search aka this class.
  unsigned int (*prev)[2] = malloc(imageSize * sizeof(unsigned int[1][2]));

  unsigned int queueIndex = 0;

  queue[queueIndex] = startNode;

  unsigned int *visited = (unsigned int *)malloc(imageSize * sizeof(unsigned int));

  memset(visited, 0, sizeof(imageSize));
  unsigned int nPos = startNode.position[0] * width + startNode.position[1];
  visited[nPos] = 1;
  unsigned int currIndex = 0;
    
  while(queue){
    //@TODO Fix this and use queueSize variable on line: 18, will help with memory management
    // if(queueIndex >= queueSize) { 
    //   // Reallocate more memory
    //   // Reallocation can be done based index
    //   // If index is still half way through
    //   // Allocate same size again and 
    //   // If index is quater the way reallocate half of the orginal allocation and so on...
    //   unsigned int size = (unsigned int)queueSize / 2;
    //   queueSize += size;

    //   allocateMoreMem(&queue, queueSize);
    // }
    
    Nodes *current = (Nodes *) malloc(sizeof(Nodes));
    current = &queue[currIndex];
    if(currIndex <= queueIndex) currIndex++;

    // if(current->position[0] == 0 && current->position[1] == 0) break;;

    if(current->position[0] == endNode.position[0] && current->position[1] == endNode.position[1]){
      break;
    }

    for(int i = 0; i < 4; i++){
      if(current->neighbours[i][0] != 0 && current->neighbours[i][1] != 0){
        int nPos = current->neighbours[i][0] * width + current->neighbours[i][1];
        if(visited[nPos] == 0) {
          prev[nPos][0] = current->position[0];
          prev[nPos][1] = current->position[1];
          visited[nPos] = 1;
          queueIndex++;
          Nodes *temp = (Nodes *) malloc(sizeof(Nodes));
          getNodeFromGraphNodes(&temp, graphNodes, current->neighbours[i], graphNodesSize);
          queue[queueIndex] = *temp;
        }
      }
    }
  }


  unsigned int (*revPath)[2] = malloc(imageSize * sizeof(int[1][2]));
  unsigned int pathIndex = 0;
  unsigned int pathSize = 1;
  unsigned int desitnationPos = endNode.position[0] * width + endNode.position[1];
  
  while (true) {
    revPath[pathIndex][0] = prev[desitnationPos][0];
    revPath[pathIndex][1] = prev[desitnationPos][1];
    pathSize++;
    if(prev[desitnationPos][0] == startNode.position[0] && prev[desitnationPos][1] == startNode.position[1]) break;
    desitnationPos = revPath[pathIndex][0] * width + revPath[pathIndex][1];
    pathIndex++;
  }
  unsigned int (*path)[2] = malloc(pathSize * sizeof(int[1][2]));
  for (int i = pathIndex, j = 0; i >= 0; i--, j++)
  {
    path[j][0] = revPath[i][0];
    path[j][1] = revPath[i][1];
  }

  memcpy(&path[pathIndex][0], &revPath[0][0], sizeof(unsigned int[1][2]));  
  pathIndex++;
  memcpy(&path[pathIndex][0], &endNode.position[0], sizeof(unsigned int[1][2]));  

  setPathSize(pathSize);
  free(queue);
  free(graphNodes);
  free(revPath);

  return path;

}