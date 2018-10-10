xcopy "lib" "bin"/y
mkdir build 
cd build
cmake -DCMAKE_SH="CMAKE_SH-NOTFOUND" -G "MinGW Makefiles" ..
cd ..
