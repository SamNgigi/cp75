@echo off

set "run_exe=0"
set "run_tests=0"

if "%1" == "" (
  set "run_exe=1"
  set "run_tests=1"
) else (
  if "%1" == "exe" set "run_exe=1"
  if "%1" == "tests" set "run_tests=1"
  if "%1" == "both" (
    set "run_exe=1"
    set "run_tests=1"
  )
)

for %%i in ("%~dp0.") do set "dir_name=%%~ni"

if not exist build (
  mkdir build
)

@REM  Add your own path. Run vcpkg integrate install to get it
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=C:\Users\samtn\vcpkg\scripts\buildsystems\vcpkg.cmake .. 
cmake --build .
cd ..

if %run_exe%==1 (
  echo Running executable...
  build\Debug\%dir_name%.exe
)

if %run_tests%==1 (
  echo Running tests...
  build\Debug\%dir_name%_test.exe
)




