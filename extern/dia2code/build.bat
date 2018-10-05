
mkdir build 
cd build
cmake -G "MinGW Makefiles" ..
mingw32-make -s -j4
REM cmake builds makefiles for mingw so we need to use mingw32-make
cd ..
