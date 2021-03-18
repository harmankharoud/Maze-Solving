
//Use a selected Aglorithm so solve the maze.

#include "./traversalalgorithms/breadthfirst.c"

unsigned int (*factory(char *selected, ImageDimensions im, Nodes *graphNodes, Nodes startNode, Nodes endNode))[2]{
  
  unsigned int (*path)[2] = malloc(sizeof(int[1][2]));
  switch(selected[0]) {
  case 'b' :
    path = bfs(im, graphNodes, startNode, endNode);
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
    path = bfs(im, graphNodes, startNode, endNode);
  }

   return path;
}