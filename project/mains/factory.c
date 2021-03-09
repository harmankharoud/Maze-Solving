
//Use a selected Aglorithm so solve the maze.

#include "breadthfirst.c"

int (*factory(char *selected, Image_Dimentions im, Nodes *routeNodes, Nodes startNode, Nodes endNode))[2]{
  
  int (*path)[2] = malloc(sizeof(int[1][2]));
  switch(selected[0]) {
  case 'b' :
    path = bfs(im, routeNodes, startNode, endNode);
    break;
  case 'd' :
    // depthFirst.
    break;
  case 'a' :
    // astar
    break;
  case 'l' :
    // leftturn
    break;
  default :
    path = bfs(im, routeNodes, startNode, endNode);
  }

   return path;
}