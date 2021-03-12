void addNeighbourNorth(unsigned int position[2], Nodes *node){
  // Add North neighbour
  node->neighbours[0][0] = position[0];
  node->neighbours[0][1] = position[1];
}

void addNeighbourEast(unsigned int position[2], Nodes *node){
  // Add East neighbour
  node->neighbours[1][0] = position[0];
  node->neighbours[1][1] = position[1];
}

void addNeighbourSouth(unsigned int position[2], Nodes *node){
  // Add South neighbours
  node->neighbours[2][0] = position[0];
  node->neighbours[2][1] = position[1];
}

void addNeighbourWest(unsigned int position[2], Nodes *node){
  // Add West neighbour
  node->neighbours[3][0] = position[0];
  node->neighbours[3][1] = position[1];
}

void processAddingWENeighbours(unsigned int currRow, unsigned int currCol, unsigned int currLeftNode[2], Nodes *graphNodesW, Nodes *graphNodesE) {

  unsigned int neighbourWest[2] = {currLeftNode[0], currLeftNode[1]}; // West Neighbour
  addNeighbourWest(neighbourWest, graphNodesW);

  unsigned int neighbourEast[2] = {currRow, currCol};  // East Neighbour
  addNeighbourEast(neighbourEast, graphNodesE);
  currLeftNode[0] = currRow;
  currLeftNode[1] = currCol;

}

void setNorthNodes(unsigned int currRowVal, unsigned int currColVal, unsigned int graphNodesIndex, unsigned int *northNodes) {
  northNodes[0] = currRowVal;
  northNodes[1] = currColVal;
  northNodes[2] = graphNodesIndex;

}

void resetNorthNodes(unsigned int width, unsigned int *northNodes) {
  unsigned int moreThanWidth = width + 1;
  northNodes[0] = moreThanWidth;  
  northNodes[1] = moreThanWidth;  
  northNodes[2] = moreThanWidth;
}

void processAddingNSNeighbours(unsigned int currRow, unsigned int currCol, unsigned int width, unsigned int northNodes[3], Nodes *graphNodesN, Nodes *graphNodesS) {
  unsigned int neighbourNorth[2] = {northNodes[0], northNodes[1]};
  addNeighbourNorth(neighbourNorth, graphNodesN);

  
  unsigned int neighbourSouth[2] = {currRow, currCol};
  addNeighbourSouth(neighbourSouth, graphNodesS);        
  
  resetNorthNodes(width, northNodes);
}

struct Nodes *generatepath(ImageDimensions im, u_int8_t imagePixels[], Nodes startNode[], Nodes endNode[]) {
  unsigned int height = im.height;
  unsigned int width = im.width;
  unsigned int imageSize = height * width;
  unsigned int graphNodesIndex = 0;

  // Need a way to keep track of northNode for each column so we can connect north side of graph nodes
  // 3D array, elm[0] = position0, elm[1] = position[1], 
  // elm[3] = graphNodesIndex (index): need this so we can set south to the northNode as well in one iteration.
  unsigned int northNodes[width][3];
  
  // Intilize all northNode value to set number instead of grabage values
  // This helps with conditional when validating.
  for (unsigned int i = 0; i < width; i++)
  {
    // resetNorthNodes(width, northNodes[i]); @TODO prolly need pointer to a pointer to override list element in memory
    unsigned int moreThanWidth = width + 1;
    northNodes[i][0] = moreThanWidth;
    northNodes[i][1] = moreThanWidth;
    northNodes[i][2] = moreThanWidth;
  }
  

  /* 
    Allocate 32% of imageSize memory for graphNodes.
    If nodes size increase more than 32%, allocate another 16% and so on... logrithmic increment in memory allocation.
  */
  unsigned int graphNodeMemSize = ceil(imageSize * 0.32);
  Nodes *graphNodes = (Nodes *) malloc(graphNodeMemSize * sizeof(Nodes));

  /*
    Lets take care of start node. 
    Also add first graph node.
  */

