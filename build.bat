@echo off
:: Create a build directory
mkdir build 
cd build
:: cmake config CMakeLists.txt
cmake .. -A Win32 || exit /b %errorlevel%
:: build binary app
cmake --build . || exit /b %errorlevel%
pause