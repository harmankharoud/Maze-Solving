

//mapping to inlculde different algorithms to solve the maze

int (*factory(int selection, Image_Dimentions im, Nodes *route_nodes, Nodes start_node, Nodes end_node))[2]{
    //create cases to make it easier to load
    //[breadthfirst, depthfirst, astar, leftturn]
    int (*path)[2] = malloc(sizeof(int[1][2]));

    //#include "breadthfirst.c"
    path = solver(im, route_nodes, start_node, end_node);
//      switch(selection) {
//       case 0 :
//         #include "breadthfirst.c"
//         path = solver(im, route_nodes, start_node, end_node);
//         break;
//       case 1 :
//         //#include "depthfirst.c" 
//         break;
//       case 2 :
//         //#include "astar.c" 
//         break;
//       case 3 :
//        // #include "leftturn.c" 
//         break;
//       default :
//         #include "breadthfirst.c"
//         path = solver(im, route_nodes, start_node, end_node);
//    }

   return path;
}