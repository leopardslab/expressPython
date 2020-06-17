cd ANTLR4runtime

rmdir /s /q build

rmdir /s /q run

mkdir build && mkdir run && cd build

cmake.exe ..  -G "MinGW Makefiles" -DCMAKE_SH="CMAKE_SH-NOTFOUND"

DESTDIR=../run mingw32-make.exe
