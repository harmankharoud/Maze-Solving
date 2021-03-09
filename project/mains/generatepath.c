void addNeighbours(u_int8_t position[4][2], Nodes node[]){
  //add a correct neighbour to a nodes
  for(int i = 0; i < 4; i++){
    node->neighbours[i][0] = position[i][0];
    node->neighbours[i][1] = position[i][1];
  }
}

void addSouthNeighbour(Nodes pathNodes[], Nodes node[], unsigned int southNode[2]){    
  int size = getNodeSize();
  for(int i = 0; i < size; i++){
    if(pathNodes[i].position[0] == southNode[0] && pathNodes[i].position[1] == southNode[1]){
      pathNodes[i].neighbours[2][0] = node->position[0];
      pathNodes[i].neighbours[2][1] = node->position[1];
      break;
    }
  }
}

void addLastRowAsSouth(int compare[2], Nodes *pathNodes){
  int size = getNodeSize();
  int southNode[2] = {0};
  southNode[0] = compare[0] - 1;
  southNode[1] = compare[1];
  for(int i = 0; i < size; i++){
    if(pathNodes[i].position[1] == compare[1] && pathNodes[i].position[0] != compare[0] - 1){
      southNode[0] = pathNodes[i].position[0];
    }
  }
  for(int i = 0; i < size; i++){
    if(pathNodes[i].position[0] == southNode[0] && pathNodes[i].position[1] == southNode[1]){
      pathNodes[i].neighbours[2][0] = compare[0] - 1;
      pathNodes[i].neighbours[2][1] = compare[1];
      break;
    }
  }
}

struct Nodes *generatepath(ImageDimensions im, unsigned char imagePixels[], Nodes routeNodes[], Nodes startNode[], Nodes endNode[]) {
  int height = im.height;
  int width = im.width;
  int imageSize = height * width;
  
  //find the index in the data/imagePixels 
  // check if its first one after the wall
  //check if there is node above it
  //check if there is node below it
  //check if it is the last one before wall and ther is node below it.

  //adding neighbours
  // for first and last we have abvious neighbours. So just plug those in right away.
  // rest can be added in the end, at last

  int size = 1;
  Nodes *topNode = (Nodes *) malloc(width *sizeof(Nodes));

  for (int i = 1; i < width - 1; i++){
    if (imagePixels[i] > 0){
      routeNodes[0].position[0] = 0;
      routeNodes[0].position[1] = i;

      u_int8_t position[4][2] = {{0,0}, {0,0}, {routeNodes[0].position[0] + 1, routeNodes[0].position[1]}, {0,0}};

      addNeighbours(position, &routeNodes[0]);
      
      startNode->position[0] = 0;
      startNode->position[1] = i;

      addNeighbours(position, &startNode[0]);

      topNode[i].position[0] = 0;
      topNode[i].position[1] = i;

      break;
    } 
  }

