@echo off

mkdir build 
cd build
cmake .. -A Win32 || exit /b %errorlevel%
cmake --build . || exit /b %errorlevel%

cd Debug
lillaspel.exe