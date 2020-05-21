@echo off
cd /D "H:\cpp-projects\console-ui"
CALL ..\libs.bat
echo %CD%
::-lglfw3 -lgdiplus
::-lopengl32 -lwinmm -lgdi32 -lm
:: -DFREEGLUT_GLES
::-lgdi32 -luser32 -lglu32 -lm -lwinmm
::-DFREEGLUT_STATIC
cmd /c g++ -I. framebuffer.cpp box.cpp screen.cpp element.cpp -I%CONSOLE% %WCONSOLE% %ADV% %* -o console-ui.exe -ggdb -w -fpermissive
if "%errorlevel%" NEQ "0" (
::start /b sam You are a dumb nigger.
pause
exit /b
)
console-ui.exe
SET RRR=%errorlevel%
if "%errorlevel%" NEQ "0" (
echo Exited with error code %errorlevel%
pause
exit /b
)
