
void assignValues(Nodes to[], Nodes from, int size){
    for(int i = 0; i < size; i++){
        to[i] = from;
    }
}

void assing_to_current(Nodes **current, Nodes from){
    *current = &from;
}


void unshift_element(Nodes *node, int array_size, Nodes *new_node){

    for (int i = array_size; i > 0; i--) {
        
        Nodes *temp_unshift = (Nodes *) malloc(sizeof(Nodes));
        
        memcpy(temp_unshift, &node[i - 1], sizeof(Nodes));
        
        node[i] = temp_unshift[0];
        
    }

    memcpy(&node[0], new_node, sizeof(Nodes));
}

void remove_element(Nodes *node, int index, int array_length)
{
    int i;
    for(i = index; i < array_length - 1; i++) node[i] = node[i + 1];
}

void getFromNodes(Nodes **storeTo, Nodes *route_nodes, int compare[2]){
    int number_of_nodes = getNodeSize(); 
    for (int i = 0; i < number_of_nodes; i++){
        if(route_nodes[i].position[0] == compare[0] && route_nodes[i].position[1] == compare[1] ){
           *storeTo = &route_nodes[i];            
        }
    }
}

int (*solver(Image_Dimentions im, Nodes route_nodes[], Nodes start_node, Nodes end_node))[2]{
    int height = im.height;
    int width = im.width;

    int image_size = height * width;
    Nodes *queue = (Nodes *) malloc(sizeof(Nodes));

    int (*prv)[2] = malloc(image_size * sizeof(int[1][2]));

    assignValues(queue, start_node, 1);

    int *visited = (int *)malloc(image_size *sizeof(int));

    for(int i = 0; i < image_size; i++){
        visited[i] = 0;
    }

    int count = 0;

    bool isCompleted = false;

    visited[start_node.position[0] * width + start_node.position[1]] = 1;

    int size = 1;
    
    while(queue){
        //intilize a size of queue so that we can use it add more memory on the way.
        count++;
        //get the current node as the last element from queue/pop queue
        Nodes *current = (Nodes *) malloc(sizeof(Nodes));

        assing_to_current(&current, queue[size - 1]);

        remove_element(queue, size - 1, size); 

        size = size - 1;

        Nodes *tmp = realloc(queue, (size) * sizeof(Nodes));
        if (tmp == NULL && size > 1) {
            /* No memory available */
            printf("No memory available");
        }
        queue = tmp;
         
        //printf("Data %i", queue->position[0]);
        
        if(current->position[0] == end_node.position[0] && current->position[1] == end_node.position[1]){
            isCompleted = true;
            break;
        }

        for(int ii = 0; ii < 4; ii++){
            if((current->neighbours[ii][0] != 0) && (current->neighbours[ii][1] != 0)){
                int nPos = current->neighbours[ii][0] * width + current->neighbours[ii][1];
                if(visited[nPos] == 0) {
                    prv[nPos][0] = current->position[0];
                    prv[nPos][1] = current->position[1];

                    visited[nPos] = 1;
                    
                    size++;

                    Nodes *temp = (Nodes *) malloc(sizeof(Nodes));

                    getFromNodes(&temp, route_nodes, current->neighbours[ii]);

                    addMemory(&queue, size, 1, -1);

                    unshift_element(queue, size, temp);
                }
            }
        }
    }
    



    int (*path)[2] = malloc(sizeof(int[1][2]));
    int (*ncurrent)[2] = malloc(sizeof(int[1][2]));
    ncurrent[0][0] = end_node.position[0];
    ncurrent[0][1] = end_node.position[1];
    int path_size = 0;
    
    while (ncurrent[0][1] && ncurrent[0][0]) {
        path_size++;
        
        for (int i = path_size; i > 0; i--) {

            int (*temp_unshift)[2] = malloc(sizeof(int[1][2]));
            
            memcpy(&temp_unshift[0], &path[i - 1], sizeof(int[1][2]));
            
            path[i][0] = temp_unshift[0][0];
            path[i][1] = temp_unshift[0][1];

            free(temp_unshift);
        }

        int (*temp_current)[2] = malloc(sizeof(int[1][2]));
        temp_current[0][0] = ncurrent[0][0];
        temp_current[0][1] = ncurrent[0][1];

        
        memcpy(&path[0], &temp_current[0], sizeof(int[1][2])); 
       int position = ncurrent[0][0] * width + ncurrent[0][1];
        ncurrent[0][0] = prv[position][0];
        ncurrent[0][1] = prv[position][1]; 

        free(temp_current); 
    }

    getPathSize(path_size);
    free(ncurrent);
    free(queue);
    free(route_nodes);
    free(prv);

    return path;

}