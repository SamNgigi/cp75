@echo off
setlocal enabledelayedexpansion
for %%i in ("%~dp0.") do set "dir_name=%%~ni"


set "exe_path=build\Debug\%dir_name%.exe"
set "test_exe_path=build\Debug\%dir_name%_test.exe"

if "%1"=="" goto :run

:process_actions
if "%1"=="" goto :eof
if "%1"=="build" goto :build
if "%1"=="run" goto :run
if "%1"=="test" goto :test
if "%1"=="clean" goto :clean
if "%1"=="run_tests_new_window" goto :run_tests_new_window

echo Invalid action: %1
echo Valid actions are: build, run, test, clean, run_tests_new_window
shift
goto :process_actions



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
shift
goto :process_actions

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
shift
goto :process_actions


:run
if not exist "%exe_path%" (
    echo Executable not found. Building the project...
    call :build
    if !errorlevel! neq 0 exit /b !errorlevel!
)
echo Running executable...
"%exe_path%"
shift
goto :process_actions


:test
echo Performing clean build and running tests...
call :clean
if !errorlevel! neq 0 exit /b !errorlevel!
call :build
if !errorlevel! neq 0 exit /b !errorlevel!
echo Running tests...
"%test_exe_path%"
shift
goto :process_actions

:run_tests_new_window
start "Running Tests" cmd /c ""%~f0" test & pause"
shift
goto :process_actions
