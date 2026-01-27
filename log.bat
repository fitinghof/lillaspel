@echo off
cd build
cd Debug
if /i "%1"=="clear" (
    type /dev/null > log.txt
    echo Log cleared
    echo Done!
) else (
    start notepad.exe "log.txt"
)
pause