  for (int i = 1; i < height - 1; i++){
      
    //get the rows current/above/below

    int rowOffset = width * i;
    int rowAboveOffset = rowOffset - width;
    int rowBelowOffset = rowOffset + width;
    
    bool prev = false;
    bool curr = false;

    bool nxt = imagePixels[rowOffset + 1] > 0 ? true : false;

    int leftNode[3] = {0};

    for(int j = 1; j < width - 1; j++){
        
      prev = curr;
      curr = nxt;

      nxt = imagePixels[rowOffset + j + 1] > 0 ? true : false; 

      if(curr == false) continue;

      bool n = false;

      if(prev == true){
        if(nxt == true){
          //PATH PATH PATH
          if(imagePixels[rowAboveOffset + j] > 0 || imagePixels[rowBelowOffset + j] > 0){
            size = addMemory(&routeNodes, size, 1, 0);
            routeNodes[size - 1].position[0] = i;
            routeNodes[size - 1].position[1] = j;
            n = true;
            u_int8_t positions[4][2] = {{0,0}, {0,0}, {0, 0}, {leftNode[0], leftNode[1]}};
            addNeighbours(positions, &routeNodes[size - 1]);

            u_int8_t positionsZero[4][2] = {{routeNodes[leftNode[2]].neighbours[0][0], routeNodes[leftNode[2]].neighbours[0][1]}, {routeNodes[size - 1].position[0], routeNodes[size - 1].position[1]}, {routeNodes[leftNode[2]].neighbours[2][0], routeNodes[leftNode[2]].neighbours[2][1]}, {routeNodes[leftNode[2]].neighbours[3][0], routeNodes[leftNode[2]].neighbours[3][1]}};
            addNeighbours(positionsZero, &routeNodes[leftNode[2]]);

            leftNode[0] = i;
            leftNode[1] = j;
            leftNode[2] = size - 1;
          }
        } else{  
          //PATH PATH WALL   
          n = true;                   
          size = addMemory(&routeNodes, size, 1, 0);
          routeNodes[size - 1].position[0] = i;
          routeNodes[size - 1].position[1] = j;

          u_int8_t positions[4][2] = {{0,0}, {0,0}, {0,0}, {leftNode[0], leftNode[1]}};

          addNeighbours(positions, &routeNodes[size - 1]);

          u_int8_t positionsZero[4][2] = {{routeNodes[leftNode[2]].neighbours[0][0], routeNodes[leftNode[2]].neighbours[0][1]}, {routeNodes[size - 1].position[0], routeNodes[size - 1].position[1]}, {routeNodes[leftNode[2]].neighbours[2][0], routeNodes[leftNode[2]].neighbours[2][1]}, {routeNodes[leftNode[2]].neighbours[3][0], routeNodes[leftNode[2]].neighbours[3][1]}};
          addNeighbours(positionsZero, &routeNodes[leftNode[2]]);
          
          leftNode[0] = 0;
          leftNode[1] = 0;
          leftNode[2] = size - 1;
        }
      } else{
        if(nxt == true){ 
          n = true;  
          //WALL PATH PATH                
          size = addMemory(&routeNodes, size, 1, 0);
          routeNodes[size - 1].position[0] = i;
          routeNodes[size - 1].position[1] = j;

          u_int8_t positions[4][2] = {{0,0}, {0,0}, {0,0}, {0,0}};

          addNeighbours(positions, &routeNodes[size - 1]);

          leftNode[0] = i;
          leftNode[1] = j;
          leftNode[2] = size - 1;
                
        } else{
          if(imagePixels[rowAboveOffset + j] == 0 || imagePixels[rowBelowOffset + j] == 0){  
            //WALL PATH WALL 
            n = true;                     
            size = addMemory(&routeNodes, size, 1, 0);
            routeNodes[size - 1].position[0] = i;
            routeNodes[size - 1].position[1] = j;

            u_int8_t positions[4][2] = {{0,0}, {0,0}, {0,0}, {0,0}};

            addNeighbours(positions, &routeNodes[size - 1]);
              
          }
        }
      }

      if(n == true){
        if (imagePixels[rowAboveOffset + j] > 0) {
          u_int8_t positions[4][2] = {{topNode[j].position[0], topNode[j].position[1]}, {routeNodes[size - 1].neighbours[1][0], routeNodes[size - 1].neighbours[1][1]}, {routeNodes[size - 1].neighbours[2][0], routeNodes[size - 1].neighbours[2][1]}, {routeNodes[size - 1].neighbours[3][0], routeNodes[size - 1].neighbours[3][1]}};
          addNeighbours(positions, &routeNodes[size - 1]);

          //adding south is bit of a work. so this should take care off it. but really a slow approach
          addSouthNeighbour(&routeNodes[0], &routeNodes[size - 1], topNode[j].position);
        }

        if (imagePixels[rowBelowOffset + j] > 0){
          topNode[j].position[0] = routeNodes[size - 1].position[0];
          topNode[j].position[1] = routeNodes[size - 1].position[1];
        } else{
          topNode[j].position[0] = 0;
          topNode[j].position[1] = 0;
        }
      }
    }
  }  


  int rowOffSet = (height - 1) * width;
  for (int i = 1; i < width - 1; i++){
    if (imagePixels[rowOffSet + i] > 0){            
      size = addMemory(&routeNodes, size, 1, 0);
      routeNodes[size - 1].position[0] = height - 1;
      routeNodes[size - 1].position[1] = i;

      u_int8_t positions[4][2] = {{height - 2, i}, {0,0}, {0,0}, {0,0}};

      addNeighbours(positions, &routeNodes[size - 1]);

      //special case to create path for the second last row since while ends before it can add the last row as south neighbour.
      int compare[2] = {height, i}; 
      addLastRowAsSouth(compare, routeNodes);
      
      endNode->position[0] = height - 1;
      endNode->position[1] = i;

      addNeighbours(positions, &endNode[0]);
      break;
    } 
  }


  //uncomment this to see all the neighbours and node are added
  // int numberOfNodes = getNodeSize();
  // for (int i = 0; i < numberOfNodes; i++){
  //     printf("Nodes[%i, %i] Neighbours{N[%i,%i], E[%i,%i], S[%i, %i], W[%i,%i]} at %i\n", routeNodes[i].position[0], routeNodes[i].position[1],routeNodes[i].neighbours[0][0],routeNodes[i].neighbours[0][1],routeNodes[i].neighbours[1][0],routeNodes[i].neighbours[1][1],routeNodes[i].neighbours[2][0],routeNodes[i].neighbours[2][1],routeNodes[i].neighbours[3][0],routeNodes[i].neighbours[3][1], (i + 1));
  // }  

  return routeNodes;
}