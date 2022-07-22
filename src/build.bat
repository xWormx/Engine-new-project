@echo off

SET CJ_LIB="..\CJ_LIB"
SET GLAD="..\GLAD\include"
SET GLFW_INCLUDE="..\GLFW\glfw-3.3.bin.WIN64\include\GLFW"
SET GLFW_LIB="..\GLFW\glfw-3.3.bin.WIN64\lib-vc2019"
SET STB=C:\stb
SET IRRLANG_LIB=C:\Irrklang\irrKlang-64bit-1.6.0\lib\Winx64-visualStudio
SET IRRLANG_INCLUDE=C:\Irrklang\irrKlang-64bit-1.6.0\include

pushd ..\build

cl /F 800000000 /EHsc /MD /wd4312 -Zi -nologo /I %STB% /I %CJ_LIB% /I %GLFW_INCLUDE% /I %GLAD% /I %IRRLANG_INCLUDE% /wd4005 ..\src\engine.cpp  /link user32.lib gdi32.lib opengl32.lib shell32.lib /LIBPATH:%CJ_LIB% cj_vector.lib /LIBPATH:%GLFW_LIB% glfw3.lib glfw3dll.lib /LIBPATH:%IRRLANG_LIB% irrKlang.lib

popd


del *.un *.un~ *.cpp~ *.swp *.bat~ *.h~
