g++ -o wavedesc.so -shared -fPIC $(python3-config --includes --cflags) -I../common wavedesc.cc
