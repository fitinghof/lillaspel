@echo off
setlocal

if exist "build" (
    rmdir /s /q "build"
)

if exist "out\build" (
    rmdir /s /q "out\build"
)

endlocal