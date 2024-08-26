@echo off
for %%i in ("%~dp0.") do set "dir_name=%%~ni"

if not exist build (
  mkdir build
)

@REM  Add your own path. Run vcpkg integrate install to get it
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=C:\Users\samtn\vcpkg\scripts\buildsystems\vcpkg.cmake .. 
cmake --build .

cd ..
build\Debug\%dir_name%.exe
build\Debug\%dir_name%_test.exe
