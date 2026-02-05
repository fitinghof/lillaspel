@echo off
setlocal

call "%~dp0build.bat"
if errorlevel 1 exit /b %errorlevel%

call "%~dp0run.bat"
exit /b %errorlevel%