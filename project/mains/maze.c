void addNeighbours(int position[4][2], Nodes node[]){
    //add a correct neighbour to a nodes
    for(int i = 0; i < 4; i++){
        node->neighbours[i][0] = position[i][0];
        node->neighbours[i][1] = position[i][1];
    }
}

void addSouthNeighbour(Nodes path_nodes[], Nodes node[], int southNode[]){    
    int size = getNodeSize();
    for(int i = 0; i < size; i++){
       if(path_nodes[i].position[0] == southNode[0] && path_nodes[i].position[1] == southNode[1]){
            path_nodes[i].neighbours[2][0] = node->position[0];
            path_nodes[i].neighbours[2][1] = node->position[1];
            break;
       }
    }
}

void addLastRowAsSouth(int compare[2], Nodes *path_nodes){
    int size = getNodeSize();
    int southNode[2] = {0};
    southNode[0] = compare[0] - 1;
    southNode[1] = compare[1];
    for(int i = 0; i < size; i++){
        if(path_nodes[i].position[1] == compare[1] && path_nodes[i].position[0] != compare[0] - 1){
            southNode[0] = path_nodes[i].position[0];
        }
    }
    for(int i = 0; i < size; i++){
       if(path_nodes[i].position[0] == southNode[0] && path_nodes[i].position[1] == southNode[1]){
            path_nodes[i].neighbours[2][0] = compare[0] - 1;
            path_nodes[i].neighbours[2][1] = compare[1];
            break;
       }
    }

}