  for (unsigned int i = 1; i < width - 1; i++){
    if (imagePixels[i] > 0){
      unsigned int position[2] = {1, i};

      graphNodes[graphNodesIndex].position[0] = 0;
      graphNodes[graphNodesIndex].position[1] = i;
      addNeighbourSouth(position, &graphNodes[graphNodesIndex]);
      
      startNode->position[0] = 0;
      startNode->position[1] = i;
      addNeighbourSouth(position, &startNode[0]);
      unsigned int currCol = i % width;
      setNorthNodes(0, i, graphNodesIndex, northNodes[currCol]);
      graphNodesIndex++;
      break;
    } 
  }

  
  unsigned int currLeftNode[2] = {0, 0};
  // Go through the whole image to generate graph nodes
  for (unsigned int i = width; i < imageSize - width; i++)
  {
    unsigned int currRow = (unsigned int) i / width;
    unsigned int currCol = i % width;

    unsigned int rowAbove = currRow - 1;
    unsigned int rowBelow = currRow + 1;

    unsigned int colBefore = i - 1;
    unsigned int colAfter = i + 1;

    unsigned int idxRowAbove = ((rowAbove * width) + currCol);
    unsigned int idxRowBelow = ((rowBelow * width) + currCol);

    if(imagePixels[i] == 0) {
      currLeftNode[0] = 0;
      currLeftNode[1] = 0;
      continue;
    }

    if(graphNodesIndex >= graphNodeMemSize) { 
      // Reallocate more memory
      // Reallocation can be done based index
      // If index is still half way through
      // Allocate same size again and 
      // If index is quater the way reallocate half of the orginal allocation and so on...
      unsigned int size = graphNodeMemSize / 2;
      graphNodeMemSize += size;

      allocateMoreMem(&graphNodes, graphNodeMemSize);
    }

    if(imagePixels[colBefore] == 0 && imagePixels[colAfter] > 0) { //WALL PATH PATH
      graphNodes[graphNodesIndex].position[0] = currRow;
      graphNodes[graphNodesIndex].position[1] = currCol;
      currLeftNode[0] = currRow;
      currLeftNode[1] = currCol;

      if(northNodes[currCol][0] != width + 1 && northNodes[currCol][1] != width + 1) { 
        processAddingNSNeighbours(currRow, currCol, width, northNodes[currCol], &graphNodes[graphNodesIndex], &graphNodes[northNodes[currCol][2]]);
      }
      if(imagePixels[idxRowBelow] > 0) {
        setNorthNodes(currRow, currCol, graphNodesIndex, northNodes[currCol]);
      }

      graphNodesIndex++;
    } else if(imagePixels[colBefore] > 0 && imagePixels[colAfter] == 0) { //PATH PATH WALL
      graphNodes[graphNodesIndex].position[0] = currRow;
      graphNodes[graphNodesIndex].position[1] = currCol;

      if(currLeftNode[0] != 0 && currLeftNode[1] != 0) { 
        processAddingWENeighbours(currRow, currCol, currLeftNode, &graphNodes[graphNodesIndex], &graphNodes[graphNodesIndex - 1]);
      } 

      if(northNodes[currCol][0] != width + 1 && northNodes[currCol][1] != width + 1) {  
        processAddingNSNeighbours(currRow, currCol, width, northNodes[currCol], &graphNodes[graphNodesIndex], &graphNodes[northNodes[currCol][2]]); 
      }
      if(imagePixels[idxRowBelow] > 0) {
        setNorthNodes(currRow, currCol, graphNodesIndex, northNodes[currCol]);
      }  
      graphNodesIndex++;
    } else if((imagePixels[idxRowAbove] > 0 || imagePixels[idxRowBelow] > 0) && (imagePixels[colBefore] > 0 && imagePixels[colAfter])) { //PATH PATH PATH
      graphNodes[graphNodesIndex].position[0] = currRow;
      graphNodes[graphNodesIndex].position[1] = currCol;
      if(currLeftNode[0] != 0 && currLeftNode[1] != 0) {
        processAddingWENeighbours(currRow, currCol, currLeftNode, &graphNodes[graphNodesIndex], &graphNodes[graphNodesIndex - 1]);
      }

      if(northNodes[currCol][0] != width + 1 && northNodes[currCol][1] != width + 1) { 
        processAddingNSNeighbours(currRow, currCol, width, northNodes[currCol], &graphNodes[graphNodesIndex], &graphNodes[northNodes[currCol][2]]);  
      }
      if(imagePixels[idxRowBelow] > 0) {
        setNorthNodes(currRow, currCol, graphNodesIndex, northNodes[currCol]);
      }
      graphNodesIndex++;
    }
  }

    /*
    Lets take care of last graph node. 
    Also add end graph node.
  */

  for (unsigned int i = (imageSize - width + 1); i < imageSize - 1; i++){
    if (imagePixels[i] > 0){
      unsigned int currRow = (unsigned int)i / width;
      unsigned int currCol = i % width;
      unsigned int position[2] = {currRow - 1, currCol};

      graphNodes[graphNodesIndex].position[0] = currRow;
      graphNodes[graphNodesIndex].position[1] = currCol;
      addNeighbourNorth(position, &graphNodes[graphNodesIndex]);
      
      endNode->position[0] = currRow;
      endNode->position[1] = currCol;
      addNeighbourNorth(position, &endNode[0]);
      // unsigned int currCol = i % width;
      // setNorthNodes(0, i, graphNodesIndex, northNodes[currCol]);
      graphNodesIndex++;
      break;
    } 
  }

  //Set how many graphNodes are there, traversal searches going to need a count/length;
  setGraphNodesSize(graphNodesIndex);
  // Uncomment below to see all the neighbours and nodes in graphNodes
  // Just FYI printf is expensive, if there are say about even 5k graphNodes it will take some time.
  // Don't try printing too many graphnodes will take forever
  // for (int i = 0; i < graphNodesIndex; i++){
  //   printf("Nodes[%i, %i] Neighbours{N[%i,%i], E[%i,%i], S[%i,%i], W[%i,%i]} at %i \n", graphNodes[i].position[0], graphNodes[i].position[1],graphNodes[i].neighbours[0][0],graphNodes[i].neighbours[0][1],graphNodes[i].neighbours[1][0],graphNodes[i].neighbours[1][1],graphNodes[i].neighbours[2][0],graphNodes[i].neighbours[2][1],graphNodes[i].neighbours[3][0],graphNodes[i].neighbours[3][1], (i + 1));
  // }  
  return graphNodes;
}