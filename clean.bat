@echo off

if /i "%1"=="debug" (
    :: If only debug clean
    if exist "out\build\x64-debug\Debug" (
        rmdir /s /q "out\build\x64-debug\Debug"
        echo Deleted x64-debug Debug folder
    )
) 
:: If only release clean
if /i "%1"=="release" (
    if exist "out\build\x64-release\Release" (
        rmdir /s /q "out\build\x64-release\Release"
        echo Deleted x64-release Release folder
    )
) 
:: If all clean
if /i "%1"=="all" (
    if exist "out\build\x64-debug\Debug" (
        rmdir /s /q "out\build\x64-debug\Debug"
        echo Deleted x64-debug Debug folder
    )
    if exist "out\build\x64-release\Release" (
        rmdir /s /q "out\build\x64-release\Release"
        echo Deleted x64-release Release folder
    )
) 

if /i "%1"=="full" (
    :: remove entire build dir to make sure new build is correct
    if exist "out" (
        rmdir /s /q "out"
        echo Deleted out folder
    )
) 

:: Fallback clean == debug clean
if exist "out\build\x64-debug\Debug" (
    rmdir /s /q "out\build\x64-debug\Debug"
    echo Deleted x64-debug Debug folder
)
        
    

