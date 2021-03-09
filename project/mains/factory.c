
//Use a selected Aglorithm so solve the maze.

#include "breadthfirst.c"

int (*factory(char *selected, Image_Dimentions im, Nodes *routeNodes, Nodes startNode, Nodes endNode))[2]{
  //create cases to make it easier to load
  const static char *listOfAlgorithms[4] = {"breadthfirst", "depthfirst", "astar", "leftturn"};
  char selection;
  for (int i = 0; i < 4; i++){
    if(listOfAlgorithms[i][0] == selected[0]) {
      selection = listOfAlgorithms[i][0];
    }
  }
  
  int (*path)[2] = malloc(sizeof(int[1][2]));
  switch(selection) {
  case 'b' :
    path = solver(im, routeNodes, startNode, endNode);
    break;
  case 1 :
    // depthFirst.
    break;
  case 2 :
    // astar
    break;
  case 3 :
    // leftturn
    break;
  default :
    path = solver(im, routeNodes, startNode, endNode);
  }

   return path;
}