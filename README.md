# Maze-Solving
Solve a 2D image maze.
Work in Progress


To run this project you need a C/C++ compiler.
This project was created using Visual studio code. '.vscode' folder is included/PUSHED to this repo.
To build and run project open using Visual studio code and use CTRL (cmd) + SHIFT + B to build. and F5 to run.

***before building change/add specific path to your project in buid.bat (for Windows only) file. The file is located inside .vscode folder.

Change input/output images/path:
To change the input maze image, it can be done inside launch.json file inside .vscode folder.
To change the output solved image, it can also be done inside launch.json file inside .vscode folder.

Change which traversal algorithm to use to find path:
To change traversal algorithm, it can also be done inside launch.json file inside .vscode folder.
List of traversal algorithms available: 
  "breadthfirst" (done working), 
  "depthfirst" (in progress), 
  "astar" (in progress), 
  "leftturn" (in progress)