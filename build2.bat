@echo off
IF NOT EXIST build mkdir build
pushd build

cl /EHsc /Od /Oi -wo4244 -Zi  /I..\dependencies\glfw\include /I..\dependencies\glad\include /I..\dependencies\glm /I..\dependencies\stb -MP ../*.cpp ../glad.c /Fe"..\opengl.exe"  /link -subsystem:console ..\dependencies\glfw\lib\glfw3_mt.lib user32.lib Gdi32.lib Shell32.lib
popd
