@echo off
:: Create a build directory
:: cd build
:: cmake config CMakeLists.txt
:: cmake .. -A Win32 || exit /b %errorlevel%
:: build binary app
cmake --preset x64-debug || exit /b %errorlevel%

cmake --build --preset x64-debug || exit /b %errorlevel%