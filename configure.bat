xcopy "lib" "bin"/y
mkdir build 
cd build
cmake -G "MinGW Makefiles" ..
cd ..