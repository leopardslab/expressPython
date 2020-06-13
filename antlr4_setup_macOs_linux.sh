cd ANTLR4runtime

rm -rf build 

mkdir build && mkdir run && cd build

cmake ..

DESTDIR=../run make install
