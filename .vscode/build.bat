@echo off

rem change the path below where you project is downlaoded or cloned on machine.
set projectPath={your_path}\MazeC

call "C:\Program Files (x86)\Microsoft Visual Studio 14.0\VC\vcvarsall.bat" x64 8.1

set compilerflags=/Od /Zi /EHsc
set outPath=%projectPath%\project\compiled\solve.exe
set linkerflags=/OUT:%outPath%
set compile=%projectPath%\project\mains\solve.c

cl.exe %compilerflags% %compile% /link %linkerflags%