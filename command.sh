#!/bin/bash
#cd /D "F:\file-catalog\other\console-ui"
#echo %CD%
#::-lglfw3 -lgdiplus
#::-lopengl32 -lwinmm -lgdi32 -lm
#:: -DFREEGLUT_GLES
#::-lgdi32 -luser32 -lglu32 -lm -lwinmm
#::-DFREEGLUT_STATIC
#cmd /c "C:\Program Files\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\bin\g++.exe" -I. framebuffer.cpp box.cpp screen.cpp element.cpp -I..\..\console ..\..\console\console.windows.cpp %* -o console-ui.exe -ggdb -w -fpermissive
g++ -I. framebuffer.cpp box.cpp screen.cpp element.cpp -I../../console ../../console/console.linux.cpp $@ -o console-ui -ggdb -w -fpermissive -lncurses
#if "%errorlevel%" NEQ "0" (
#::start /b sam You are a dumb nigger.
#pause
#exit /b
#)
#console-ui.exe
#SET RRR=%errorlevel%
#if "%errorlevel%" NEQ "0" (
#echo Exited with error code %errorlevel%
#pause
#exit /b
#)
