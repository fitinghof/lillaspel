@echo off
if exist "out\build\x64-debug\Debug\lillaspel.exe" (
    call "out\build\x64-debug\Debug\lillaspel.exe"
) else (
    echo executable file didn't exist, build first
)