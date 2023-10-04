@echo off
mkdir "Build/Win_x64"
cd "Build/Win_x64"
cmake -G "Visual Studio 17 2022" -A x64 ../../