unsigned char *Maze(Image_Dimentions im, unsigned char image_pixels[], Nodes route_nodes[], Nodes start_node[], Nodes end_node[]) {

    int im_data = 1;

    int height = im.height;
    int width = im.width;

    int image_size = height * width;
    
    //find the index in the data/image_pixels 
    // check if its first one after the wall
    //check if there is node above it
    //check if there is node below it
    //check if it is the last one before wall and ther is node below it.

    //adding neighbours
    // for first and last we have abvious neighbours. So just plug those in right away.
    // rest can be added in the end, at last

    int size = 1;
    Nodes *top_node = (Nodes *) malloc(width *sizeof(Nodes));

    for (int i = 1; i < width - 1; i++){
        if (image_pixels[i] > 0){
            route_nodes[0].position[0] = 0;
            route_nodes[0].position[1] = i;

            int position[4][2] = {{0,0}, {0,0}, {route_nodes[0].position[0] + 1, route_nodes[0].position[1]}, {0,0}};

            addNeighbours(position, &route_nodes[0]);
            
            start_node->position[0] = 0;
            start_node->position[1] = i;

            addNeighbours(position, &start_node[0]);

            top_node[i].position[0] = 0;
            top_node[i].position[1] = i;

            break;
        } 
    }

    for (int i = 1; i < height - 1; i++){
        
        //get the rows current/above/below

        int row_offset = width * i;
        int row_above_offset = row_offset - width;
        int row_below_offset = row_offset + width;
        
        bool prv = false;
        bool cur = false;

        bool nxt = image_pixels[row_offset + 1] > 0 ? true : false;

        int leftNode[3] = {0};

        for(int j = 1; j < width - 1; j++){
            
            prv = cur;
            cur = nxt;

            nxt = image_pixels[row_offset + j + 1] > 0 ? true : false; 

            if(cur == false) continue;

            bool n = false;

            if(prv == true){
                if(nxt == true){
                    //PATH PATH PATH
                    if(image_pixels[row_above_offset + j] > 0 || image_pixels[row_below_offset + j] > 0){
                        size = addMemory(&route_nodes, size, 1, 0);
                        route_nodes[size - 1].position[0] = i;
                        route_nodes[size - 1].position[1] = j;
                        n = true;
                        int positions[4][2] = {{0,0}, {0,0}, {0, 0}, {leftNode[0], leftNode[1]}};
                        addNeighbours(positions, &route_nodes[size - 1]);

                         int positionsZero[4][2] = {{route_nodes[leftNode[2]].neighbours[0][0], route_nodes[leftNode[2]].neighbours[0][1]}, {route_nodes[size - 1].position[0], route_nodes[size - 1].position[1]}, {route_nodes[leftNode[2]].neighbours[2][0], route_nodes[leftNode[2]].neighbours[2][1]}, {route_nodes[leftNode[2]].neighbours[3][0], route_nodes[leftNode[2]].neighbours[3][1]}};
                        addNeighbours(positionsZero, &route_nodes[leftNode[2]]);

                        leftNode[0] = i;
                        leftNode[1] = j;
                        leftNode[2] = size - 1;

                    }
                } else{  
                    //PATH PATH WALL   
                        n = true;                   
                        size = addMemory(&route_nodes, size, 1, 0);
                        route_nodes[size - 1].position[0] = i;
                        route_nodes[size - 1].position[1] = j;

                        int positions[4][2] = {{0,0}, {0,0}, {0,0}, {leftNode[0], leftNode[1]}};

                        addNeighbours(positions, &route_nodes[size - 1]);

                         int positionsZero[4][2] = {{route_nodes[leftNode[2]].neighbours[0][0], route_nodes[leftNode[2]].neighbours[0][1]}, {route_nodes[size - 1].position[0], route_nodes[size - 1].position[1]}, {route_nodes[leftNode[2]].neighbours[2][0], route_nodes[leftNode[2]].neighbours[2][1]}, {route_nodes[leftNode[2]].neighbours[3][0], route_nodes[leftNode[2]].neighbours[3][1]}};
                        addNeighbours(positionsZero, &route_nodes[leftNode[2]]);
                        
                        leftNode[0] = 0;
                        leftNode[1] = 0;
                        leftNode[2] = size - 1;
                }
            } else{
                if(nxt == true){ 
                    n = true;  
                    //WALL PATH PATH                
                        size = addMemory(&route_nodes, size, 1, 0);
                        route_nodes[size - 1].position[0] = i;
                        route_nodes[size - 1].position[1] = j;

                        int positions[4][2] = {{0,0}, {0,0}, {0,0}, {0,0}};

                        addNeighbours(positions, &route_nodes[size - 1]);

                        leftNode[0] = i;
                        leftNode[1] = j;
                        leftNode[2] = size - 1;
                        
                } else{
                    if(image_pixels[row_above_offset + j] == 0 || image_pixels[row_below_offset + j] == 0){  
                        //WALL PATH WALL 
                        n = true;                     
                        size = addMemory(&route_nodes, size, 1, 0);
                        route_nodes[size - 1].position[0] = i;
                        route_nodes[size - 1].position[1] = j;

                        int positions[4][2] = {{0,0}, {0,0}, {0,0}, {0,0}};

                        addNeighbours(positions, &route_nodes[size - 1]);
                        
                    }
                }
            }

            if(n == true){
                 if (image_pixels[row_above_offset + j] > 0) {
                    int positions[4][2] = {{top_node[j].position[0], top_node[j].position[1]}, {route_nodes[size - 1].neighbours[1][0], route_nodes[size - 1].neighbours[1][1]}, {route_nodes[size - 1].neighbours[2][0], route_nodes[size - 1].neighbours[2][1]}, {route_nodes[size - 1].neighbours[3][0], route_nodes[size - 1].neighbours[3][1]}};
                    addNeighbours(positions, &route_nodes[size - 1]);

                    //adding south is bit of a work. so this should take care off it. but really a slow approach
                    addSouthNeighbour(&route_nodes[0], &route_nodes[size - 1], top_node[j].position);
                }

                if (image_pixels[row_below_offset + j] > 0){
                    top_node[j].position[0] = route_nodes[size - 1].position[0];
                    top_node[j].position[1] = route_nodes[size - 1].position[1];
                } else{
                    top_node[j].position[0] = 0;
                    top_node[j].position[1] = 0;
                }
            }
        }
    }  


    int row_offSet = (height - 1) * width;
    for (int i = 1; i < width - 1; i++){
        if (image_pixels[row_offSet + i] > 0){            
            size = addMemory(&route_nodes, size, 1, 0);
            route_nodes[size - 1].position[0] = height - 1;
            route_nodes[size - 1].position[1] = i;

            int positions[4][2] = {{height - 2, i}, {0,0}, {0,0}, {0,0}};

            addNeighbours(positions, &route_nodes[size - 1]);

            //special case to create path for the second last row since while ends before it can add the last row as south neighbour.
            int compare[2] = {height, i}; 
            addLastRowAsSouth(compare, route_nodes);
            
            end_node->position[0] = height - 1;
            end_node->position[1] = i;

            addNeighbours(positions, &end_node[0]);
            break;
        } 
    }


    //uncomment this to see all the neighbours and node are added
    // int number_of_nodes = getNodeSize();
    // for (int i = 0; i < number_of_nodes; i++){
    //     printf("Nodes[%i, %i] Neighbours{N[%i,%i], E[%i,%i], S[%i, %i], W[%i,%i]} at %i\n", route_nodes[i].position[0], route_nodes[i].position[1],route_nodes[i].neighbours[0][0],route_nodes[i].neighbours[0][1],route_nodes[i].neighbours[1][0],route_nodes[i].neighbours[1][1],route_nodes[i].neighbours[2][0],route_nodes[i].neighbours[2][1],route_nodes[i].neighbours[3][0],route_nodes[i].neighbours[3][1], (i + 1));
    // }  

    return route_nodes;
}