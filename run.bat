@echo off
setlocal enabledelayedexpansion
for %%i in ("%~dp0.") do set "dir_name=%%~ni"


set "action=%1"
set "exe_path=build\Debug\%dir_name%.exe"
set "test_exe_path=build\Debug\%dir_name%_test.exe"

if "%action%"=="build" goto :build
if "%action%"=="run" goto :run
if "%action%"=="test" goto :test
if "%action%"=="clean" goto :clean
if "%action%"=="run_tests_new_window" goto :run_tests_new_window
if "%action%"=="" goto :run

echo Invalid action. Use 'build', 'run', 'test', 'clean', or 'run_tests_new_window'.
exit /b 1



:clean
echo Cleaning build directory...
if exist build (
    echo Removing existing build directory...
    rmdir /s /q build
    if !errorlevel! neq 0 (
        echo Failed to remove build directory.
        exit /b !errorlevel!
    )
)
echo Creating new build directory...
mkdir build
if !errorlevel! neq 0 (
    echo Failed to create build directory.
    exit /b !errorlevel!
)
echo Clean completed successfully.
goto :eof


:build
if not exist build mkdir build
echo Building the project...
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=C:\Users\samtn\vcpkg\scripts\buildsystems\vcpkg.cmake ..
if !errorlevel! neq 0 (
    echo CMake configuration failed.
    exit /b !errorlevel!
)
cmake --build .
if !errorlevel! neq 0 (
    echo Build failed.
    exit /b !errorlevel!
)
cd ..
echo Build completed successfully.
goto :eof

:run
if not exist "%exe_path%" (
    echo Executable not found. Building the project...
    call :build
    if !errorlevel! neq 0 exit /b !errorlevel!
)
echo Running executable...
"%exe_path%"
exit /b !errorlevel!

:test
echo Performing clean build and running tests...
call :clean
if !errorlevel! neq 0 exit /b !errorlevel!
call :build
if !errorlevel! neq 0 exit /b !errorlevel!
echo Running tests...
"%test_exe_path%"
exit /b !errorlevel!


:run_tests_new_window
start "Running Tests" cmd /c ""%~f0" test & pause"
exit /b 0
