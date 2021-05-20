@echo off

SET LIBS=gdi32.lib kernel32.lib user32.lib Winmm.lib opengl32.lib glu32.lib strmiids.lib Winmm.lib Xinput.lib Ole32.lib


IF NOT EXIST build mkdir build

pushd build
cl /EHsc /Zi ..\code\*.cpp ..\code\glad.c /Fe:game %LIBS% ^
/I"D:\Dev\FPShooter\include" 
popd


